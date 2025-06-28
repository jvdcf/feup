package org.cpd2425.t09g13.protocol;

public final class PktField {
    public static final int CONNECT = 0;
    public static final int GET_CHANNEL_LIST = 1;
    public static final int JOIN_CHANNEL = 2;
    public static final int EXIT_CHANNEL = 3;
    public static final int CREATE_CHANNEL = 4;
    public static final int MESSAGE = 5;
    public static final int REGISTER = 6;

    public static final int COOKIE = 7;
    public static final int CREDENTIALS = 8;

    public static final int ACKNOWLEDGE = 9;
    public static final int REJECT = 10;
    public static final int CONNECT_ACK = 11;
    public static final int CHANNEL_LIST = 12;
    public static final int NEW_MESSAGE = 13;

    private PktField() {}
}
