package org.cpd2425.t09g13.protocol.server;

import org.cpd2425.t09g13.protocol.Message;
import org.cpd2425.t09g13.protocol.PktField;
import org.msgpack.core.MessagePack;
import org.msgpack.core.MessagePacker;

import java.io.IOException;
import java.io.OutputStream;

public record NewMessagePkt(Message m) implements ServerPkt {
    @Override
    public void to(MessagePacker packer) throws IOException {
        packer.packInt(PktField.NEW_MESSAGE);
        packer.packString(m.author());
        packer.packString(m.msg());
        packer.flush();
    }
}
