package org.cpd2425.t09g13.overseer;

import org.cpd2425.t09g13.room.RoomMsg;
import org.cpd2425.t09g13.sync.channel.Sender;

/**
 * ReplyRoomMsg
 */
public record ReplyRoomChange(Sender<RoomMsg> room) implements OverSeerReply {
}
