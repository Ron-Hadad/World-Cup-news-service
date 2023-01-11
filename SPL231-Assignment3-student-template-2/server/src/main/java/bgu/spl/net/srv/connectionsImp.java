package bgu.spl.net.srv;

import java.io.IOException;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.HashMap;

import javax.swing.text.html.HTMLDocument.Iterator;

public class connectionsImp<T> implements Connections<T> {
  /*
   * mapping between connection Ids and their connection handlers(generic. can be
   * NonBlockingConnectionHandler or BlockingConnectionHandler)
   */
  public HashMap<Integer, ConnectionHandler<T>> connIdToConnHand;

  /*
   * mapping between the existing user names and their password.
   */
  public HashMap<String, String> userNameToPass;

  /*
   * mapping between channels names and array of the subscribed clients
   * connectionHandlers
   */
  public HashMap<String, ArrayList<ConnectionHandler<T>>> channNameToSubConnHand;

  public boolean send(int connectionId, T msg) {
    // in which case well wanna return false?
    // maybe the connection ID does not exist?
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

}
