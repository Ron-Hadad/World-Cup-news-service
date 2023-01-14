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
	bool connected = false;
	if(!connected){
		 if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
	}
	else{
		std::cout <<"The client is already logged in, log out before trying again";
	}
	}
	while(1){
		//need to understand how to do the loop correctly, how to connect to the socket and to the keyboard
		std::thread keyboardThread;
		//std::thread serverThread; //meybe the main thread instead
		
		keyboardThread.join();
		//serverThread.join();
	}
	return 0;
}
	



		
        
    
	
