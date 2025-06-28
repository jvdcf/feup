package org.cpd2425.t09g13.protocol.server;

import org.cpd2425.t09g13.protocol.Channel;
import org.cpd2425.t09g13.protocol.Message;
import org.cpd2425.t09g13.protocol.Packet;
import org.cpd2425.t09g13.protocol.PktField;
import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.msgpack.core.MessagePacker;
import org.msgpack.core.MessageTypeException;
import org.msgpack.core.MessageUnpacker;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Optional;

public sealed interface ServerPkt extends Packet permits AckPkt, RejPkt, ConnectAckPkt, ChannelListPkt, NewMessagePkt {
    public static Optional<ServerPkt> from(MessageUnpacker unpacker) throws IOException {
        try {
            switch (unpacker.unpackInt()) {
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
                        String chan = unpacker.unpackString();
                        channels.add(chan);
                    }
                    return Optional.of(new ChannelListPkt(channels));
                }
                case PktField.NEW_MESSAGE -> {
                    String author = unpacker.unpackString();
                    String msg = unpacker.unpackString();
                    return Optional.of(new NewMessagePkt(new Message(author, msg)));
                }
                default -> { throw new IOException("Invalid Packet!"); }
            }
        } catch (MessageTypeException e) {}
        return Optional.empty();
    }

    public void to(MessagePacker packer) throws IOException;
}
