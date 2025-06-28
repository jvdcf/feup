package org.cpd2425.t09g13;

import java.io.IOException;
import java.io.InvalidObjectException;
import java.net.Socket;
import java.util.List;
import java.util.Optional;

import org.cpd2425.t09g13.overseer.*;
import org.cpd2425.t09g13.overseer.ReplyError.ErrorKind;
import org.cpd2425.t09g13.protocol.Channel;
import org.cpd2425.t09g13.protocol.Message;
import org.cpd2425.t09g13.protocol.auth.Auth;
import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.cpd2425.t09g13.protocol.auth.Credentials;
import org.cpd2425.t09g13.protocol.client.ClientPkt;
import org.cpd2425.t09g13.protocol.client.ConnectPkt;
import org.cpd2425.t09g13.protocol.client.CreateChPkt;
import org.cpd2425.t09g13.protocol.client.ExitChPkt;
import org.cpd2425.t09g13.protocol.client.GetChsPkt;
import org.cpd2425.t09g13.protocol.client.JoinChPkt;
import org.cpd2425.t09g13.protocol.client.MessagePkt;
import org.cpd2425.t09g13.protocol.client.RegisterPkt;
import org.cpd2425.t09g13.protocol.server.AckPkt;
import org.cpd2425.t09g13.protocol.server.ChannelListPkt;
import org.cpd2425.t09g13.protocol.server.ConnectAckPkt;
import org.cpd2425.t09g13.protocol.server.NewMessagePkt;
import org.cpd2425.t09g13.protocol.server.RejPkt;
import org.cpd2425.t09g13.room.DataMsg;
import org.cpd2425.t09g13.room.RoomMsg;
import org.cpd2425.t09g13.server.ErrorMsg;
import org.cpd2425.t09g13.server.ErrorMsg.Kind;
import org.cpd2425.t09g13.server.ExitMsg;
import org.cpd2425.t09g13.server.InternalMsg;
import org.cpd2425.t09g13.server.NewMsg;
import org.cpd2425.t09g13.server.RoomListMsg;
import org.cpd2425.t09g13.server.SendAckMsg;
import org.cpd2425.t09g13.server.SendCookieMsg;
import org.cpd2425.t09g13.server.WelcomeMsg;
import org.cpd2425.t09g13.sync.Cell;
import org.cpd2425.t09g13.sync.channel.InvalidReceiverException;
import org.cpd2425.t09g13.sync.channel.Receiver;
import org.cpd2425.t09g13.sync.channel.Sender;
import org.cpd2425.t09g13.utils.Pair;
import org.cpd2425.t09g13.utils.Panic;
import org.msgpack.core.MessagePack;
import org.msgpack.core.MessagePacker;
import org.msgpack.core.MessageUnpacker;

/**
 * ServerWorker
 */
public class ServerWorker {
  private enum Stage {
    Authentication,
    Conversation,
  };

  private record Room(String name, Sender<RoomMsg> msgTrcv, Receiver<RoomMsg> msgRcvr) {
  }

  private Stage currentStage = Stage.Authentication;
  private final Sender<OverSeerRequest> overSeer;
  private final Receiver<InternalMsg> recv;
  private final Sender<InternalMsg> trcv;
  private final MessagePacker output;
  private final MessageUnpacker input;
  private Optional<Room> room = Optional.empty();
  private Optional<Thread> roomThread = Optional.empty();
  private String userName = "unnamed";
  private Cell<Integer> timeOut = new Cell<>(50);
  private boolean closed = false;

  public ServerWorker(Sender<OverSeerRequest> overSeer, Socket sock) throws IOException {
    this.overSeer = overSeer;
    this.trcv = new Sender<>(20);
    this.recv = this.trcv.newReceiver();
    this.input = MessagePack.newDefaultUnpacker(sock.getInputStream());
    this.output = MessagePack.newDefaultPacker(sock.getOutputStream());
  }

