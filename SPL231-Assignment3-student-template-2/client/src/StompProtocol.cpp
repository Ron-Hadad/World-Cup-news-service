#include "../include/StompProtocol.h"
#include "../include/event.h"


StompProtocol::StompProtocol(ConnectionHandler& connection) : connection(connection), terminateKeyboard(false), terminateServerResponses(false) 
, uniqueSubID(0), uniqueRecieptID(0)
{
}

//StompProtocol::~StompProtocol(){}

void StompProtocol::keyboardProcess(std::string messege){
    const short bufsize = 1024;
    char buf[bufsize];
    while (!terminateKeyboard) {
        if(std::cin.getline(buf, bufsize)){ //Getting an input from the keyboard
            std::string messege(buf);
            if (terminateKeyboard) {
                continue;
            }
            std::string command = messege.substr(0, messege.find(' '));
            if(command == "login"){
            std::cout << "The client is already logged in, log out before trying again" << std::endl << std::endl;
            }
            if(command == "join"){
                std::string frame = SubscribeFrame(messege);
                connection.sendFrame(frame);
            }
            else if(command == "exit"){
                std::string frame = UnsubscribeFrame(messege);
                connection.sendFrame(frame);
            }
            else if(command == "report"){
                std::string frame = SendFrame(messege);
                connection.sendFrame(frame);
            }
            else if(command == "summary"){
                PrintSummary(messege);
            }
            else if(command == "logout"){
                std::string frame = DisconnectFrame(messege);
                connection.sendFrame(frame);
            }
        }
    }
}

std::string StompProtocol::getuniqueSubID(){
    return std::to_string(uniqueSubID);
}

std::string StompProtocol::getuniqueRecieptID(){
    return std::to_string(uniqueRecieptID);
}

std::string StompProtocol::SendFrame(std::string messege){
    //recieve : report {file}
    //need to: (1)read the file
    //(2) Save each event on the client as a game update reported by the current logged-in user
    //(3) SEND frame 
    std::string location = "/data" + messege.substr(messege.find(' ') + 1);
    std::ifstream ifile;
    ifile.open(location);
    names_and_events events = parseEventsFile(location);
    std::string game_name = events.team_a_name + "_" + events.team_b_name;
    std::string frame = "SEND\ndestination:" + game_name;
    //frame+="user:" + connection.currentUser--> need to save the current user somewhere
    for (Event evn : events.events) {
        frame += "team a:" + evn.get_team_a_name() + "\n";
        frame += "team b:" + evn.get_team_b_name() + "\n";
        frame += "event name:" + evn.get_name() + "\n";
        frame += "time:" + std::to_string(evn.get_time()) + "\n";
        frame += "general game updates:\n";
        for (auto& stat : evn.get_game_updates()) {
            frame += stat.first +":";
            frame += stat.second; + "\n";
        }
        frame += "team a updates:\n";
        for (auto& stat : evn.get_team_a_updates()) {
            frame += stat.first +":";
            frame += stat.second; + "\n";
        }
        frame += "team b updates:\n";
        for (auto& stat : evn.get_team_b_updates()) {
            frame += stat.first +":";
            frame += stat.second; + "\n";
        }
        frame += "decription:\n" + evn.get_discription();
        //connection.addReport(connection.getLoginedUser(), game_name, e); //--> need to add an update to the reports
    }
}
 
std::string StompProtocol::SubscribeFrame(std::string messege){
    std::string frame = "SUBSCRIBE\ndestination:/";
    frame += messege.substr(messege.find(' ') + 1) + "id:" + StompProtocol::getuniqueSubID() + "\n\n\0";
    uniqueSubID++;
    return frame;
}

std::string StompProtocol::UnsubscribeFrame(std::string messege){
    std::string frame = "UBSUBSCRIBE\n/";
    //need to understand where to get the user id
    //need to understand how and where the reciept is creating and what is the reciept id
}

std::string StompProtocol::DisconnectFrame(std::string messege){
    std::string frame = "DISCONNECT\nreciept:/" + StompProtocol::getuniqueRecieptID() + "\n\n\0";
    uniqueRecieptID++;
    terminateKeyboard = true;
    return frame;
}   

std::string StompProtocol::PrintSummary(std::string messege){
    std::string s = messege;
    std::string delimiter = " ";
    std::vector<std::string> messegeParts;
    int index = 0;
    while ((index = s.find(delimiter))!= std::string::npos) {
        messegeParts.push_back(s.substr(0, index));
        s.erase(0, index + delimiter.length());
    }
    std::string game_name = messegeParts[1];
    std::string user = messegeParts[2];
    std::string file = messegeParts[3];
    // vector<Event> reports = connection.getReportsByUser(user, game_name);
    // if (!reports.empty()) {
    //     std::string team_a_name = reports.at(0).get_team_a_name();
    //     std::string team_b_name = reports.at(0).get_team_b_name();
    //     names_and_events object(team_a_name, team_b_name, reports);
    //     parseEventsToFile(object, file);
    //     std::cout << "Created a file" << endl;
    // }
    // else {
    //     std::cout << "There are no reports from that user" << endl;
    // }
}

void StompProtocol::serverProcess(){
    while (!terminateServerResponses) {
        std::string responseFrame;
        bool Answered = connection.getFrame(responseFrame); 
        if (!Answered) { //If the server connection was closed and no response receieved - close the client
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            terminateKeyboard = true;
            break;
        }
        else{
            std::string command = responseFrame.substr(0, responseFrame.find(' '));
            if(command == "RECEIPT"){
                // map<string, string> headers = frame.getHeaders();
                // std::string receiptID = headers.find("receipt-id")->second;
                // std::string receiptCOMMAND = connection.findReceiptCommand(receiptID);
                // if (receiptCOMMAND == "DISCONNECT") { //If the receipt received is for a DISCONNECT frame - close the connection
                //     std::cout << "bye bye" << std::endl;
                //     terminateServerResponses = true;
                //     connection.close();
                // }

            }
            else if(command == "MESSEGE"){
                //need to parse the meesege ,save it and the report 


                // string report = frame.getBody();
                // vector<string> lines = split(report, '\n');
                // string userLine = lines[0];
                // int index = userLine.find(':') + 1;
                // string user = userLine.substr(index);
                // Event object = parseEventReport(report);
                // string game_name = object.get_team_a_name();
                // game_name.append("_");
                // game_name.append(object.get_team_b_name());
                // connection.addReport(user, game_name, object);
            }
            else if(command == "ERROR"){
                terminateKeyboard = true;
                terminateServerResponses = true;
            }

        }
    }
}




