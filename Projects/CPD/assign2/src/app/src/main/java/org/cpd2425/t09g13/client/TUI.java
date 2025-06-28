package org.cpd2425.t09g13.client;

import java.io.IOException;
import java.net.Socket;
import java.util.List;

import org.cpd2425.t09g13.protocol.auth.Auth;
import org.cpd2425.t09g13.protocol.auth.Credentials;
import org.cpd2425.t09g13.protocol.client.ConnectPkt;
import org.msgpack.core.MessagePack;
import org.msgpack.core.MessagePacker;

import com.googlecode.lanterna.TerminalSize;
import com.googlecode.lanterna.gui2.BasicWindow;
import com.googlecode.lanterna.gui2.Button;
import com.googlecode.lanterna.gui2.CheckBox;
import com.googlecode.lanterna.gui2.Direction;
import com.googlecode.lanterna.gui2.EmptySpace;
import com.googlecode.lanterna.gui2.GridLayout;
import com.googlecode.lanterna.gui2.Label;
import com.googlecode.lanterna.gui2.LinearLayout;
import com.googlecode.lanterna.gui2.Panel;
import com.googlecode.lanterna.gui2.TextBox;

public interface TUI {
    public void draw() throws IOException;
}
