#include <stdlib.h>
#include <thread>
#include "../include/ConnectionHandler.h"
#include "../include/StompProtocol.h"

int main(int argc, char *argv[]) {
	// TODO: implement the STOMP client
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }

	//log in	
	std::string Messege;
	std::cout <<"enter a messege: ";
	std::getline(std::cin, Messege);

	while(Messege.substr(0, Messege.find(" ")) != "login"){
		std::cout <<"enter again: ";
		std::getline(std::cin, Messege);
	}

	std::string host= Messege.substr(6,Messege.find(':')-6);
	short port =  stoi(Messege.substr(Messege.find(':') + 1,Messege.find(' ')));
    
    ConnectionHandler connectionHandler(host, port);
	if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    //std::vector<std::string> MessegeParts = split(Messege, " ");
        //vector<std::string> split(std::string msg, std::string delimiter){
    int indexStart = 0;
    unsigned indexEnd = Messege.find(" ");
    std::vector <std::string> MessegeParts;
    while (indexEnd != std::string::npos) {
        MessegeParts.push_back(Messege.substr(indexStart, indexEnd - indexStart));
        indexStart = indexEnd + 1;
        indexEnd = Messege.find(" ", indexStart);
    }
    // return lines;
    // }
	std::string frame = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\n" ;
    // int usernameIndex = Messege.find_first_of("7777") + 4;
    // int passwordIndex = Messege.find_last_of(" ");
	// std::string currentUser = Messege.substr(usernameIndex + 1, passwordIndex - usernameIndex);
    std::string currentUser = MessegeParts[2];
    frame+= "login: " + currentUser + "\n";
    //frame+= "passcode: " +  Messege.substr(passwordIndex) +"\n\n" + "\0";
    frame+= "passcode: " + MessegeParts[3] + "\n\n";

	connectionHandler.connectUser(currentUser);
    connectionHandler.sendFrame(frame);
    StompProtocol protocol(connectionHandler);
    std::thread serverThread(&StompProtocol::serverProcess, &protocol); 
    std::thread keyboardThread(&StompProtocol::keyboardProcess, &protocol); 
    serverThread.join();
    keyboardThread.join();
	return 0;
}








	



		
        
    
	
