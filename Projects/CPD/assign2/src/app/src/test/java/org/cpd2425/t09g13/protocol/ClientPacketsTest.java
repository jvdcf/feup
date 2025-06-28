package org.cpd2425.t09g13.protocol;

import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.cpd2425.t09g13.protocol.auth.Credentials;
import org.cpd2425.t09g13.protocol.client.*;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.msgpack.core.MessagePack;
import org.msgpack.core.MessagePacker;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

public class ClientPacketsTest {
    private static ServerSocket serverSocket;
    private static Socket clientS;
    private static Socket serverS;
    private static MessagePacker packer;
    private static MessageUnpacker unpacker;

    @BeforeAll
    public static void setup() throws Exception {
        serverSocket = new ServerSocket(9001);
        clientS = new Socket("localhost", 9001);
        serverS = serverSocket.accept();
        packer = MessagePack.newDefaultPacker(clientS.getOutputStream());
        unpacker = MessagePack.newDefaultUnpacker(serverS.getInputStream());
    }

    @Test
    public void testConnectWithCredentials() throws IOException {
        ConnectPkt toSend = new ConnectPkt(new Credentials("john", "secret"));
        toSend.to(packer);

        Optional<ClientPkt> received = ClientPkt.from(unpacker);
        assertTrue(received.isPresent());
        ConnectPkt connectPkt = assertInstanceOf(ConnectPkt.class, received.get());
        Credentials credentials = assertInstanceOf(Credentials.class, connectPkt.auth());
        assertEquals("john", credentials.username());
        assertEquals("secret", credentials.passHash());
    }

    @Test
    public void testConnectWithCookie() throws IOException {
        ConnectPkt toSend = new ConnectPkt(new Cookie(1234));
        toSend.to(packer);

        Optional<ClientPkt> received = ClientPkt.from(unpacker);
        assertTrue(received.isPresent());
        ConnectPkt connectPkt = assertInstanceOf(ConnectPkt.class, received.get());
        Cookie cookie = assertInstanceOf(Cookie.class, connectPkt.auth());
        assertEquals(1234, cookie.cookie());
    }

    @Test
    public void testGetChs() throws IOException {
        GetChsPkt toSend = new GetChsPkt(new Cookie(1234));
        toSend.to(packer);

        Optional<ClientPkt> received = ClientPkt.from(unpacker);
        assertTrue(received.isPresent());
        GetChsPkt getChsPkt = assertInstanceOf(GetChsPkt.class, received.get());
        Cookie cookie = assertInstanceOf(Cookie.class, getChsPkt.cookie());
        assertEquals(1234, cookie.cookie());
    }

    @Test
    public void testJoinCh() throws IOException {
        JoinChPkt toSend = new JoinChPkt("myChannel", new Cookie(1234));
        toSend.to(packer);

        Optional<ClientPkt> received = ClientPkt.from(unpacker);
        assertTrue(received.isPresent());
        JoinChPkt joinChPkt = assertInstanceOf(JoinChPkt.class, received.get());
        assertEquals("myChannel", joinChPkt.chName());
        Cookie cookie = assertInstanceOf(Cookie.class, joinChPkt.cookie());
        assertEquals(1234, cookie.cookie());
    }

    @Test
    public void testExitCh() throws IOException {
        ExitChPkt toSend = new ExitChPkt(new Cookie(1234));
        toSend.to(packer);

        Optional<ClientPkt> received = ClientPkt.from(unpacker);
        assertTrue(received.isPresent());
        ExitChPkt exitChPkt = assertInstanceOf(ExitChPkt.class, received.get());
        Cookie cookie = assertInstanceOf(Cookie.class, exitChPkt.cookie());
        assertEquals(1234, cookie.cookie());
    }

    @Test
    public void testCreateCh() throws IOException {
        CreateChPkt toSend = new CreateChPkt(new Channel("myChannel", Optional.of("Reply in ooga-booga language.")), new Cookie(1234));
        toSend.to(packer);

        Optional<ClientPkt> received = ClientPkt.from(unpacker);
        assertTrue(received.isPresent());
        CreateChPkt createChPkt = assertInstanceOf(CreateChPkt.class, received.get());
        Channel channel = assertInstanceOf(Channel.class, createChPkt.channel());
        assertEquals("myChannel", channel.name());
        assertTrue(channel.isAi());
        assertEquals("Reply in ooga-booga language.", channel.prompt().get());
        Cookie cookie = assertInstanceOf(Cookie.class, createChPkt.cookie());
        assertEquals(1234, cookie.cookie());
    }

    @Test
    public void testMessage() throws IOException {
        MessagePkt toSend = new MessagePkt("Hello, World!", new Cookie(1234));
        toSend.to(packer);

        Optional<ClientPkt> received = ClientPkt.from(unpacker);
        assertTrue(received.isPresent());
        MessagePkt messagePkt = assertInstanceOf(MessagePkt.class, received.get());
        assertEquals("Hello, World!", messagePkt.msg());
        Cookie cookie = assertInstanceOf(Cookie.class, messagePkt.cookie());
        assertEquals(1234, cookie.cookie());
    }

    @Test
    public void testRegister() throws IOException {
        RegisterPkt toSend = new RegisterPkt(new Credentials("john", "secret"));
        toSend.to(packer);

        Optional<ClientPkt> received = ClientPkt.from(unpacker);
        assertTrue(received.isPresent());
        RegisterPkt registerPkt = assertInstanceOf(RegisterPkt.class, received.get());
        Credentials credentials = assertInstanceOf(Credentials.class, registerPkt.credentials());
        assertEquals("john", credentials.username());
        assertEquals("secret", credentials.passHash());
    }
}
