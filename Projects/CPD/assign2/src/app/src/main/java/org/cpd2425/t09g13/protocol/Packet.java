package org.cpd2425.t09g13.protocol;

import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.cpd2425.t09g13.protocol.auth.Credentials;
import org.cpd2425.t09g13.protocol.client.*;
import org.cpd2425.t09g13.protocol.server.AckPkt;
import org.cpd2425.t09g13.protocol.server.ChannelListPkt;
import org.cpd2425.t09g13.protocol.server.ConnectAckPkt;
import org.cpd2425.t09g13.protocol.server.NewMessagePkt;
import org.cpd2425.t09g13.protocol.server.RejPkt;
import org.msgpack.core.MessagePacker;
import org.msgpack.core.MessageTypeException;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Optional;

public interface Packet {
    public static Optional<Packet> from(MessageUnpacker unpacker) throws IOException {
        try {
            switch (unpacker.unpackInt()) {
                // Client -> Server Packets ----------------------------------------------------------------------------
                case PktField.CONNECT -> {
                    switch (unpacker.unpackInt()) {
                        case PktField.CREDENTIALS -> {
                            return Optional.of(new ConnectPkt(new Credentials(unpacker.unpackString(), unpacker.unpackString())));
                        }
                        case PktField.COOKIE -> {
                            return Optional.of(new ConnectPkt(new Cookie(unpacker.unpackLong())));
                        }
                        default -> { return Optional.empty(); }
                    }
                }
                case PktField.GET_CHANNEL_LIST -> {
                    if (unpacker.unpackInt() == PktField.COOKIE) {
                        return Optional.of(new GetChsPkt(new Cookie(unpacker.unpackLong())));
                    }
                    return Optional.empty();
                }
                case PktField.JOIN_CHANNEL -> {
                    String chName = unpacker.unpackString();
                    if (unpacker.unpackInt() == PktField.COOKIE) {
                        return Optional.of(new JoinChPkt(chName, new Cookie(unpacker.unpackLong())));
                    }
                    return Optional.empty();
                }
                case PktField.EXIT_CHANNEL -> {
                    if (unpacker.unpackInt() == PktField.COOKIE) {
                        return Optional.of(new ExitChPkt(new Cookie(unpacker.unpackLong())));
                    }
                    return Optional.empty();
                }
                case PktField.CREATE_CHANNEL -> {
                    String chName = unpacker.unpackString();
                    boolean ai = unpacker.unpackBoolean();
                    Optional<String> prompt = Optional.empty();
                    if (ai) {
                        prompt = Optional.of(unpacker.unpackString());
                    }
                    if (unpacker.unpackInt() == PktField.COOKIE) {
                        return Optional.of(new CreateChPkt(new Channel(chName, prompt), new Cookie(unpacker.unpackLong())));
                    }
                    return Optional.empty();
                }
                case PktField.MESSAGE -> {
                    String msg = unpacker.unpackString();
                    if (unpacker.unpackInt() == PktField.COOKIE) {
                        return Optional.of(new MessagePkt(msg, new Cookie(unpacker.unpackLong())));
                    }
                    return Optional.empty();
                }
                case PktField.REGISTER -> {
                    if (unpacker.unpackInt() == PktField.CREDENTIALS) {
                        return Optional.of(new RegisterPkt(new Credentials(unpacker.unpackString(), unpacker.unpackString())));
                    }
                    return Optional.empty();
                }
                // Server -> Client Packets ----------------------------------------------------------------------------
                case PktField.ACKNOWLEDGE -> { return Optional.of(new AckPkt()); }
                case PktField.REJECT -> { return Optional.of(new RejPkt(unpacker.unpackInt())); }
                case PktField.CONNECT_ACK -> {
                    if (unpacker.unpackInt() == PktField.COOKIE) {
                        return Optional.of(new ConnectAckPkt(new Cookie(unpacker.unpackLong())));
                    }
                    return Optional.empty();
                }
                case PktField.CHANNEL_LIST -> {
                    int size = unpacker.unpackInt();
                    ArrayList<String> channels = new ArrayList<>(size);
                    for (int i = 0; i < size; i++) {
                        channels.add(unpacker.unpackString());
                    }
                    return Optional.of(new ChannelListPkt(channels));
                }
                case PktField.NEW_MESSAGE -> {
                    String author = unpacker.unpackString();
                    String msg = unpacker.unpackString();
                    return Optional.of(new NewMessagePkt(new Message(author, msg)));
                }

                default -> { return Optional.empty(); }
            }
        } catch (MessageTypeException e) {} // In case there is a format error in the message.
        return Optional.empty();
    }

    public void to(MessagePacker messagePacker) throws IOException;
}
