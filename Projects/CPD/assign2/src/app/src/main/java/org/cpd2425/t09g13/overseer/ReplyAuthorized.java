package org.cpd2425.t09g13.overseer;

import java.util.Optional;

import org.cpd2425.t09g13.protocol.auth.Cookie;


/**
 * ReplyAuth
 */
public record ReplyAuthorized(Optional<String> roomName, Cookie cookie) implements OverSeerReply {
  
}

