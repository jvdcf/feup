package org.cpd2425.t09g13.protocol;

import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.cpd2425.t09g13.protocol.server.*;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.msgpack.core.MessagePack;
import org.msgpack.core.MessagePacker;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

import static org.junit.jupiter.api.Assertions.*;

public class ServerPacketsTest {
    private static MessagePacker packer;
    private static MessageUnpacker unpacker;
    private static ServerSocket serverSocket;
    private static Socket clientS;
    private static Socket serverS;

    @BeforeAll
    public static void setup() throws Exception {
        serverSocket = new ServerSocket(9001);
        clientS = new Socket("localhost", 9001);
        serverS = serverSocket.accept();
        packer = MessagePack.newDefaultPacker(serverS.getOutputStream());
        unpacker = MessagePack.newDefaultUnpacker(clientS.getInputStream());
    }

    @Test
    public void testAck() throws IOException {
        AckPkt toSend = new AckPkt();
        toSend.to(packer);

        Optional<ServerPkt> received = ServerPkt.from(unpacker);
        assertTrue(received.isPresent());
        assertInstanceOf(AckPkt.class, received.get());
    }

    @Test
    public void testRej() throws IOException {
        RejPkt toSend = RejPkt.InvalidPacket();
        toSend.to(packer);

        Optional<ServerPkt> received = ServerPkt.from(unpacker);
        assertTrue(received.isPresent());
        RejPkt rejPkt = assertInstanceOf(RejPkt.class, received.get());
        assertEquals(RejPkt.INVALID_PACKET, rejPkt.reason());
    }

    @Test
    public void testConnectAck() throws IOException {
        ConnectAckPkt toSend = new ConnectAckPkt(new Cookie(1234));
        toSend.to(packer);

        Optional<ServerPkt> received = ServerPkt.from(unpacker);
        assertTrue(received.isPresent());
        ConnectAckPkt connectAckPkt = assertInstanceOf(ConnectAckPkt.class, received.get());
        assertEquals(1234, connectAckPkt.cookie().cookie());
    }

    @Test
    public void testChannelList() throws IOException {
        ArrayList<Channel> channelList = new ArrayList<>();
        channelList.add(new Channel("Welcome", Optional.of("Say hi!")));
        channelList.add(new Channel("Tasty Recipes", Optional.empty()));
        channelList.add(new Channel("CPD Chat Room", Optional.empty()));
        channelList.add(new Channel("LLM Chat", Optional.of("This room is yours!")));

        List<String> channelNames = channelList.stream().map((c) -> {
            return c.name();
        }).collect(Collectors.toList());

        ChannelListPkt toSend = new ChannelListPkt(channelNames);
        toSend.to(packer);

        Optional<ServerPkt> received = ServerPkt.from(unpacker);
        assertTrue(received.isPresent());
        ChannelListPkt channelListPkt = assertInstanceOf(ChannelListPkt.class, received.get());
        assertEquals(channelNames, channelListPkt.channels());
    }

    @Test
    public void testNewMessage() throws IOException {
        String author = "Unit Test";
        String msg = "A sample message for a unit test.";
        NewMessagePkt toSend = new NewMessagePkt(new Message(author, msg));
        toSend.to(packer);

        Optional<ServerPkt> received = ServerPkt.from(unpacker);
        assertTrue(received.isPresent());
        NewMessagePkt newMessagePkt = assertInstanceOf(NewMessagePkt.class, received.get());
        assertEquals(author, newMessagePkt.m().author());
        assertEquals(msg, newMessagePkt.m().msg());
    }
}
