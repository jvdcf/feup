package org.cpd2425.t09g13.protocol.client;

import org.cpd2425.t09g13.protocol.Channel;
import org.cpd2425.t09g13.protocol.Packet;
import org.cpd2425.t09g13.protocol.PktField;
import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.cpd2425.t09g13.protocol.auth.Credentials;
import org.msgpack.core.MessagePacker;
import org.msgpack.core.MessageTypeException;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;
import java.util.Optional;

public sealed interface ClientPkt extends Packet permits ConnectPkt, GetChsPkt, JoinChPkt, ExitChPkt, CreateChPkt, MessagePkt, RegisterPkt {
    public static Optional<ClientPkt> from(MessageUnpacker unpacker) throws IOException {
        try {
            switch (unpacker.unpackInt()) {
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
                    Optional<String> prompt = Optional.empty();
                    if (unpacker.unpackBoolean()) {
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
                default -> { throw new IOException("Invalid Packet!"); }
            }
        } catch (MessageTypeException e) {} // In case there is a format error in the message.
        return Optional.empty();
    }

    public void to(MessagePacker messagePacker) throws IOException;
}
