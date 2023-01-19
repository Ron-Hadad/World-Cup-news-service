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
	std::string frame = "CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\n" ;
    size_t pos1 = Messege.find(" ", Messege.find(" ") + 1);
    // Find the position of the fourth word
    size_t pos2 = Messege.find(" ", pos1 + 1);
    // Extract the third word
    std::string currentUser = Messege.substr(pos1 + 1, pos2 - pos1 - 1);
    // Extract the fourth word
    std::string password = Messege.substr(pos2 + 1);

    frame+= "login: " + currentUser + "\n";
    frame+= "passcode:" +  password +"\n\n" + "\0";

	connectionHandler.connectUser(currentUser);
    //std::cout << "messege sent to the server: " + frame << std::endl;
    connectionHandler.sendFrame(frame);
    StompProtocol protocol(connectionHandler);
    std::thread serverThread(&StompProtocol::serverProcess, &protocol); 
    std::thread keyboardThread(&StompProtocol::keyboardProcess, &protocol); 
    serverThread.join();
    keyboardThread.join();
	return 0;
}








	



		
        
    
	
