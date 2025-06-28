package org.cpd2425.t09g13.server;

/**
 * ErrorMsg
 */
public record ErrorMsg(Kind k) implements InternalMsg {
  public enum Kind {
    AuthFailure,
    InvalidRoom,
    DuplicatedSession,
    AccountAlreadyExists,
    RoomAlreadyExists,
    OutsideRoom,
  }

  public static ErrorMsg duplicatedSession() {
    return new ErrorMsg(Kind.DuplicatedSession);
  }

  public static ErrorMsg authFailure() {
    return new ErrorMsg(Kind.AuthFailure);
  }

  public static ErrorMsg invalidRoom() {
    return new ErrorMsg(Kind.InvalidRoom);
  }

  public static ErrorMsg accountAlreadyExists() {
    return new ErrorMsg(Kind.AccountAlreadyExists);
  }

  public static ErrorMsg roomAlreadyExists() {
    return new ErrorMsg(Kind.RoomAlreadyExists);
  }

  public static ErrorMsg outsideRoom() {
    return new ErrorMsg(Kind.OutsideRoom);
  }
}
