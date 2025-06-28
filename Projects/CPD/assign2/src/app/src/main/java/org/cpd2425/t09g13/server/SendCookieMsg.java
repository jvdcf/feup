package org.cpd2425.t09g13.server;

import org.cpd2425.t09g13.protocol.auth.Cookie;

/**
 * ExitMsg
 */
public record SendCookieMsg(Cookie cookie) implements InternalMsg {}
