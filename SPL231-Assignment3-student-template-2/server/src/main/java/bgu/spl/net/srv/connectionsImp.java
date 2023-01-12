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
    connIdToConnHand = new HashMap<>();
    userNameToUser = new HashMap<>();
    channNameToSubConnHand = new HashMap<>();
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

}
