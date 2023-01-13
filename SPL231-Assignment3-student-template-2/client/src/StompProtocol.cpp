#pragma once
#include "../include/StompProtocol.h"




//StompProtocol::StompProtocol(){}

//StompProtocol::~StompProtocol(){}

std::string StompProtocol::process(std::string messege){
    std::string command = messege.substr(0, messege.find(' '));
    if(command == "login"){
        //check if the clinet is already loged in- if yes print "The client is already logged in, log out before trying again"

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
    else if(command == "exit"){
        return DisconnectFrame(messege);
    }
}








std::string ConnectFrame(std::string messege){
    std::string frame = "CONNECT/naccept-version : 1.2/nhost: stomp.cs.bgu.ac.il/n" ;
    int usernameIndex = messege.find(' ', messege.find('host:'));
    int passwordIndex = messege.find(' ', usernameIndex + 1);
    frame+= "login: " + messege.substr(usernameIndex,passwordIndex) + "/n";
    frame+= "passcode: " +  messege.substr(passwordIndex) +"/n/n" + "/0";
    return frame;
}

std::string SendFrame(std::string messege){

}

std::string SubscribeFrame(std::string messege){

}

std::string UnsubscribeFrame(std::string messege){

}

std::string DisconnectFrame(std::string messege){

}

std::string PrintSummary(){

}




