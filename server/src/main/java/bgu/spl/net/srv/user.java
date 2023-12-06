package bgu.spl.net.srv;

import java.util.HashMap;

public class user {

  public String userName;
  private String Password;
  public boolean loggedIn;
  private HashMap<String, String> channToSubId;
  public HashMap<String, String> subIdToChan;

  public user(String userName, String Password) {
    this.userName = userName;
    this.Password = Password;
    this.loggedIn = true;
    this.channToSubId = new HashMap<>();
    this.subIdToChan = new HashMap<>();
  }

  public String getPassword() {
    return Password;
  }

  public void newSub(String chan, String subscribeId) {
    channToSubId.put(chan, subscribeId);
    subIdToChan.put(subscribeId, chan);
    System.out.print("the pair :" + chan + "," + subscribeId + "was added to the user subscribe records");
  }

  public void unSub(String subscribeId) {
    String chan = subIdToChan.get(subscribeId);
    channToSubId.remove(chan);
    subIdToChan.remove(subscribeId);
  }

  public boolean userSubToChann(String chan) {
    return channToSubId.containsKey(chan);
  }

  public boolean hasThatSubId(String subId) {
    return subIdToChan.containsKey(subId);
  }

  public void clearSub() {
    subIdToChan.clear();
    channToSubId.clear();
  }

}
