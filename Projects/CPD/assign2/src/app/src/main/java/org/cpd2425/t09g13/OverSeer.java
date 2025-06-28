package org.cpd2425.t09g13;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Random;
import java.time.Instant;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;

import org.cpd2425.t09g13.overseer.OverSeerReply;
import org.cpd2425.t09g13.overseer.OverSeerRequest;
import org.cpd2425.t09g13.overseer.ReplyAuthDetails;
import org.cpd2425.t09g13.overseer.ReplyAuthorized;
import org.cpd2425.t09g13.overseer.ReplyError;
import org.cpd2425.t09g13.overseer.ReplyRoomChange;
import org.cpd2425.t09g13.overseer.ReplyRoomList;
import org.cpd2425.t09g13.overseer.RequestCookieJar;
import org.cpd2425.t09g13.overseer.RequestLogin;
import org.cpd2425.t09g13.overseer.RequestNewRoom;
import org.cpd2425.t09g13.overseer.RequestReconnect;
import org.cpd2425.t09g13.overseer.RequestRegister;
import org.cpd2425.t09g13.overseer.RequestRoomChange;
import org.cpd2425.t09g13.overseer.RequestRoomList;
import org.cpd2425.t09g13.overseer.RequestUsernameFromCookie;
import org.cpd2425.t09g13.overseer.ReplyError.ErrorKind;
import org.cpd2425.t09g13.protocol.Channel;
import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.cpd2425.t09g13.room.RoomMsg;
import org.cpd2425.t09g13.sync.channel.Receiver;
import org.cpd2425.t09g13.sync.channel.Sender;
import org.cpd2425.t09g13.utils.CsvReader;
import org.cpd2425.t09g13.utils.Hasher;
import org.cpd2425.t09g13.utils.Pair;


/**
 * OverSeer
 */
public class OverSeer {
  private record CookieJarEntry(String user, Cookie cookie, Instant inst, Optional<String> roomName) {}
  private final List<CookieJarEntry> cookieJar = new ArrayList<>();
  public record RoomEntry(Channel channel, Sender<RoomMsg> sender, Optional<Ollama> ollama) {}
  private final List<RoomEntry> rooms;
  private final List<Pair<String, Cookie>> connectedUsers;
  private final CsvReader authFile;
  private final Receiver<OverSeerRequest> reqs;
  private final Sender<OverSeerRequest> __reqSender;
  private final Random rng = new Random();
  private final Integer SENDER_BUFFER = 50;

  public OverSeer(List<RoomEntry> initRooms, String pathname) throws FileNotFoundException, IOException {
    this.rooms = initRooms;
    String[] fields = {"username", "hash", "salt"};
    this.authFile = new CsvReader(fields, new RandomAccessFile(new File(pathname), "rw"));
    this.connectedUsers = new ArrayList<>();
    this.__reqSender = new Sender<>(1000);
    this.reqs = this.__reqSender.newReceiver();
  }

  private Cookie nextCookie() {
    return this.nextCookie(0);
  }

  private Cookie nextCookie(long rest) {
    long value = this.rng.nextLong() + rest;
    Cookie cookie = new Cookie(value);
    for (CookieJarEntry c : cookieJar) {
      if (cookie.equals(c.cookie())) return this.nextCookie(value);
    }
    for (Pair<String, Cookie> c : connectedUsers) {
      if (cookie.equals(c.right())) return this.nextCookie(value);
    }
    this.rng.setSeed(value ^ rest);
    return cookie;
  }

