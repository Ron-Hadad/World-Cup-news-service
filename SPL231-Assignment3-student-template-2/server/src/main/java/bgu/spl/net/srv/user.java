package bgu.spl.net.srv;

public class user {

  public String userName;
  private String Password;
  public boolean loggedIn;

  public user(String userName, String Password) {
    this.userName = userName;
    this.Password = Password;
    this.loggedIn = true;
  }

  public String getPassword() {
    return Password;
  }
}
