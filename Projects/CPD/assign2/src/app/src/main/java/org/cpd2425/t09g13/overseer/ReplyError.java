package org.cpd2425.t09g13.overseer;

/**
 * ReplyError
 */
public record ReplyError(ErrorKind e) implements OverSeerReply {
  public enum ErrorKind {
    NoSuchRoom,
    AlreadyExists,
    InvalidCredentials,
    AlreadyLoggedIn,
  }
}
