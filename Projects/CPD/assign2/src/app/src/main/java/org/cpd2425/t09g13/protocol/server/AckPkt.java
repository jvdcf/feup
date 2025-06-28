package org.cpd2425.t09g13.protocol.server;

import org.cpd2425.t09g13.protocol.PktField;
import org.msgpack.core.MessagePacker;

import java.io.IOException;

public record AckPkt() implements ServerPkt {
    @Override
    public void to(MessagePacker packer) throws IOException {
        packer.packInt(PktField.ACKNOWLEDGE);
        packer.flush();
    }
}
