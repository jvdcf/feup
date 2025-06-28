package org.cpd2425.t09g13.protocol.client;

import org.cpd2425.t09g13.protocol.Channel;
import org.cpd2425.t09g13.protocol.PktField;
import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.msgpack.core.MessagePacker;

import java.io.IOException;

public final record CreateChPkt(Channel channel, Cookie cookie) implements ClientPkt {
    @Override
    public void to(MessagePacker packer) throws IOException {
        packer.packInt(PktField.CREATE_CHANNEL);
        packer.packString(channel.name());
        packer.packBoolean(channel.isAi());
        if (channel.isAi()) {
            packer.packString(channel.prompt().get());
        }
        packer.packInt(PktField.COOKIE);
        packer.packLong(cookie.cookie());
        packer.flush();
    }
}
