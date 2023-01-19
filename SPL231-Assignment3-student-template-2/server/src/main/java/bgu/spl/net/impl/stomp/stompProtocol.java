package bgu.spl.net.impl.stomp;

//import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
//import bgu.spl.net.srv.connectionsImp;
//import bgu.spl.net.srv.user;

//import java.time.LocalDateTime;
//import java.util.ArrayList;

public class stompProtocol implements StompMessagingProtocol<String> {

    private boolean shouldTerminate = false;
    public Connections<String> connections;
    int connectionId;
    String userName;

    public void start(int connectionId, Connections<String> connections) {
        this.connections = connections;
        this.connectionId = connectionId;
        this.userName = null;
    }

    @Override
    public String process(String msg) {
        System.out.println("messege from client: " + msg);
        String[] msgSpliteByLines = msg.split("\n", 0);
        String stompCommand = msgSpliteByLines[0];
        System.out.println("stompCommand: " + stompCommand);
        switch (stompCommand) {
            case "CONNECT":
                connectProtocol(msgSpliteByLines, msg);
                break;
            case "SEND":
                sendProtocol(msgSpliteByLines, msg);
                break;
            case "SUBSCRIBE":
                subscribeProtocol(msgSpliteByLines, msg);
                break;
            case "UNSUBSCRIBE":
                unsubscribeProtocol(msgSpliteByLines, msg);
                break;
            case "DISCONNECT":
                disconnectProtocol(msgSpliteByLines, msg);
                break;
        }
        return null;
    }

    private void connectProtocol(String[] msgSpliteByLines, String msg) {
        System.out.println("in connectProtocol fun");
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
                userName = givenUserName;
                connections.getUser(givenUserName).loggedIn = true;
                connections.send(connectionId, "CONNECTED\nvertion:1.2\n");
                sendReceiptIfNeeded(givenReceiptId);
            } else {// wrong password
                sendError("wrong password. the correct password: " + connections.getUser(givenUserName).getPassword(), msg, givenReceiptId);
            }
        } else {// userName doesnt exist
            connections.addUser(givenUserName, givenPasscode);
            userName = givenUserName;
            connections.send(connectionId, "CONNECTED\nvertion:1.2\n");

        }
    }

    private void sendProtocol(String[] msgSpliteByLines, String msg) {
        // collecting info:
        String givenDestination = null;
        String messageToDestribute = "";
        String givenReceiptId = null;
        for (String line : msgSpliteByLines) {
            if (line.startsWith("destination:/")) {
                givenDestination = line.substring(13);
            }
            if (line.startsWith("receipt-id:")) {
                givenReceiptId = line.substring(11);
            }
            if (!line.startsWith("destination:/") & !line.equals("SEND")) {
                messageToDestribute = messageToDestribute + line + "\n";
            }
        }
        // proccesing info:
        if (userName != null) {
            if (givenDestination == null) {// if he did not speceifide a destination.
                sendError("you must add a destination!!", msg, givenReceiptId);
            } else {
                if (!connections.channelExist(givenDestination)) {// case the channel given doesnt exist
                    sendError("destination does'nt exist!\ndestination given:", msg, givenReceiptId);
                } else {
                    if (!connections.getUser(userName).userSubToChann(givenDestination)) {// if the user isnt subscribed to that cannel
                        sendError("your not subscribe to the channel :" + givenDestination + ". the channels the user subscribed to are:" + connections.getUser(userName).subIdToChan.toString() , msg, givenReceiptId);
                    } else {// all good, we want to sent the messege
                        connections.send(givenDestination,
                                "MESSEGE\nsubscription:  \nmessege-id:" + connections.getFreeToUseMessegeId()
                                        + "  \ndestination:" + givenDestination + "\n\n" + messageToDestribute);
                        sendReceiptIfNeeded(givenReceiptId);
                    }
                }
            }
        } else {
            sendError("you must login first!", msg, givenReceiptId);
        }
    }

    private void subscribeProtocol(String[] msgSpliteByLines, String msg) {
        // collecting info:
        String givenDestination = null;
        String givenReceiptId = null;
        String givenSubId = null;
        for (String line : msgSpliteByLines) {
            if (line.startsWith("destination:/")) {
                givenDestination = line.substring(13);
            }
            if (line.startsWith("id:")) {
                givenSubId = line.substring(3);
            }
            if (line.startsWith("receipt-id:")) {
                givenReceiptId = line.substring(11);
            }
        }
        // proccesing info:
        if (userName != null) {

            if (givenDestination == null) {// if he did not speceifide a destination.
                sendError("you must add a destination!!", msg, givenReceiptId);
            } else {
                if (givenSubId == null) {// if he did not speceifide a subscription id
                    sendError("you must add a subscription id", msg, givenReceiptId);
                } else {// all good, we want to subscribe the client.
                    connections.addSub(userName, givenDestination, givenSubId, connectionId);
                    connections.send(connectionId, "you subscribed succesfully");
                    sendReceiptIfNeeded(givenReceiptId);
                }
            }
        } else {
            sendError("you must login first!", msg, givenReceiptId);
        }

    }

    private void unsubscribeProtocol(String[] msgSpliteByLines, String msg) {
        // collecting info:
        String givenReceiptId = null;
        String givenSubId = null;
        for (String line : msgSpliteByLines) {
            if (line.startsWith("id:")) {
                givenSubId = line.substring(3);
            }
            if (line.startsWith("receipt-id:")) {
                givenReceiptId = line.substring(11);
            }
        }
        // proccesing info:
        if (userName != null) {
            if (givenSubId == null) {// if he did not speceifide a subscribe id
                sendError("you must add the subscribe id!!", msg, givenReceiptId);
            } else {
                if (!connections.getUser(userName).hasThatSubId(givenSubId)) {// case he doesnt have that subscribe id
                    sendError("you do not have that subscribe id!", msg, givenReceiptId);
                } else {// all good. lets unsebscribe him.
                    connections.unSub(userName, givenSubId, connectionId);
                    connections.send(connectionId, "you unsebscribed succesfully");
                    sendReceiptIfNeeded(givenReceiptId);
                }
            }
        } else {
            sendError("you must login first!", msg, givenReceiptId);
        }

    }

    private void disconnectProtocol(String[] msgSpliteByLines, String msg) {
        // collecting info:
        String givenReceiptId = null;
        for (String line : msgSpliteByLines) {
            if (line.startsWith("receipt-id:")) {
                givenReceiptId = line.substring(11);
            }
        }
        // proccesing info:
        if (userName != null) {
            if (givenReceiptId == null) {// case he did not specifide a receipt number.
                sendError("you must add a receipt to a disconnect frame!", msg, givenReceiptId);
            } else {// all good. we want to unsubscribe him fro all channels and log him out.
                connections.disconnect(connectionId);
                sendReceiptIfNeeded(givenReceiptId);
                shouldTerminate = true;
            }
        } else {
            sendError("you must login first!", msg, givenReceiptId);

        }
    }

    private void sendError(String errorSpecification, String wrongArgument, String receiptNum) {
        String receiptLine = "";
        if (receiptNum != null) {
            receiptLine = "receipt-id:" + receiptNum;
        }
        String msg = String.format("ERROR\nthe problem: %s \n ----------------- \n frame recieved: %s \n ----------------\n %s", errorSpecification,
                wrongArgument, receiptLine);
        connections.send(connectionId, msg);
    }

    private void sendReceiptIfNeeded(String givenReceiptId) {
        if (givenReceiptId != null) {// if he want a receipt
            connections.send(connectionId, "RECEIPT\nreceipt-id:" + givenReceiptId + "\n");
        }
    }

    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }

}
