#pragma once

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <map>
//#include <game.h>

using boost::asio::ip::tcp;
using namespace std;
class game;
class Event;

class ConnectionHandler {
private:
	const std::string host_;
	const short port_;
	std::string currentUser;
	bool logedIn;
	boost::asio::io_service io_service_;   // Provides core I/O functionality
	tcp::socket socket_;
	map<string, game>msgRecordsPerUser;

public:
	ConnectionHandler(std::string host, short port);

	virtual ~ConnectionHandler();

	// Connect to the remote machine
	bool connect();

	void connectUser(std::string user);

	std::string getLogedInUser();

	void setLogedInUser(bool stat);

	// Read a fixed number of bytes from the server - blocking.
	// Returns false in case the connection is closed before bytesToRead bytes can be read.
	bool getBytes(char bytes[], unsigned int bytesToRead);

	// Send a fixed number of bytes from the client - blocking.
	// Returns false in case the connection is closed before all the data is sent.
	bool sendBytes(const char bytes[], int bytesToWrite);

	// Read an ascii line from the server
	// Returns false in case connection closed before a newline can be read.
	bool getLine(std::string &line);

	// Send an ascii line from the server
	// Returns false in case connection closed before all the data is sent.
	bool sendLine(std::string &line);

	// Read an ascii line from the server
	// Returns false in case connection closed before a newline can be read.
	bool getFrame(std::string & frame);

	// Send an ascii line from the server
	// Returns false in case connection closed before all the data is sent.
	bool sendFrame(std::string & frame);

	// Get Ascii data from the server until the delimiter character
	// Returns false in case connection closed before null can be read.
	bool getFrameAscii(std::string &frame, char delimiter);

	// Send a message to the remote host.
	// Returns false in case connection is closed before all the data is sent.
	bool sendFrameAscii(const std::string &frame, char delimiter);

	// Close down the connection properly.
	void close();

	void addReport(string user, string game_name, Event report);

	vector<Event> getReportsByUser(string user, string game_name);
}; //class ConnectionHandler
