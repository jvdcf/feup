package org.cpd2425.t09g13.overseer;

import java.util.Optional;

import org.cpd2425.t09g13.protocol.auth.Cookie;

/**
 * RequestCookieJar
 */
public record RequestCookieJar(String user, Optional<String> roomName) implements OverSeerRequest {
}
