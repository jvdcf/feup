package org.cpd2425.t09g13.overseer;

/**
 * OverSeerMsg
 */
public sealed interface OverSeerRequest permits RequestRoomChange, RequestRegister, RequestLogin, RequestCookieJar, RequestReconnect, RequestUsernameFromCookie, RequestRoomList, RequestNewRoom {

}
