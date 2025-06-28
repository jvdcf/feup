package org.cpd2425.t09g13.overseer;

import org.cpd2425.t09g13.sync.channel.Sender;

public record RequestRoomList(Sender<OverSeerReply> replyChan) implements OverSeerRequest {

}
