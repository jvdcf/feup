package org.cpd2425.t09g13.protocol.server;

import org.cpd2425.t09g13.protocol.PktField;
import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.msgpack.core.MessagePacker;

import java.io.IOException;

public record ConnectAckPkt(Cookie cookie) implements ServerPkt {
    @Override
    public void to(MessagePacker packer) throws IOException {
        packer.packInt(PktField.CONNECT_ACK);
        packer.packInt(PktField.COOKIE);
        packer.packLong(cookie.cookie());
        packer.flush();
    }
}
