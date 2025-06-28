package org.cpd2425.t09g13.overseer;

import org.cpd2425.t09g13.sync.channel.Sender;

/**
 * RequestLogin
 */
public record RequestLogin(String user, String passHash, Sender<OverSeerReply> chan) implements OverSeerRequest {
}
