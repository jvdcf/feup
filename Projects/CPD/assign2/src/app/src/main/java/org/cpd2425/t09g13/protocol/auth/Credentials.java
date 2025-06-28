package org.cpd2425.t09g13.protocol.auth;

public record Credentials(String username, String passHash) implements Auth {}