  public Sender<OverSeerRequest> run() {
    Thread.ofVirtual().start(() -> {
      Ollama.configureModel();
      try {
        while (true) {
          this.reqs.waitForMsg();
          Optional<OverSeerRequest> maybe_msg = this.reqs.receive(); 
          do {
            OverSeerRequest msg = maybe_msg.get();
            s: switch (msg) {
              case RequestLogin(String user, String passHash, Sender<OverSeerReply> replyChan) -> {
                for (Pair<String, Cookie> u : connectedUsers) {
                  if (u.left().equals(user)) {
                    replyChan.send(new ReplyError(ErrorKind.AlreadyLoggedIn));
                    break s;
                  }
                }
                List<Map<String,String>> matches = this.authFile.lookup("username", user);
                if (matches.isEmpty()) {
                  replyChan.send(new ReplyError(ReplyError.ErrorKind.InvalidCredentials));
                  break s;
                }
                for (Map<String,String> userEntry : matches) {
                  if (userEntry.get("username").equals(user)) {
                    String salt = userEntry.get("salt");
                    String composed = passHash + salt;
                    String innerHash = Hasher.hashToHex(composed);
                    if (userEntry.get("hash").equals(innerHash)) {
                      Cookie cookie = this.nextCookie();
                      replyChan.send(new ReplyAuthorized(Optional.empty(), cookie));
                      this.connectedUsers.add(new Pair<>(user, cookie));
                      break s;
                    } else {
                      replyChan.send(new ReplyError(ReplyError.ErrorKind.InvalidCredentials));
                      break s;
                    }
                  }
                }
                replyChan.send(new ReplyError(ReplyError.ErrorKind.InvalidCredentials));
              }
              case RequestRegister(String user, String passHash, Sender<OverSeerReply> replyChan) -> {
                List<Map<String, String>> matches = this.authFile.lookup("username", user);
                for (Map<String,String> m : matches) {
                  if (m.get("username").equals(user)) {
                    replyChan.send(new ReplyError(ErrorKind.AlreadyExists));
                    break s;
                  } 
                }
                String salt = Hasher.hashToHex(String.valueOf(this.rng.nextLong()));
                String composed = passHash + salt;
                String innerHash = Hasher.hashToHex(composed);

                Map<String, String> entry = new HashMap<>();
                entry.put("username", user);
                entry.put("hash", innerHash);
                entry.put("salt", salt);
                this.authFile.insert(entry);
                Cookie cookie = this.nextCookie();
                replyChan.send(new ReplyAuthorized(Optional.empty(), cookie));
                this.connectedUsers.add(new Pair<>(user, cookie));
              }
              case RequestRoomChange(String chanName, Pair<String, Cookie> userPair, Sender<OverSeerReply> replyChan) -> {
                for (RoomEntry entry : rooms) {
                  if (entry.channel().name().equals(chanName)) {
                    for (Pair<String, Cookie> c : this.connectedUsers) {
                      if (c.equals(userPair)) {
                        replyChan.send(new ReplyRoomChange(entry.sender().clone()));
                        break s;
                      }
                    }
                    replyChan.send(new ReplyError(ErrorKind.InvalidCredentials));
                    break s;
                  }
                }
                replyChan.send(new ReplyError(ErrorKind.NoSuchRoom));
              }
              case RequestCookieJar(String user, Optional<String> roomName) -> {
                Optional<Cookie> cookie = this.connectedUsers.stream()
                        .filter(p -> p.left().equals(user))
                        .map(Pair::right)
                        .findFirst();
                if (cookie.isEmpty()) {
                  break s;
                }
                Pair<String, Cookie> entry = new Pair<>(user, cookie.get());
                this.connectedUsers.remove(entry);
                this.cookieJar.add(new CookieJarEntry(user, entry.right(), Instant.now(), roomName));
                this.cookieJar.removeIf(c -> {
                  long dt = c.inst().toEpochMilli() - Instant.now().toEpochMilli();
                  return dt <= 300_000;
                });
              }
              case RequestReconnect(Cookie cookie, Sender<OverSeerReply> replyChan) -> {
                for (CookieJarEntry t: cookieJar) {
                  if (t.cookie().equals(cookie)) {
                    long dt = t.inst().toEpochMilli() - Instant.now().toEpochMilli();
                    if (dt > 300_000) {
                      replyChan.send(new ReplyError(ErrorKind.InvalidCredentials));
                      break s;
                    } else {
                      this.connectedUsers.add(new Pair<>(t.user(), t.cookie()));
                      replyChan.send(new ReplyAuthorized(t.roomName(), cookie));
                      break s;
                    }
                  }
                }
                replyChan.send(new ReplyError(ErrorKind.InvalidCredentials));
              }
              case RequestUsernameFromCookie(Cookie cookie, Sender<OverSeerReply> replyChan) -> {
                for (Pair<String, Cookie> user : connectedUsers) {
                  if (user.right().equals(cookie)) {
                    replyChan.send(new ReplyAuthDetails(user.left(), user.right()));
                    break s;
                  }
                }
                replyChan.send(new ReplyError(ErrorKind.InvalidCredentials));
              }
              case RequestRoomList(Sender<OverSeerReply> replyChan) -> {
                List<String> roomNames = this.rooms.stream().map((p) -> p.channel().name()).toList();
                replyChan.send(new ReplyRoomList(roomNames));
              }
              case RequestNewRoom(Channel c, Sender<OverSeerReply> replyChan) -> {
                for (RoomEntry entry : rooms) {
                  if (entry.channel().name().equals(c.name())) {
                    replyChan.send(new ReplyError(ErrorKind.AlreadyExists));
                    break s;
                  }
                }
                Sender<RoomMsg> roomSender = new Sender<>(SENDER_BUFFER);
                RoomEntry roomEntry;
                if (c.isAi()) {
                  Ollama ollama = new Ollama(c.prompt().get(), roomSender);
                  ollama.start();
                  roomEntry = new RoomEntry(c, roomSender, Optional.of(ollama));
                } else {
                  roomEntry = new RoomEntry(c, roomSender, Optional.empty());
                }
                this.rooms.add(roomEntry);
                replyChan.send(new ReplyRoomChange(roomSender.clone()));
              }
            }
            maybe_msg = this.reqs.receive();
          } while (maybe_msg.isPresent());
        }
      } catch (Exception e) {
        // TODO: Toyota
        e.printStackTrace();
      }
    });
    return this.__reqSender;
  }

}
