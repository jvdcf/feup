package org.cpd2425.t09g13.server;

import org.cpd2425.t09g13.protocol.Message;

public record NewMsg(Message msg) implements InternalMsg {

}
