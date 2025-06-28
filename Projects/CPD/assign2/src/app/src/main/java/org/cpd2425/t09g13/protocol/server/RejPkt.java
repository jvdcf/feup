package org.cpd2425.t09g13.protocol.server;

import org.cpd2425.t09g13.protocol.PktField;
import org.msgpack.core.MessagePacker;

import java.io.IOException;

public record RejPkt(int reason) implements ServerPkt {
    public static final int UNKNOWN = 0;
    public static final int INVALID_PACKET = 1;
    public static final int ALREADY_CONNECTED = 2;
    public static final int NOT_CONNECTED = 3;
    public static final int INVALID_COOKIE = 4;
    public static final int INVALID_CREDENTIALS = 5;
    public static final int CHANNEL_ALREADY_EXISTS = 6;
    public static final int OUTSIDE_CHANNEL = 7;
    public static final int CHANNEL_NOT_EXISTS = 8;
    public static final int ACCOUNT_ALREADY_EXISTS = 9;

    public RejPkt {
        if (reason < UNKNOWN || reason > ACCOUNT_ALREADY_EXISTS) {
            reason = UNKNOWN;
        }
    }

    public RejPkt() {
        this(UNKNOWN);
    }

    public static RejPkt InvalidPacket() {
        return new RejPkt(INVALID_PACKET);
    }

    public static RejPkt AlreadyConnected() {
        return new RejPkt(ALREADY_CONNECTED);
    }

    public static RejPkt NotConnected() {
        return new RejPkt(NOT_CONNECTED);
    }

    public static RejPkt InvalidCookie() {
        return new RejPkt(INVALID_COOKIE);
    }

    public static RejPkt InvalidCredentials() {
        return new RejPkt(INVALID_CREDENTIALS);
    }

    public static RejPkt ChannelAlreadyExists() {
        return new RejPkt(CHANNEL_ALREADY_EXISTS);
    }

    public static RejPkt OutsideChannel() {
        return new RejPkt(OUTSIDE_CHANNEL);
    }

    public static RejPkt ChannelNotExists() {
        return new RejPkt(CHANNEL_NOT_EXISTS);
    }

    public static RejPkt AccountAlreadyExists() {
        return new RejPkt(ACCOUNT_ALREADY_EXISTS);
    }

    @Override
    public void to(MessagePacker packer) throws IOException {
        packer.packInt(PktField.REJECT);
        packer.packInt(reason);
        packer.flush();
    }
}
