package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessagingProtocol;
import java.time.LocalDateTime;

public class stompProtocol implements MessagingProtocol<String> {

    private boolean shouldTerminate = false;

    @Override
    public String process(String msg) {
        return null;
    }

    // private String createEcho(String message) {
    // String echoPart = message.substring(Math.max(message.length() - 2, 0),
    // message.length());
    // return message + " .. " + echoPart + " .. " + echoPart + " ..";
    // }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
