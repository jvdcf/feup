package org.cpd2425.t09g13.overseer;

import org.cpd2425.t09g13.protocol.auth.Cookie;
import org.cpd2425.t09g13.sync.channel.Sender;

/**
 * RequestReconnect
 */
public record RequestReconnect(Cookie cookie, Sender<OverSeerReply> replyChan) implements OverSeerRequest {

}
