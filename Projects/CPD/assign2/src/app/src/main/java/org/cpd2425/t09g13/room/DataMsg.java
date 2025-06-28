package org.cpd2425.t09g13.room;

import org.cpd2425.t09g13.protocol.Message;

/**
 * DataMsg
 */
public record DataMsg(Message msg) implements RoomMsg {}
