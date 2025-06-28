package org.cpd2425.t09g13;

import java.io.IOException;
import java.net.Socket;
import java.security.NoSuchAlgorithmException;
import java.util.*;
import java.util.stream.Collectors;

import org.cpd2425.t09g13.client.StateMachine;
import org.cpd2425.t09g13.client.TUI;
import org.cpd2425.t09g13.protocol.Channel;
import org.cpd2425.t09g13.protocol.Message;
import org.cpd2425.t09g13.protocol.auth.*;
import org.cpd2425.t09g13.protocol.client.*;
import org.cpd2425.t09g13.protocol.server.*;
import org.cpd2425.t09g13.utils.Hasher;
import org.cpd2425.t09g13.utils.Panic;
import org.msgpack.core.MessagePack;
import org.msgpack.core.MessagePacker;
import org.msgpack.core.MessageUnpacker;

import com.googlecode.lanterna.gui2.*;
import com.googlecode.lanterna.TerminalSize;
import com.googlecode.lanterna.TextColor;
import com.googlecode.lanterna.screen.Screen;
import com.googlecode.lanterna.screen.TerminalScreen;
import com.googlecode.lanterna.terminal.DefaultTerminalFactory;
import com.googlecode.lanterna.terminal.Terminal;

/**
 * Client
 */
public class Client implements TUI {
    private String host;
    private Integer port;
    private Socket conn;
    private StateMachine state;
    private MultiWindowTextGUI gui;
    private Optional<Cookie> cookie;
    private List<String> rooms;
    private Optional<String> currentRoom;
    private ArrayDeque<Message> messages;
    private Optional<String> pendingMsm;
    private HashMap<String, Component> components;
    
    private MessagePacker output;
    private MessageUnpacker input;

    private final Integer MAX_MSG = 60;

    public Client(String host, Integer port) throws IOException {
        this.host = host;
        this.port = port;
        this.state = StateMachine.Disconnected;
        this.cookie = Optional.empty();

        // drawList State
        this.rooms = new LinkedList<>();
        this.currentRoom = Optional.empty();

        // drawRoom State
        this.messages = new ArrayDeque<>(MAX_MSG);
        this.pendingMsm = Optional.empty();

        this.components = new HashMap<>();
        this.components.put("roomText", new TextBox());
        this.components.put("aiCheck", new CheckBox());
        this.components.put("aiText", new TextBox("Summarize the most recent message you receive."));
        this.components.put("msgText", new TextBox(new TerminalSize(999, 1)));

        Terminal terminal = new DefaultTerminalFactory().createTerminal();
        Screen screen = new TerminalScreen(terminal);
        screen.startScreen();
        this.gui = new MultiWindowTextGUI(screen, new DefaultWindowManager(),
                new EmptySpace(TextColor.ANSI.BLUE));
        this.gui.setBlockingIO(false);
        this.gui.setEOFWhenNoWindows(true);
    }

    public void run() throws IOException {
        Thread.ofVirtual().start(() -> {
            List<Integer> timeouts = new LinkedList<>(Arrays.asList(0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144));
            while (true) {
                try {
                    Thread.sleep(timeouts.getFirst() * 1000);
                    timeouts.removeFirst();
                    this.conn = new Socket(this.host, this.port);
                    this.output = MessagePack.newDefaultPacker(conn.getOutputStream());
                    this.input = MessagePack.newDefaultUnpacker(conn.getInputStream());

                    timeouts = new LinkedList<>(Arrays.asList(0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144));

                    listenToServer(this.input);

                } catch (Exception e) {
                    if (timeouts.isEmpty()) {
                        System.out.println("[ERR] Could not reconnect to the server. Exiting...");
                        System.exit(1);
                    }
                    Integer timeout = timeouts.getFirst();
                    System.out.println("[WARN] Connection broke. Retrying in " + timeout + " second(s)...");
                    System.out.println("Exception: " + e);
                }
            }
        });
        this.draw();
        while (true) {
            Window current = this.gui.getActiveWindow();
            if (current == null) continue;
            current.waitUntilClosed();
        }
    }

