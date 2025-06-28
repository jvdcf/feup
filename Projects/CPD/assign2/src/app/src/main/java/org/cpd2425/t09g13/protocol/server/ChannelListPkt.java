package org.cpd2425.t09g13.protocol.server;

import org.cpd2425.t09g13.protocol.Channel;
import org.cpd2425.t09g13.protocol.PktField;
import org.msgpack.core.MessagePacker;

import java.io.IOException;
import java.util.List;

public record ChannelListPkt(List<String> channels) implements ServerPkt {
    @Override
    public void to(MessagePacker packer) throws IOException {
        packer.packInt(PktField.CHANNEL_LIST);
        packer.packInt(channels.size());
        for (String channel : channels) {
            packer.packString(channel);
        }
        packer.flush();
    }
}
