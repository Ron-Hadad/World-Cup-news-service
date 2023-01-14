#pragma once
#include "../include/StompProtocol.h"


StompProtocol::StompProtocol() {
    uniqueSubID = 0;
    uniqueRecieptID = 0;
    }

//StompProtocol::~StompProtocol(){}

std::string StompProtocol::process(std::string messege){
    std::string command = messege.substr(0, messege.find(' '));
    if(command == "login"){
        return ConnectFrame(messege);
    }
    else if(command == "join"){
        return SubscribeFrame(messege);
    }
    else if(command == "exit"){
        return UnsubscribeFrame(messege);
    }
    else if(command == "report"){
        //can be report {file} or report events1_partial.json
        return SendFrame(messege);
    }
    else if(command == "summary"){
        PrintSummary();
    }
    else if(command == "logout"){
        return DisconnectFrame(messege);
    }
}

std::string StompProtocol::getuniqueSubID(){
    return std::to_string(uniqueSubID);
}

std::string StompProtocol::getuniqueRecieptID(){
    return std::to_string(uniqueRecieptID);
}


std::string StompProtocol::ConnectFrame(std::string messege){
    std::string frame = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\n" ;
    int usernameIndex = messege.find(' ', messege.find('host:'));
    int passwordIndex = messege.find(' ', usernameIndex + 1);
    frame+= "login: " + messege.substr(usernameIndex + 1,passwordIndex + 1) + "\n";
    frame+= "passcode: " +  messege.substr(passwordIndex) +"\n\n" + "\0";
    return frame;
}

std::string StompProtocol::SendFrame(std::string messege){
    //recieve : report {file}
    //need to: (1)read the file
    //(2) Save each event on the client as a game update reported by the current logged-in user
    //(3) SEND frame 
}

std::string StompProtocol::SubscribeFrame(std::string messege){
    std::string frame = "SUBSCRIBE\ndestination:/";
    frame += messege.substr(messege.find(' ') + 1) + "id:" + StompProtocol::getuniqueSubID() + "\n\n\0";
    uniqueSubID++;
    return frame;
}

std::string StompProtocol::UnsubscribeFrame(std::string messege){

}

std::string StompProtocol::DisconnectFrame(std::string messege){
    std::string frame = "DISCONNECT\nreciept:/" + StompProtocol::getuniqueRecieptID() + "\n\n\0";
    uniqueRecieptID++;
    return frame;
}   

std::string StompProtocol::PrintSummary(){

}




