package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessagingProtocol;
import java.time.LocalDateTime;

public class stompProtocol implements MessagingProtocol<String> {

    private boolean shouldTerminate = false;

    @Override
    public String process(String msg) {
        int firstEndLineIndex = msg.indexOf("/n");
        String stompCommand = msg.substring(0, firstEndLineIndex);

        switch (stompCommand) {
            case "CONNECT":
                connectProtocol();
            case "SEND":
                sendProtocol();
            case "SUBSCRIBE":
                subscribeProtocol();
            case "UNSUBSCRIBE":
                unsubscribeProtocol();
            case "DISCONNECT":
                disconnectProtocol();
        }
        // while(msg != null){
        // int nextEndLineIndex = msg.indexOf("/n");
        // String stomCommand = msg.substring(firstEndLineIndex, firstEndLineIndex)
        // }

        return null;
    }

    // private String createEcho(String message) {
    // String echoPart = message.substring(Math.max(message.length() - 2, 0),
    // message.length());
    // return message + " .. " + echoPart + " .. " + echoPart + " ..";
    // }

    private void connectProtocol() {
        // how to access connections?
    }

    private void sendProtocol() {

    }

    private void subscribeProtocol() {

    }

    private void unsubscribeProtocol() {

    }

    private void disconnectProtocol() {

    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
