package org.cpd2425.t09g13;

import org.cpd2425.t09g13.client.TUI;

public class Main {
  public static void main(String[] args) throws Exception {
    if (args.length == 0) return;
    switch (args[0]) {
      case "Server":
      case "server":
        Server s = new Server(Integer.parseInt(args[1]));
        String databasePath = args[2];
        s.run(databasePath);
      break;

      case "Client":
      case "client":
        Client c = new Client(args[1], Integer.parseInt(args[2]));
        c.run();
        break;
      
      case "server-mock":
        ServerMock m = new ServerMock(Integer.parseInt(args[1]));
        m.run();
        break;

      default:
        return;
    }
  }
}
