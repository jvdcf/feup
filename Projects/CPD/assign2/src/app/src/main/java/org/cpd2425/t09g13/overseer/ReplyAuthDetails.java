package org.cpd2425.t09g13.overseer;

import org.cpd2425.t09g13.protocol.auth.Cookie;
/**
 * ReplyAuthDetails
 */

public record ReplyAuthDetails(String username, Cookie cookie) implements OverSeerReply {
}
