#include "../include/ConnectionHandler.h"
#include "../include/game.h"
#include "../include/event.h"

using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

ConnectionHandler::ConnectionHandler(std::string host, short port) : host_(host), port_(port),currentUser(), logedIn(false),
io_service_(), socket_(io_service_), msgRecordsPerUser() {}

ConnectionHandler::~ConnectionHandler() {
	close();
}

std::string ConnectionHandler::getLogedInUser()
{
	return currentUser;
}

void ConnectionHandler::connectUser(std::string user)
{
	currentUser = user;
	logedIn = true;
}

bool ConnectionHandler::connect() {
	std::cout << "Starting connect to "
	          << host_ << ":" << port_ << std::endl;
	try {
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
	}
	catch (std::exception &e) {
		std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

void ConnectionHandler::setLogedInUser(bool stat){
	logedIn = stat;
}

bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
	size_t tmp = 0;
	boost::system::error_code error;
	try {
		while (!error && bytesToRead > tmp) {
			tmp += socket_.read_some(boost::asio::buffer(bytes + tmp, bytesToRead - tmp), error);
		}
		if (error)
			throw boost::system::system_error(error);
	} catch (std::exception &e) {
		std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
	int tmp = 0;
	boost::system::error_code error;
	try {
		while (!error && bytesToWrite > tmp) {
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
		}
		if (error)
			throw boost::system::system_error(error);
	} catch (std::exception &e) {
		std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

bool ConnectionHandler::getLine(std::string &line) {
	return getFrameAscii(line, '\n');
}

bool ConnectionHandler::sendLine(std::string &line) {
	return sendFrameAscii(line, '\n');
}


bool ConnectionHandler::getFrameAscii(std::string &frame, char delimiter) {
	char ch;
	// Stop when we encounter the null character.
	// Notice that the null character is not appended to the frame string.
	try {
		do {
			if (!getBytes(&ch, 1)) {
				return false;
			}
			if (ch != '\0')
				frame.append(1, ch);
		} while (delimiter != ch);
	} catch (std::exception &e) {
		std::cerr << "recv failed2 (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

bool ConnectionHandler::sendFrameAscii(const std::string &frame, char delimiter) {
	bool result = sendBytes(frame.c_str(), frame.length());
	if (!result) return false;
	return sendBytes(&delimiter, 1);
}

bool ConnectionHandler::getFrame(std::string &frame) {
	return getFrameAscii(frame, '\0');
}

bool ConnectionHandler::sendFrame(std::string & frame) {
	std::cout << "messege sent to the server: " + frame << std::endl;
	return sendFrameAscii(frame, '\0');
}

// Close down the connection properly.
void ConnectionHandler::close() {
	try {
		socket_.close();
	} catch (...) {
		std::cout << "closing failed: connection already closed" << std::endl;
	}
}

void ConnectionHandler::addReport(string user, string game_name, Event report)
{
	if (msgRecordsPerUser.find(game_name) == msgRecordsPerUser.end()) {
		game game(game_name);
		game.addEvent(user, report);
		msgRecordsPerUser.insert({ game_name, game });
	}
	else {
		msgRecordsPerUser.find(game_name)->second.addEvent(user, report);
	}
}

vector<Event> ConnectionHandler::getReportsByUser(string user, string game_name)
{
	if (msgRecordsPerUser.find(game_name) == msgRecordsPerUser.end()) {
		return vector<Event>();
	}
	else {
		return msgRecordsPerUser.find(game_name)->second.getEvents_ByUser(user);
	}
}
