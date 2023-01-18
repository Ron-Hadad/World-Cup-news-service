#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../include/ConnectionHandler.h"
#include <map>
#include <mutex>

// TODO: implement the STOMP protocol
class StompProtocol
{
private:

    ConnectionHandler &connection;
    bool terminateKeyboard;
	bool terminateServerResponses;
    int uniqueSubID;
    int uniqueRecieptID;
    std::map <std::string,std::string> SubIdToChan;
    std::map <std::string,std::string> ChanToSubId;
    std::string DisconnectId;
    
    //creating a send frame to send to the server
    std::string SendFrame(std::string messege);

    //creating a subscribe frame to send to the server
    std::string SubscribeFrame(std::string messege);

    //creating a unsubscribe frame to send to the server
    std::string UnsubscribeFrame(std::string messege);

    //creating a disconnect frame to send to the server
    std::string DisconnectFrame(std::string messege);

    //printing a summary of game events
    std::string PrintSummary(std::string messege);

public:

    StompProtocol(ConnectionHandler& connection);

    //recieves command from the keyboard
    //for each command sends a frame to the server
    void keyboardProcess();

    void write_to_file(const std::string& file_name, const std::string& data);

	void serverProcess();

    std::string getuniqueSubID();

    std::string getuniqueRecieptID();

    vector<std::string> split(std::string msg, std::string delimiter);

    Event createEvent(vector<std::string> lines);
    
};
