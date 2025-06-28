package org.cpd2425.t09g13.overseer;

import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.cpd2425.t09g13.sync.channel.Sender;
import org.cpd2425.t09g13.utils.Pair;

/**
 * RequestRoomMsg
 */
public record RequestRoomChange(String chanName, Pair<String, Cookie> userPair, Sender<OverSeerReply> replyChan) implements OverSeerRequest {
}
