package org.cpd2425.t09g13.protocol.client;

import org.cpd2425.t09g13.protocol.PktField;
import org.cpd2425.t09g13.protocol.auth.Auth;
import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.cpd2425.t09g13.protocol.auth.Credentials;
import org.msgpack.core.MessagePacker;

import java.io.IOException;

public final record ConnectPkt(Auth auth) implements ClientPkt {
    @Override
    public void to(MessagePacker packer) throws IOException {
        packer.packInt(PktField.CONNECT);
        switch (auth) {
            case Cookie(long cookie) -> {
                packer.packInt(PktField.COOKIE);
                packer.packLong(cookie);
            }
            case Credentials(String username, String password) -> {
                packer.packInt(PktField.CREDENTIALS);
                packer.packString(username);
                packer.packString(password);
            }
        }
        packer.flush();
    }
}