  // RECV: ServerMsg, SEND: ServerPkt
  private void sending_half() {
    try {
      Optional<InternalMsg> msg = Optional.empty();
      while (true) {
        if (this.recv.isInvalid() || this.closed) {
          System.out.println("Client '" + this.userName + "' being dropped because it is too slow.");
          this.output.close();
          return;
        }
        this.recv.waitForMsg(this.timeOut.get());
        this.timeOut.set(0);
        msg = this.recv.receive();
        while (msg.isPresent()) {
          this.output.flush();
          InternalMsg msgC = msg.get();
          msg = this.recv.receive();
          l: switch (msgC) {
            // TODO: Evaluate if needed (maybe redundant?)
            case ExitMsg(String username) -> {
              if (!username.equals(this.userName)) {
                Message exitMsg = new Message("Server", "Client '" + username + "' disconnected.");
                new NewMessagePkt(exitMsg).to(this.output);
              } else {
                new AckPkt().to(this.output);
              }
            }
            case WelcomeMsg(Integer id) -> {
            } // TODO: Revisit this
            case ErrorMsg(Kind k) -> {
              switch (k) {
                case AuthFailure -> RejPkt.InvalidCredentials().to(this.output);
                case InvalidRoom -> RejPkt.ChannelNotExists().to(this.output);
                case DuplicatedSession -> RejPkt.AlreadyConnected().to(this.output);
                case AccountAlreadyExists -> RejPkt.AccountAlreadyExists().to(this.output);
                case RoomAlreadyExists -> RejPkt.ChannelAlreadyExists().to(this.output);
                case OutsideRoom -> RejPkt.OutsideChannel().to(this.output);
              }
            }
            case SendAckMsg() -> new AckPkt().to(this.output);
            case SendCookieMsg(Cookie c) -> new ConnectAckPkt(c).to(this.output);
            case RoomListMsg(List<String> r) -> new ChannelListPkt(r).to(this.output);
            case NewMsg(Message m) -> {
              if (this.room.isEmpty()) break l;
              if (m.author().equals(this.userName))
                break l;
              new NewMessagePkt(m).to(this.output);
            }
          }
        }
      }
    } catch (Exception e) {
      System.out.println("Client '" + this.userName + "' disconnected with error: " + e);
      return;
    }
  }

  private OverSeerReply waitForOverseer(Receiver<OverSeerReply> listener) {
    while (true) {
      listener.waitForMsg(100);
      try {
        Optional<OverSeerReply> msg = listener.receive();
        if (msg.isPresent()) {
          return msg.get();
        }
      } catch (Exception e) {
        throw Panic.unreachable();
      }
    }
  }

  private InternalMsg handle_login(Auth auth) {
    return switch (auth) {
      case Cookie(long id) -> {
        // INFO: Reconnect request
        Sender<OverSeerReply> replyChan = new Sender<>(1);
        Receiver<OverSeerReply> replyListener = replyChan.newReceiver();
        this.overSeer.send(new RequestReconnect(new Cookie(id), replyChan));
        yield switch (this.waitForOverseer(replyListener)) {
          case ReplyAuthorized(Optional<String> roomName, Cookie cookie) -> {
            if (roomName.isPresent()) {
              // TODO: HANDLE THIS!!!!!!!
              this.handle_chg_room(roomName.get(), cookie);
            }
            this.currentStage = Stage.Conversation;
            this.overSeer.send(new RequestUsernameFromCookie(cookie, replyChan));
            switch (this.waitForOverseer(replyListener)) {
              case ReplyError(ErrorKind e) -> {
                switch (e) {
                  case InvalidCredentials -> this.trcv.send(ErrorMsg.authFailure());
                  default -> throw Panic.unreachable();
                }
              }
              case ReplyAuthDetails(String username, Cookie cook) -> this.userName = username;
              default -> throw Panic.unreachable();
            }
            yield new SendAckMsg();
          }
          case ReplyError(ErrorKind e) -> {
            yield switch (e) {
              case AlreadyLoggedIn -> ErrorMsg.duplicatedSession();
              case InvalidCredentials -> ErrorMsg.authFailure();
              default -> throw Panic.unreachable();
            };
          }
          default -> throw Panic.unreachable();
        };
      }
      case Credentials(String username, String passHash) -> {
        // INFO: Login request
        Sender<OverSeerReply> replyChan = new Sender<>(1);
        Receiver<OverSeerReply> replyListener = replyChan.newReceiver();
        this.overSeer.send(new RequestLogin(username, passHash, replyChan));
        yield switch (this.waitForOverseer(replyListener)) {
          case ReplyAuthorized(Optional<String> roomName, Cookie cookie) -> {
            this.currentStage = Stage.Conversation;
            this.userName = username;
            yield new SendCookieMsg(cookie);
          }
          case ReplyError(ErrorKind e) -> {
            yield switch (e) {
              case AlreadyLoggedIn -> ErrorMsg.duplicatedSession();
              case InvalidCredentials -> ErrorMsg.authFailure();
              default -> throw Panic.unreachable();
            };
          }
          default -> throw Panic.unreachable();
        };
      }
    };
  }

