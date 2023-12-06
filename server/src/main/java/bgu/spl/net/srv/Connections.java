package bgu.spl.net.srv;

//import java.io.IOException;
//import java.util.HashMap;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    // geters:
    public int getFreeToUseMessegeId();

    public ConnectionHandler<T> getConnHand(Integer connectionId);

    public user getUser(String userName);

    public boolean channelExist(String chan);

    // adders:
    public void addUser(String userName, String passcode);

    public void addConnId(Integer connectionId, ConnectionHandler<T> connectionHandler);

    public void addSub(String userName, String chan, String SubId, int connId);

    // remove:
    public void unSub(String userName, String SubId, int connId);

}
