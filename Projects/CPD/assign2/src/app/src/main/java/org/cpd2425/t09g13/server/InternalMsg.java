package org.cpd2425.t09g13.server;



public sealed interface InternalMsg permits ExitMsg, WelcomeMsg, SendAckMsg, ErrorMsg, SendCookieMsg, RoomListMsg, NewMsg {
  
}