  private InternalMsg handle_register(Credentials credentials) {
    Sender<OverSeerReply> replyChan = new Sender<>(1);
    Receiver<OverSeerReply> replyListener = replyChan.newReceiver();
    this.overSeer.send(new RequestRegister(credentials.username(), credentials.passHash(), replyChan));
    return switch (this.waitForOverseer(replyListener)) {
      case ReplyAuthorized(Optional<String> _roomName, Cookie c) -> {
        this.currentStage = Stage.Conversation;
        this.userName = credentials.username();
        yield new SendCookieMsg(c);
      }
      case ReplyError(ErrorKind e) -> {
        yield switch (e) {
          case AlreadyExists -> ErrorMsg.accountAlreadyExists();
          default -> throw Panic.unreachable();
        };
      }
      default -> throw Panic.unreachable();
    };
  }

  private InternalMsg handle_chan_create(Channel chan, Cookie cookie) {
    Sender<OverSeerReply> replyChan = new Sender<>(1);
    Receiver<OverSeerReply> replyListener = replyChan.newReceiver();
    this.overSeer.send(new RequestNewRoom(chan, replyChan));
    return switch (this.waitForOverseer(replyListener)) {
      case ReplyRoomChange(Sender<RoomMsg> roomSender) -> {
        this.room = Optional.of(new Room(chan.name(), roomSender, roomSender.newReceiver()));
        this.roomThread = Optional.of(Thread.ofVirtual().start(() -> this.room_daemon()));
        yield new SendAckMsg();
      }
      case ReplyError(ErrorKind e) -> {
        yield switch (e) {
          case ErrorKind.AlreadyExists -> ErrorMsg.roomAlreadyExists();
          default -> throw Panic.unreachable();
        };
      }
      default -> throw Panic.unreachable();
    };
  }

  private InternalMsg handle_get_chs(Cookie cookie) {
    Sender<OverSeerReply> replyChan = new Sender<>(1);
    Receiver<OverSeerReply> replyListener = replyChan.newReceiver();
    this.overSeer.send(new RequestRoomList(replyChan));
    return switch (this.waitForOverseer(replyListener)) {
      case ReplyRoomList(List<String> roomNames) -> new RoomListMsg(roomNames);
      default -> throw Panic.unreachable();
    };
  }

  private InternalMsg handle_message(String msg, Cookie cookie) {
    if (this.room.isEmpty())
      return ErrorMsg.outsideRoom();
    Message msgWrapper = new Message(this.userName, msg);
    this.room.get().msgTrcv().send(new DataMsg(msgWrapper));
    return new SendAckMsg();
  }

  private InternalMsg handle_exit_ch(Cookie cookie) {
    if (this.roomThread.isEmpty())
      throw Panic.unreachable();
    Room oldRoom = this.room.get();
    this.room = Optional.empty();
    this.roomThread.get().interrupt();
    oldRoom.msgTrcv()
        .send(new DataMsg(new Message("Server", "Client '" + this.userName + "' disconnected.")));
    return new SendAckMsg();
  }

