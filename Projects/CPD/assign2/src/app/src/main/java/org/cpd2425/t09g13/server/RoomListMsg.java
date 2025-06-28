package org.cpd2425.t09g13.server;

import java.util.List;

public record RoomListMsg(List<String> roomNames) implements InternalMsg {

}
