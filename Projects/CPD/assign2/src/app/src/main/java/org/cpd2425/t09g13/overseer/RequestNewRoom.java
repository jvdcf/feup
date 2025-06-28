package org.cpd2425.t09g13.overseer;

import org.cpd2425.t09g13.protocol.Channel;
import org.cpd2425.t09g13.sync.channel.Sender;

public record RequestNewRoom(Channel c, Sender<OverSeerReply> chan) implements OverSeerRequest {

}