  private InternalMsg handle_chg_room(String chan, Cookie cookie) {
    Sender<OverSeerReply> replyChan = new Sender<>(1);
    Receiver<OverSeerReply> replyListener = replyChan.newReceiver();
    this.overSeer.send(new RequestRoomChange(chan, new Pair<String, Cookie>(this.userName, cookie), replyChan));
    return switch (this.waitForOverseer(replyListener)) {
      case ReplyRoomChange(Sender<RoomMsg> room) -> {
        this.room = Optional.of(new Room(chan, room, room.newReceiver()));
        this.roomThread = Optional.of(Thread.ofVirtual().start(() -> this.room_daemon()));
        room.send(new DataMsg(new Message("Server", "Welcome to room '" + chan + "', '" + this.userName + "'!")));
        yield new SendAckMsg();
      }
      case ReplyError(ErrorKind e) -> {
        yield switch (e) {
          case InvalidCredentials -> ErrorMsg.authFailure();
          case NoSuchRoom -> ErrorMsg.invalidRoom();
          default -> throw Panic.unreachable();
        };
      }
      default -> throw Panic.unreachable();
    };
  }

  private InternalMsg handle_convo(ClientPkt packet) {
    return switch (packet) {
      case ConnectPkt(Auth auth) -> this.handle_login(auth);
      case RegisterPkt(Credentials credentials) -> this.handle_register(credentials);
      case CreateChPkt(Channel chan, Cookie cook) -> this.handle_chan_create(chan, cook);
      case GetChsPkt(Cookie cookie) -> handle_get_chs(cookie);
      case JoinChPkt(String chan, Cookie cookie) -> handle_chg_room(chan, cookie);
      case MessagePkt(String msg, Cookie cookie) -> handle_message(msg, cookie);
      case ExitChPkt(Cookie cookie) -> handle_exit_ch(cookie);
    };
  }

  // RECV: ClientPkt, SEND: ServerMsg
  private void receiving_half() {
    try {
      while (true) {
        if (this.closed) {
          return;
        }
        Optional<ClientPkt> maybe_pkt = ClientPkt.from(this.input);
        if (maybe_pkt.isEmpty()) {
          throw new InvalidObjectException("Received Invalid Packet.");
        }
        ClientPkt pkt = maybe_pkt.get();
        InternalMsg msg = this.handle_convo(pkt);
        this.trcv.send(msg);
      }
    } catch (Exception e) {
      System.out.println("Client '" + this.userName + "' disconnecting with error: " + e);
      this.trcv.send(new ExitMsg(this.userName));
      this.closed = true;
      if (this.room.isPresent()) {
        this.overSeer.send(new RequestCookieJar(this.userName, Optional.of(this.room.get().name())));
      } else {
        this.overSeer.send(new RequestCookieJar(this.userName, Optional.empty()));
      }
      return;
    }
  }

  // For receiving room msgs
  private void room_daemon() {
    try {
      Thread.sleep(500);
      while (this.room.isPresent()) {
        Room r = this.room.get();
        r.msgRcvr.waitForMsg(2000);
        Optional<RoomMsg> maybe_msg = r.msgRcvr.receive();
        if (maybe_msg.isEmpty())
          continue;
        RoomMsg msg = maybe_msg.get();
        switch (msg) {
          case DataMsg(Message m) -> this.trcv.send(new NewMsg(m));
          default -> throw Panic.unreachable();
        }
      }
    } catch (InvalidReceiverException e) {
      this.room = Optional.empty();
      this.closed = true;
      return;
    } catch (InterruptedException e) {
      return;
    }
  }

  public void start() {
    Thread.ofVirtual().start(() -> this.receiving_half());
    Thread.ofVirtual().start(() -> this.sending_half());
  }
}
