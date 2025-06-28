package org.cpd2425.t09g13.client;

import org.cpd2425.t09g13.protocol.Packet;

public enum StateMachine {
    Disconnected,
    Registering,
    Connecting,
    OutsideCh,
    CreatingCh,
    JoiningCh,
    InsideCh,
    ExitingCh,
    SendingMsg;
}