    private void listenToServer(MessageUnpacker input) throws IOException {
        while (true) {
            if (this.conn.isClosed()) throw new IOException("Connection is closed!");
            Optional<ServerPkt> pktWrapper = ServerPkt.from(input);

            if (pktWrapper.isEmpty()) {
                System.err.println("[WARN] Received an empty packet from server.");
                continue;
            }

            ServerPkt pkt = pktWrapper.get();

            System.out.println("[INFO] Old State: " + this.state);
            System.out.println("[INFO] Got Pkt '" + pkt + "'.");

            switch (this.state) {
                case Disconnected:
                    input.close();
                    System.err.println("[ERR] Received a message from server while in the state '" + this.state + "'.");
                    break;

                case Connecting:
                    switch (pkt) {
                        case ConnectAckPkt(Cookie c):
                            this.cookie = Optional.of(c);
                            this.state = StateMachine.OutsideCh;
                            new GetChsPkt(this.cookie.get()).to(output);
                            break;

                        case RejPkt(int reason):
                            if (reason == RejPkt.INVALID_CREDENTIALS) {
                                this.drawErrorAndWait("Wrong username and/or password.", StateMachine.Disconnected);
                            } else if (reason == RejPkt.ALREADY_CONNECTED) {
                                this.drawErrorAndWait("This user is already authenticated.", StateMachine.Disconnected);
                            } else {
                                input.close();
                                System.err.println("[ERR] In '" + this.state + "', got 'RejPkt' of reason #" + reason + ".");
                            }
                            break;

                        default:
                            input.close();
                            System.err.println("[ERR] In '" + this.state + "', got '" + pkt + "'.");
                    }
                    break;

                case Registering:
                    switch (pkt) {
                        case ConnectAckPkt(Cookie c):
                            this.cookie = Optional.of(c);
                            this.state = StateMachine.OutsideCh;
                            new GetChsPkt(this.cookie.get()).to(output);
                            break;

                        case RejPkt(int reason):
                            if (reason == RejPkt.ACCOUNT_ALREADY_EXISTS) {
                                this.drawErrorAndWait("An account with that username already exists.", StateMachine.Disconnected);
                            } else {
                                input.close();
                                System.err.println("[ERR] In '" + this.state + "', got 'RejPkt' of reason #" + reason + ".");
                            }
                            break;

                        default:
                            input.close();
                            System.err.println("[ERR] In '" + this.state + "', got '" + pkt + "'.");
                    }
                    break;

                case OutsideCh:
                    switch (pkt) {
                        case ChannelListPkt(List<String> ch):
                            this.rooms = ch;
                            break;

                        default:
                            input.close();
                            System.err.println("[ERR] In '" + this.state + "', got '" + pkt + "'.");
                    }
                    break;

                case CreatingCh:
                    switch (pkt) {
                        case ChannelListPkt(List<String> ch):
                            this.rooms = ch;
                            break;

                        case AckPkt():
                            assert this.currentRoom.isPresent();
                            this.state = StateMachine.InsideCh;
                            break;

                        case RejPkt(int reason):
                            assert this.currentRoom.isPresent();
                            if (reason == RejPkt.CHANNEL_ALREADY_EXISTS) {
                                this.drawErrorAndWait(
                                    "A room called '" + this.currentRoom.get() + "' already exists.", 
                                    StateMachine.OutsideCh
                                );
                            } else {
                                input.close();
                                System.err.println("[ERR] In '" + this.state + "', got 'RejPkt' of reason #" + reason + ".");
                            }
                            break;

                        default:
                            input.close();
                            System.err.println("[ERR] In '" + this.state + "', got '" + pkt + "'.");
                    }
                    break;

                case JoiningCh:
                    switch (pkt) {
                        case ChannelListPkt(List<String> ch):
                            this.rooms = ch;
                            break;

                        case AckPkt():
                            assert this.currentRoom.isPresent();
                            this.state = StateMachine.InsideCh;
                            break;

                        case RejPkt(int reason):
                            assert this.currentRoom.isPresent();
                            if (reason == RejPkt.CHANNEL_NOT_EXISTS) {
                                this.drawErrorAndWait(
                                    "A room called '" + this.currentRoom.get() + "' does not exist.", 
                                    StateMachine.OutsideCh
                                );
                            } else {
                                input.close();
                                System.err.println("[ERR] In '" + this.state + "', got 'RejPkt' of reason #" + reason + ".");
                            }
                            break;

                        default:
                            input.close();
                            System.err.println("[ERR] In '" + this.state + "', got '" + pkt + "'.");
                    }
                    break;

                case InsideCh:
                    switch (pkt) {
                        case ChannelListPkt(List<String> ch):
                            this.rooms = ch;
                            break;

                        case NewMessagePkt(Message m):
                            if (this.messages.size() == MAX_MSG) this.messages.pop();
                            this.messages.addLast(m);
                            break;

                        case RejPkt(int reason):
                            if (reason == RejPkt.OUTSIDE_CHANNEL) {
                                this.state = StateMachine.OutsideCh;
                                break;
                            }
                            // vv Fallthrough vv
                        default:
                            input.close();
                            System.err.println("[ERR] In '" + this.state + "', got '" + pkt + "'.");
                    }
                    break;

                case SendingMsg:
                    switch (pkt) {
                        case ChannelListPkt(List<String> ch):
                            this.rooms = ch;
                            break;

                        case NewMessagePkt(Message m):
                            if (this.messages.size() == MAX_MSG) this.messages.pop();
                            this.messages.addLast(m);
                            break;

                        case AckPkt():
                            assert pendingMsm.isPresent();
                            if (this.messages.size() == MAX_MSG) this.messages.pop();
                            this.messages.addLast(new Message("You", pendingMsm.get()));
                            pendingMsm = Optional.empty();
                            this.state = StateMachine.InsideCh;
                            break;

                        case RejPkt(int reason):
                            if (reason == RejPkt.OUTSIDE_CHANNEL) {
                                this.state = StateMachine.OutsideCh;
                                break;
                            }
                            // vv Fallthrough vv
                        default:
                            input.close();
                            System.err.println("[ERR] In '" + this.state + "', got '" + pkt + "'.");
                    }
                    break;

                case ExitingCh:
                    switch (pkt) {
                        case ChannelListPkt(List<String> ch):
                            this.rooms = ch;
                            break;

                        case NewMessagePkt(Message m):
                            this.messages.add(m);
                            break;

                        case AckPkt():
                            this.currentRoom = Optional.empty();
                            this.messages = new ArrayDeque<>(MAX_MSG);
                            this.state = StateMachine.OutsideCh;
                            assert this.cookie.isPresent();
                            (new GetChsPkt(this.cookie.get())).to(this.output);
                            break;

                        default:
                            input.close();
                            System.err.println("[ERR] In '" + this.state + "', got '" + pkt + "'.");
                    }
                    break;

                default:
                    throw new IllegalArgumentException("Unexpected value: " + this.state);
            }

            System.out.println("[INFO] New state: '" + this.state + "'.");
            this.draw();
            this.gui.updateScreen();
        }
    }

