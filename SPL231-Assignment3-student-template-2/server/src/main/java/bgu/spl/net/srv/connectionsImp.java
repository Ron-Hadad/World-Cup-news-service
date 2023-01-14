package bgu.spl.net.srv;

import java.io.IOException;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.HashMap;

public class connectionsImp<T> implements Connections<T> {

  /*
   * the next free to use connection id
   */
  private static int freeToUseConnId;

  /*
   * the next free to use messege id
   */
  private static int freeToUseMessegeId;

  /*
   * mapping between connection Ids and their connection handlers(generic. can be
   * NonBlockingConnectionHandler or BlockingConnectionHandler)
   */
  public HashMap<Integer, ConnectionHandler<T>> connIdToConnHand;

  /*
   * mapping the users by userName.
   */
  public HashMap<String, user> userNameToUser;

  /*
   * mapping between channels names and array of the subscribed clients
   * connectionHandlers
   */
  public HashMap<String, ArrayList<ConnectionHandler<T>>> channNameToSubConnHand;

  // making it a singlton:
  private static connectionsImp connectionsInstance = null;

  public static connectionsImp getInstance() {
    if (connectionsInstance == null) {
      connectionsInstance = new connectionsImp<>();
    }
    return connectionsInstance;
  }

  private connectionsImp() {
    freeToUseConnId = 0;
    freeToUseMessegeId = 0;
    connIdToConnHand = new HashMap<>();
    userNameToUser = new HashMap<>();
    channNameToSubConnHand = new HashMap<>();
    // channNameToSubUsers = new HashMap<>();
  }

  public boolean send(int connectionId, T msg) {
    connIdToConnHand.get(connectionId).send(msg);
    return false;
  }

  public void send(String channel, T msg) {
    ArrayList<ConnectionHandler<T>> subConnHan = channNameToSubConnHand.get(channel);
    for (ConnectionHandler<T> current : subConnHan) {
      current.send(msg);
    }

  }

  public void disconnect(int connectionId) {
    connIdToConnHand.remove(connectionId);
  }

  public static int getFreeToUseConnId() {
    freeToUseConnId++;
    return freeToUseConnId;
  }

  public int getFreeToUseMessegeId() {
    freeToUseMessegeId++;
    return freeToUseMessegeId;
  }

  public user getUser(String userName) {
    return userNameToUser.get(userName);
  }

  public void addUser(String userName, String passcode) {
    user newOne = new user(userName, passcode);
    userNameToUser.put(userName, newOne);
  }

  public ConnectionHandler<T> getConnHand(Integer connectionId) {
    return connIdToConnHand.get(connectionId);
  }

  public void addConnId(Integer connectionId, ConnectionHandler<T> connectionHandler) {
    connIdToConnHand.put(connectionId, connectionHandler);
  }

  public boolean channelExist(String chan) {
    return channNameToSubConnHand.containsKey(chan);
  }

  public void addSub(String userName, String chan, String SubId, int connId) {
    // adding to the user:
    getUser(userName).newSub(chan, SubId);
    // adding to the hash map of the subscribed handllers to the channel:
    channNameToSubConnHand.get(chan).add(getConnHand(connId));
  }

  public void unSub(String userName, String SubId, int connId) {
    // removing from the "connections" DB:
    String chan = getUser(userName).subIdToChan.get(SubId);
    channNameToSubConnHand.get(chan).remove(getConnHand(connId));
    // removing the sub from ths=e user:
    getUser(userName).unSub(SubId);
  }

  public void disconnect(String userName, int connId) {
    // removing subscribe from "connections" DB:
    ConnectionHandler<T> toDisconnect = connIdToConnHand.get(connId);
    channNameToSubConnHand.forEach((key, value) -> value.remove(toDisconnect));
    // removing subscribe from user DB:
    getUser(userName).clearSub();
  }
}
