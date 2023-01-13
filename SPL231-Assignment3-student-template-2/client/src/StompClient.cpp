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
	return 0;

	while(1){
		// Messege= nextmessege
		// std::thread keyboardThread;
		// std::thread serverThread;
		// keyboardThread.join();
		// serverThread.join();
	}
	
	return 0;
}




		
        
    
	
