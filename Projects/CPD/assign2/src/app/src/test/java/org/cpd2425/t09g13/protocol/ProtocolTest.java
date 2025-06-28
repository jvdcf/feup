package org.cpd2425.t09g13.protocol;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.assertEquals;

import org.msgpack.core.MessagePack;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;

public class ProtocolTest {
    @Test
    void test() {
        try {
            ServerSocket ss = new ServerSocket(9001);
            Socket clientS = new Socket("localhost", 9001);
            Socket serverS = ss.accept();

            byte[] buffer = {1};
            clientS.getOutputStream().write(buffer);

            MessageUnpacker unpacker = MessagePack.newDefaultUnpacker(serverS.getInputStream());
            int first = unpacker.unpackInt();

            assertEquals(1, first);

            // NOTE: These fail, which proves that Message Pack reads all bytes in the input stream.
            //       It does not block, so the flow continues after all bytes are read.
            // assertEquals(3, serverS.getInputStream().available());
            // byte[] rcvd = serverS.getInputStream().readNBytes(1);
            // assertEquals(2, rcvd[0]);

            ss.close();
            clientS.close();

        } catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}