    public void draw() throws IOException {
        // Clear screen
        Window w = this.gui.getActiveWindow();
        if (w != null) this.gui.removeWindow(w);

        // Add window based in the state
        switch (this.state) {
            case Disconnected -> { w = drawAuth(); }
            case Connecting -> { w = drawLabel("Connecting to the server..."); }
            case Registering -> { w = drawLabel("Registering a new account..."); }
            case OutsideCh -> { w = drawList(); }
            case JoiningCh -> { w = drawLabel("Joining the room '" + this.currentRoom.get() + "'..."); }
            case CreatingCh -> { w = drawLabel("Creating the room '" + this.currentRoom.get() + "'..."); }
            case InsideCh -> { w = drawRoom(); }
            case SendingMsg -> { w = drawLabel("Sending a message to the server..."); }
            case ExitingCh -> { w = drawLabel("Leaving the room..."); }
            default -> throw new IllegalArgumentException("Unexpected value: " + this.state);
        }
        w.setHints(Arrays.asList(Window.Hint.FULL_SCREEN));
        this.gui.addWindow(w);
        this.gui.setActiveWindow(w);
    }

    private BasicWindow drawAuth() throws IOException {
        Panel panel = new Panel();
        panel.setLayoutManager(new GridLayout(2));

        panel.addComponent(new Label("Username: "));
        TextBox usernameField = new TextBox();
        panel.addComponent(usernameField);
        panel.addComponent(new Label("Password: "));
        TextBox passwordField = new TextBox();
        panel.addComponent(passwordField);

        Button loginButton = new Button("Login");
        loginButton.addListener((b) -> {
            // Send packet
            String username = usernameField.getText();
            String password = passwordField.getText();
            try {
                password = Hasher.hashToHex(password);
            } catch (NoSuchAlgorithmException e) {
                // Auto-generated catch block
                e.printStackTrace();
            }
            Auth credentials = new Credentials(username, password);
            ConnectPkt request = new ConnectPkt(credentials);
            try {
                if (this.conn.isClosed()) throw new IOException("Connection is closed!");
                request.to(this.output);
                this.state = StateMachine.Connecting;
                this.draw();
            } catch (IOException e) {
                try {
                    this.output.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                drawErrorAndWait("Failure when sending ConnectPkt to the server.", StateMachine.Disconnected);
            }
        });
        panel.addComponent(loginButton);

        Button registerButton = new Button("Register");
        registerButton.addListener((b) -> {
            String username = usernameField.getText();
            String password = passwordField.getText();
            try {
                password = Hasher.hashToHex(password);
            } catch (NoSuchAlgorithmException e) {
                // Auto-generated catch block
                e.printStackTrace();
            }
            Credentials credentials = new Credentials(username, password);
            RegisterPkt request = new RegisterPkt(credentials);
            try {
                if (this.conn.isClosed()) throw new IOException("Connection is closed!");
                request.to(this.output);
                this.state = StateMachine.Registering;
                this.draw();
            } catch (IOException e) {
                try {
                    this.output.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                drawErrorAndWait("Failure when sending RegisterPkt to the server.", StateMachine.Disconnected);
            }
        });
        panel.addComponent(registerButton);

        // Create window to hold the panel
        BasicWindow window = new BasicWindow();
        window.setComponent(panel);
        return window;
    }

    private BasicWindow drawList() throws IOException {
        // Select an existing room
        Panel joinPanel = new Panel();
        joinPanel.setLayoutManager(new GridLayout(1));
        if (rooms.isEmpty()) {
            joinPanel.addComponent(new Label("Waiting for the server to send the rooms..."));
        } else {
            joinPanel.addComponent(new Label("Select an existing room: "));
            for (String roomName : rooms) {
                Button button = new Button(roomName);
                button.addListener((b) -> {
                    String roomLabel = b.getLabel();
                    assert cookie.isPresent();
                    JoinChPkt request = new JoinChPkt(roomLabel, cookie.get());
                    try {
                        if (this.conn.isClosed()) throw new IOException("Connection is closed!");
                        request.to(this.output);
                        this.currentRoom = Optional.of(roomLabel);
                        this.state = StateMachine.JoiningCh;
                        this.draw();
                    } catch (IOException e) {
                        try {
                            this.output.close();
                        } catch (IOException e1) {
                            e1.printStackTrace();
                        }
                        drawErrorAndWait("Failure when sending JoinChPkt to the server.", StateMachine.OutsideCh);
                    }
                });
                joinPanel.addComponent(button);
            }
        }

        // Create a new room
        Panel createPanel = new Panel();
        createPanel.setLayoutManager(new GridLayout(2));
        createPanel.addComponent(new Label("Create a new room: "));
        createPanel.addComponent(new EmptySpace(new TerminalSize(0, 0)));
        createPanel.addComponent(new Label("Room name: "));
        createPanel.addComponent(this.components.get("roomText"));
        createPanel.addComponent(new Label("AI support: "));
        createPanel.addComponent(this.components.get("aiCheck"));
        createPanel.addComponent(new Label("AI prompt: "));
        createPanel.addComponent(this.components.get("aiText"));
        createPanel.addComponent(new EmptySpace(new TerminalSize(0, 0)));
        Button button = new Button("Create");
        button.addListener((b) -> {
            String roomLabel = switch (this.components.get("roomText")) {
                case TextBox c -> c.getText();
                default -> throw Panic.unreachable();
            };
            Boolean aiLabel = switch (this.components.get("aiCheck")) {
                case CheckBox c -> c.isChecked();
                default -> throw Panic.unreachable();
            };
            String aiPrompt = switch (this.components.get("aiText")) {
                case TextBox c -> c.getText();
                default -> throw Panic.unreachable();
            };
            Optional<String> prompt = aiLabel ? Optional.of(aiPrompt) : Optional.empty();
            Channel channel = new Channel(roomLabel, prompt);
            assert cookie.isPresent();
            CreateChPkt request = new CreateChPkt(channel, cookie.get());
            try {
                if (this.conn.isClosed()) throw new IOException("Connection is closed!");
                request.to(this.output);
                this.currentRoom = Optional.of(roomLabel);
                this.state = StateMachine.CreatingCh;
                this.draw();
            } catch (IOException e) {
                try {
                    this.output.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                drawErrorAndWait("Failure when sending CreateChPkt to the server.", StateMachine.OutsideCh);
            }
        });
        createPanel.addComponent(button);

        // Main Panel
        Panel mainPanel = new Panel();
        Button refreshButton = new Button("Refresh");
        refreshButton.addListener((b) -> {
            assert cookie.isPresent();
            GetChsPkt request = new GetChsPkt(cookie.get());
            try {
                if (this.conn.isClosed()) throw new IOException("Connection is closed!");
                request.to(this.output);
                this.draw();
            } catch (IOException e) {
                try {
                    this.output.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                drawErrorAndWait("Failure when sending CreateChPkt to the server.", StateMachine.OutsideCh);
        }});
        Button closeButton = new Button("Exit");
        closeButton.addListener((b) -> {
            try {
                this.conn.close();
            } catch (IOException e) {
                // Do nothing
            }
            System.exit(0);
        });
        mainPanel.addComponent(closeButton);
        mainPanel.addComponent(refreshButton);
        mainPanel.addComponent(joinPanel);
        mainPanel.addComponent(new EmptySpace(new TerminalSize(0, 1)));
        mainPanel.addComponent(createPanel);

        BasicWindow window = new BasicWindow();
        window.setComponent(mainPanel);
        return window;
    }

    private BasicWindow drawRoom() throws IOException {
        // User input panel
        Panel inputPanel = new Panel();
        inputPanel.setPreferredSize(new TerminalSize(999, 1));
        inputPanel.setLayoutManager(new LinearLayout(Direction.HORIZONTAL));
        inputPanel.addComponent(new Label(currentRoom.get()));
        inputPanel.addComponent(this.components.get("msgText"));
        Button submitButton = new Button("Send");
        submitButton.addListener((b) -> {
            String msg = switch (this.components.get("msgText")) {
                case TextBox c -> c.getText();
                default -> throw Panic.unreachable();
            };
            assert cookie.isPresent();
            MessagePkt request = new MessagePkt(msg, cookie.get());
            try {
                if (this.conn.isClosed()) throw new IOException("Connection is closed!");
                request.to(this.output);
                this.pendingMsm = Optional.of(msg);
                this.state = StateMachine.SendingMsg;
                switch (this.components.get("msgText")) {
                    case TextBox c -> c.setText("");
                    default -> throw Panic.unreachable();
                };
                this.draw();
            } catch (IOException e) {
                try {
                    this.output.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                drawErrorAndWait("Failure when sending MessagePkt to the server.", StateMachine.InsideCh);
            }
        });
        inputPanel.addComponent(submitButton);
        Button exitButton = new Button("Exit");
        exitButton.addListener((b) -> {
            assert cookie.isPresent();
            ExitChPkt request = new ExitChPkt(cookie.get());
            try {
                if (this.conn.isClosed()) throw new IOException("Connection is closed!");
                request.to(this.output);
                this.state = StateMachine.ExitingCh;
                this.draw();
            } catch (IOException e) {
                try {
                    this.output.close();
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                drawErrorAndWait("Failure when sending ExitChPkt to the server.", StateMachine.InsideCh);
            }
        });
        inputPanel.addComponent(exitButton);

        // Message Panel
        Panel msgPanel = new Panel();
        msgPanel.setLayoutManager(new LinearLayout(Direction.VERTICAL));
        int height = this.gui.getScreen().getTerminalSize().getRows() - 3;
        ArrayList<String> toDraw = new ArrayList<>();
        int line_count = 0;
        for (Iterator<Message> it = messages.descendingIterator(); it.hasNext(); ) {
            Message m = it.next();
            String mString = "[" +
                    m.author() +
                    "]: " +
                    m.msg();
            String sFit = fitText(mString);
            line_count += (int) sFit.chars().filter(ch -> ch == '\n').count() + 1;
            if (line_count <= height) {
                toDraw.add(sFit);
            } else {
                break;
            }
        }
        for (String msg : toDraw.reversed()) {
            msgPanel.addComponent(new Label(msg));
            //msgPanel.addComponent(new EmptySpace(new TerminalSize(0, 1)));
        }

        // Main Panel
        Panel mainPanel = new Panel();
        mainPanel.addComponent(inputPanel);
        mainPanel.addComponent(msgPanel);

        BasicWindow window = new BasicWindow();
        window.setComponent(mainPanel);
        return window;
    }

    private void drawErrorAndWait(String message, StateMachine nextState) {
        Panel panel = new Panel();
        panel.addComponent(new Label(message));
        Button backButton = new Button("Ok");
        backButton.addListener((b) -> {
            this.state = nextState;

            // Workaround for draw() throwing an IOException:
            // https://www.baeldung.com/java-lambda-exceptions#1-throwing-checked-exception-from-lambda-expressions
            try {
                draw();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
        panel.addComponent(backButton);

        BasicWindow window = new BasicWindow();
        window.setComponent(panel);
        this.gui.removeWindow(this.gui.getActiveWindow());
        this.gui.addWindow(window);
        window.waitUntilClosed();
    }

    private BasicWindow drawLabel(String message) {
        Panel panel = new Panel();
        panel.addComponent(new Label("Please wait: "));
        panel.addComponent(new Label(message));

        BasicWindow window = new BasicWindow();
        window.setComponent(panel);
        return window;
    }

    private String fitText(String text) {
        String textCopy = new String(text);
        int width = this.gui.getScreen().getTerminalSize().getColumns() - 3;
        return Arrays.stream(textCopy.split("\\r?\\n"))
                .map(original -> {
                    if (original.length() > width) {
                        ArrayList<String> lines = new ArrayList<>();
                        StringBuilder curr_line = new StringBuilder();
                        Arrays.stream(original.split("\\s+")).forEach(word -> {
                            if (curr_line.length() + word.length() + 1 > width) {
                                lines.add(curr_line.toString());
                                curr_line.setLength(0);
                                curr_line.append(word);
                            } else {
                                curr_line.append(" ");
                                curr_line.append(word);
                            }
                        });
                        lines.add(curr_line.toString());
                        return String.join("\n", lines);
                    } else return original;
                }).collect(Collectors.joining("\n"));
    }
}
