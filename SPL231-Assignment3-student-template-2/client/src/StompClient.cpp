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

	while(!Messege.find("login")){
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
    int usernameIndex = Messege.find(' ', Messege.find('host:'));
    int passwordIndex = Messege.find(' ', usernameIndex + 1);
    frame+= "login: " + Messege.substr(usernameIndex + 1,passwordIndex + 1) + "\n";
    frame+= "passcode: " +  Messege.substr(passwordIndex) +"\n\n" + "\0";

	//connectionHandler.connectUser(Messege.substr(usernameIndex + 1,passwordIndex + 1)); // need to see how to connect user

    connectionHandler.sendFrame(frame);
	try {
        StompProtocol protocol(connectionHandler);
        std::thread serverThread(&StompProtocol::serverProcess, &protocol); 
        std::thread keyboardThread(&StompProtocol::keyboardProcess, &protocol); 
        serverThread.join();
        keyboardThread.join();
    }
	//catch(exception) {
    //     std::cout << "An error received, disconnecting.." << std::endl;
    // }
	return 0;
}
	



		
        
    
	
