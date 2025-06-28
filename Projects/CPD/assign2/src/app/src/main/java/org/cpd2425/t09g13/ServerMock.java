package org.cpd2425.t09g13;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedList;
import java.util.List;
import java.util.Optional;

import org.cpd2425.t09g13.protocol.Channel;
import org.cpd2425.t09g13.protocol.Message;
import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.cpd2425.t09g13.protocol.client.ClientPkt;
import org.cpd2425.t09g13.protocol.server.AckPkt;
import org.cpd2425.t09g13.protocol.server.ChannelListPkt;
import org.cpd2425.t09g13.protocol.server.ConnectAckPkt;
import org.cpd2425.t09g13.protocol.server.NewMessagePkt;
import org.cpd2425.t09g13.protocol.server.RejPkt;
import org.msgpack.core.MessagePack;
import org.msgpack.core.MessagePacker;
import org.msgpack.core.MessageUnpacker;

import com.googlecode.lanterna.TextColor;
import com.googlecode.lanterna.gui2.BasicWindow;
import com.googlecode.lanterna.gui2.Button;
import com.googlecode.lanterna.gui2.DefaultWindowManager;
import com.googlecode.lanterna.gui2.EmptySpace;
import com.googlecode.lanterna.gui2.Label;
import com.googlecode.lanterna.gui2.MultiWindowTextGUI;
import com.googlecode.lanterna.gui2.Panel;
import com.googlecode.lanterna.gui2.TextBox;
import com.googlecode.lanterna.screen.Screen;
import com.googlecode.lanterna.screen.TerminalScreen;
import com.googlecode.lanterna.terminal.DefaultTerminalFactory;
import com.googlecode.lanterna.terminal.Terminal;

public class ServerMock {
    private ServerSocket sSock;

    public ServerMock(Integer port) throws IOException {
        this.sSock = new ServerSocket(port);
    }

    public void run() throws IOException {
        System.out.println("Waiting for a connection...");
        Socket sock = this.sSock.accept();
        System.out.println("Connection accepted from " + sock.getInetAddress());
        MessagePacker output = MessagePack.newDefaultPacker(sock.getOutputStream());

        Terminal terminal = new DefaultTerminalFactory().createTerminal();
        Screen screen = new TerminalScreen(terminal);
        screen.startScreen();
        MultiWindowTextGUI gui = new MultiWindowTextGUI(screen, new DefaultWindowManager(),
                new EmptySpace(TextColor.ANSI.BLUE));
        gui.setBlockingIO(false);

        Panel panel = new Panel();
        panel.addComponent(new Label("Last message received: "));
        Label rcvLabel = new Label("<nothing yet>");
        panel.addComponent(rcvLabel);

        panel.addComponent(new Label("Send actions: "));
        Button ack = new Button("AckPkt");
        ack.addListener((b) -> {
            try {
                new AckPkt().to(output);
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        });
        panel.addComponent(ack);

        TextBox rejCode = new TextBox("0");
        panel.addComponent(rejCode);
        Button rej = new Button("RejPkt");
        rej.addListener((b) -> {
            Integer code = Integer.parseInt(rejCode.getText());
            try {
                new RejPkt(code).to(output);
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        });
        panel.addComponent(rej);

        Button connectAck = new Button("ConnectAckPkt");
        connectAck.addListener((b) -> {
            try {
                new ConnectAckPkt(new Cookie(1234567890)).to(output);
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        });
        panel.addComponent(connectAck);

        Button chList = new Button("ChannelListPkt");
        chList.addListener((b) -> {
            List<Channel> ch = new LinkedList<>();
            ch.add(new Channel("Channel 1", Optional.of("Mention Waddler always!")));
            ch.add(new Channel("Channel 2", Optional.empty()));
            List<String> rooms = ch.stream().map((c) -> c.name()).toList();
            try {
                new ChannelListPkt(rooms).to(output);
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        });
        panel.addComponent(chList);

        Button newMsg = new Button("NewMessagePkt");
        newMsg.addListener((b) -> {
            Message m = new Message("Mock","An example message...");
            try {
                new NewMessagePkt(m).to(output);
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        });
        panel.addComponent(newMsg);

        BasicWindow window = new BasicWindow();
        window.setComponent(panel);
        gui.addWindow(window);

        Thread.ofVirtual().start(() -> {
            try (MessageUnpacker input = MessagePack.newDefaultUnpacker(sock.getInputStream())) {
                while (true) {
                    Optional<ClientPkt> c = ClientPkt.from(input); 
                    rcvLabel.setText(c.toString());
                }
            } catch (Exception e) {
                System.out.println("Exception: " + e);
            }
        });

        window.waitUntilClosed();
    }
}
