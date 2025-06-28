package org.cpd2425.t09g13.protocol.client;

import org.cpd2425.t09g13.protocol.PktField;
import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.msgpack.core.MessagePacker;

import java.io.IOException;

public final record JoinChPkt(String chName, Cookie cookie) implements ClientPkt {
    @Override
    public void to(MessagePacker packer) throws IOException {
        packer.packInt(PktField.JOIN_CHANNEL);
        packer.packString(chName);
        packer.packInt(PktField.COOKIE);
        packer.packLong(cookie.cookie());
        packer.flush();
    }
}
