#pragma once
#include <string>
#include "../include/ConnectionHandler.h"

// TODO: implement the STOMP protocol
class StompProtocol
{
private:

    //creating a connect frame to send to the server
    std::string ConnectFrame(std::string messege);

    //creating a send frame to send to the server
    std::string SendFrame(std::string messege);

    //creating a subscribe frame to send to the server
    std::string SubscribeFrame(std::string messege);

    //creating a unsubscribe frame to send to the server
    std::string UnsubscribeFrame(std::string messege);

    //creating a disconnect frame to send to the server
    std::string DisconnectFrame(std::string messege);

    //printing a summary of game events
    std::string PrintSummary();

public:

    //recieves command from the keyboard
    //for each command sends a frame to the server
    std::string process(std::string messege);
    
};
