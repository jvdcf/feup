package org.cpd2425.t09g13.protocol.client;

import org.cpd2425.t09g13.protocol.PktField;
import org.cpd2425.t09g13.protocol.auth.Credentials;
import org.msgpack.core.MessagePacker;

import java.io.IOException;

public record RegisterPkt(Credentials credentials) implements ClientPkt {
    @Override
    public void to(MessagePacker packer) throws IOException {
        packer.packInt(PktField.REGISTER);
        packer.packInt(PktField.CREDENTIALS);
        packer.packString(credentials.username());
        packer.packString(credentials.passHash());
        packer.flush();
    }
}
