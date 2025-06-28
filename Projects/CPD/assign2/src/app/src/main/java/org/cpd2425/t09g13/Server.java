package org.cpd2425.t09g13;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;

import org.cpd2425.t09g13.overseer.OverSeerRequest;
import org.cpd2425.t09g13.sync.channel.Sender;


/**
 * Server
 */
public class Server {
  private ServerSocket sSock;

  public Server(Integer port) throws IOException {
    this.sSock = new ServerSocket();
    this.sSock.bind(new InetSocketAddress("0.0.0.0", port));
  }

  public void run(String database) throws IOException {
    OverSeer overSeer = new OverSeer(new ArrayList<>(), database);
    Sender<OverSeerRequest> overSeerRequester = overSeer.run();
    while (true) {
      Socket sock = this.sSock.accept();
      System.out.println("[INFO]: New connection: " + sock.getInetAddress());
      ServerWorker conn = new ServerWorker(overSeerRequester, sock);
      conn.start();
    }
  }
}
