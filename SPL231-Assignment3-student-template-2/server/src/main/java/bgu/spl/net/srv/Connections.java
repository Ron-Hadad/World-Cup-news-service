package bgu.spl.net.srv;

import java.io.IOException;
import java.util.HashMap;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    // geters:
    public ConnectionHandler<T> getConnHand(Integer connectionId);

    public user getUser(String userName);

    // adders:
    public void addUser(String userName, String passcode);

    public void addConnId(Integer connectionId, ConnectionHandler<T> connectionHandler);

}
