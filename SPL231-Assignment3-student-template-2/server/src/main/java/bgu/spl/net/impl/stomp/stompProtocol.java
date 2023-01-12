package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.connectionsImp;
import bgu.spl.net.srv.user;

import java.time.LocalDateTime;
import java.util.ArrayList;

public class stompProtocol implements StompMessagingProtocol<String> {

    private boolean shouldTerminate = false;
    public Connections connections;
    int connectionId;

    public void start(int connectionId, Connections connections) {
        this.connections = connections;
        this.connectionId = connectionId;
    }

    @Override
    public String process(String msg) {
        String[] msgSpliteByLines = msg.split("/n", 0);
        String stompCommand = msgSpliteByLines[0];
        switch (stompCommand) {
            case "CONNECT":
                connectProtocol(msgSpliteByLines);
            case "SEND":
                sendProtocol(msgSpliteByLines);
            case "SUBSCRIBE":
                subscribeProtocol(msgSpliteByLines);
            case "UNSUBSCRIBE":
                unsubscribeProtocol(msgSpliteByLines);
            case "DISCONNECT":
                disconnectProtocol(msgSpliteByLines);
        }
        return "extention most";
    }

    private void connectProtocol(String[] msgSpliteByLines) {
        // collecting info:
        String givenUserName = null;
        String givenPasscode = null;
        String givenReceiptId = null;
        for (String line : msgSpliteByLines) {
            if (line.startsWith("accept-vertion:1.2")) {
                // then what? - nothing?
            }
            if (line.startsWith("host:")) {
                // then what? - nothing?
            }
            if (line.startsWith("login:")) {
                givenUserName = line.substring(6);
            }
            if (line.startsWith("passcode:")) {
                givenPasscode = line.substring(9);
            }
            if (line.startsWith("receipt-id:")) {
                givenReceiptId = line.substring(11);
            }
        }
        // proccesing info:
        if (connections.getUser(givenUserName) != null) {// userName exist
            if (connections.getUser(givenUserName).getPassword() == givenPasscode) {// correct password
                connections.send(connectionId, "CONNECTED\nvertion:1.2\n");
                if (givenReceiptId != null) {// if he want a receipt
                    connections.send(connectionId, "RECEIPT\nreceipt-id:" + givenReceiptId + "\n");
                }
            } else {// wrong password
                String receiptLine = "";
                if (givenReceiptId != null) {
                    receiptLine = "receipt-id:" + givenReceiptId;
                }
                String msg = String.format("ERROR\nmessage: wrong password. the password you typed: %s \n %s",
                        givenPasscode, receiptLine);
                connections.send(connectionId, msg);
            }
        } else {// userName doesnt exist
            connections.addUser(givenUserName, givenPasscode);
            connections.send(connectionId, "CONNECTED\nvertion:1.2\n");
            if (givenReceiptId != null) {// if he want a receipt
                connections.send(connectionId, "RECEIPT\nreceipt-id:" + givenReceiptId + "\n");
            }
        }
    }

    private void sendProtocol(String[] msgSpliteByLines) {

    }

    private void subscribeProtocol(String[] msgSpliteByLines) {

    }

    private void unsubscribeProtocol(String[] msgSpliteByLines) {

    }

    private void disconnectProtocol(String[] msgSpliteByLines) {

    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
