package org.cpd2425.t09g13.overseer;

/**
 * OverSeerReply
 */
public sealed interface OverSeerReply permits ReplyRoomChange, ReplyAuthorized, ReplyError, ReplyAuthDetails, ReplyRoomList {
  
}
