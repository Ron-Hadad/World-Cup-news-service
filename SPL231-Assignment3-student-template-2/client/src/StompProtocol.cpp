#include "../include/StompProtocol.h"
#include "../include/event.h"
#include <vector>
#include <map>



StompProtocol::StompProtocol(ConnectionHandler& connection) : connection(connection), terminateKeyboard(false), terminateServerResponses(false) 
, uniqueSubID(0), uniqueRecieptID(0), SubIdToChan{}, ChanToSubId{}, DisconnectId("-1")
{
}

void StompProtocol::keyboardProcess(){
    std::string messege;
    const short bufsize = 1024;
    char buf[bufsize];
    while (!terminateKeyboard) {
        std::cout <<"enter a messege: ";
        if(std::cin.getline(buf, bufsize)){ //Getting an input from the keyboard
            std::string messege(buf);
            if (terminateKeyboard) {
                continue;
            }
            std::string command = messege.substr(0, messege.find(' '));
            if(command == "login"){
            std::cout << "The client is already logged in, log out before trying again" << std::endl;
            }
            else if(command == "join"){
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
            else {
                std::cout << "Command is invalid. Enter a new command:" << std::endl;
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
    std::string location = "data/" + messege.substr(messege.find(' ') + 1);
    //std::cout << "the location given : " + location << std::endl;
    std::ifstream ifile;
    ifile.open(location);
    if (!ifile) {
        cout << "File doesn't exist" << endl;
    }
    names_and_events events = parseEventsFile(location);
    std::string game_name = events.team_a_name + "_" + events.team_b_name;
    std::string frame = "SEND\ndestination:/" + game_name + "\n";
    frame += "user:" + connection.getLogedInUser() + "\n";
    for (Event evn : events.events) {
        frame += "team a:" + evn.get_team_a_name() + "\n";
        frame += "team b:" + evn.get_team_b_name() + "\n";
        frame += "event name:" + evn.get_name() + "\n";
        frame += "time:" + std::to_string(evn.get_time()) + "\n";
        frame += "general game updates:\n";
        for (auto& stat : evn.get_game_updates()) {
            frame += stat.first +":";
            frame += stat.second + "\n";
        }
        frame += "team a updates:\n";
        for (auto& stat : evn.get_team_a_updates()) {
            frame += stat.first +":";
            frame += stat.second + "\n";
        }
        frame += "team b updates:\n";
        for (auto& stat : evn.get_team_b_updates()) {
            frame += stat.first +":";
            frame += stat.second + "\n";
        }
        frame += "decription:\n" + evn.get_discription();
        
        //connection.addReport(connection.getLoginedUser(), game_name, evn); //
    }
    return frame;
}
 
std::string StompProtocol::SubscribeFrame(std::string messege){
    std::string frame = "SUBSCRIBE\ndestination:/";
    std::string gameName = messege.substr(messege.find(' ') + 1);
    frame += gameName + "\nid:" + StompProtocol::getuniqueSubID() + "\n\n\0";
    ChanToSubId[gameName] =  StompProtocol::getuniqueSubID();
    SubIdToChan[StompProtocol::getuniqueSubID()] = gameName;
    uniqueSubID++;
    return frame;
}

std::string StompProtocol::UnsubscribeFrame(std::string messege){
    std::string frame = "UNSUBSCRIBE\n";
    std::string gameName = messege.substr(messege.find(' ') + 1);
    frame += "id:" + ChanToSubId[gameName] +"\n";
    frame += "reciept:" + getuniqueRecieptID() +"\n\n\0";
    uniqueRecieptID++;
    return frame;
}

std::string StompProtocol::DisconnectFrame(std::string messege){
    std::string frame = "DISCONNECT\nreceipt-id:" + StompProtocol::getuniqueRecieptID() + "\n\n\0";
    DisconnectId = StompProtocol::getuniqueRecieptID();
    uniqueRecieptID++;
    terminateKeyboard = true;
    return frame;
}   

std::string StompProtocol::PrintSummary(std::string messege){
    // summary {game_name} {user} {file}
    std::vector<std::string> answer;
    //std::cout<< "inside PrintSummary a" << endl;
    std::vector<std::string> messegeParts = split(messege, " ");
    //std::cout<< "inside PrintSummary b" << endl;
    std::string game_name = messegeParts[1];
    //std::cout<< "inside PrintSummary b/1" << endl;
    int indexOfSep = game_name.find("_");
    //std::cout<< "inside PrintSummary b/2" << endl;
    std::string team_a_name = game_name.substr(0,indexOfSep);
    //std::cout<< "inside PrintSummary b/3" << endl;
    std::string team_b_name = game_name.substr(indexOfSep + 1);
    //std::cout<< "inside PrintSummary b/4" << endl;
    std::string user = messegeParts[2];
    //std::cout<< "inside PrintSummary b/5" + user << endl;
    std::string filePath = messegeParts[3];
    //std::cout<< "inside PrintSummary c" << endl;
    std::vector<Event> reports = connection.getReportsByUser(user, game_name);
    //std::cout<< "inside PrintSummary d" << endl;
    std::fstream file;
    //mesharsherim the data:
    std::string data = team_a_name + " vs " + team_b_name + "\n";
    data += "Game stats:\n";
    data += "General stats:\n";
    for(unsigned int i = 0; i < reports.size(); i++){
        map<std::string, std::string> game_updates = reports[i].get_game_updates();
        for(auto const& it : game_updates){
            data += it.first + ":" + it.second + "\n";
        }  
    }
    data += team_a_name + "stats:\n";
    for(unsigned int i = 0; i < reports.size(); i++){
        map<std::string, std::string> team_a_updates = reports[i].get_team_a_updates();
        for(auto const& it : team_a_updates){
            data += it.first + ":" + it.second + "\n";
        }  
    }
    data += team_b_name + "stats:\n";
    for(unsigned int i = 0; i < reports.size(); i++){
        map<std::string, std::string> team_b_updates = reports[i].get_team_b_updates();
        for(auto const& it : team_b_updates){
            data += it.first + ":" + it.second + "\n";
        }  
    }
    data += team_a_name + "game event reports:\n";
    for(unsigned int i = 0; i < reports.size(); i++){
        int time = reports[i].get_time();
        std::string name = reports[i].get_name();
        std::string description = reports[i].get_discription();
        data += time + "-" + name + "\n";
        data += description + "\n";
    }
    write_to_file(filePath, data);
    cout << file.rdbuf(); // suppused to print all the file in the end
    return data;
}

//gets a file path and data to write, if the file exist writes the data over it, if not, create it and than write it.
void StompProtocol::write_to_file(const std::string& file_name, const std::string& data) {
    std::fstream file;
    file.open(file_name, std::ios::out | std::ios::trunc);
    if (!file) {
        std::cout << "File does not exist. Creating file.." << std::endl;
        file.open(file_name, std::ios::out | std::ios::trunc);
    }
    file << data << std::endl;
    file.close();
}


void StompProtocol::serverProcess(){
    while (!terminateServerResponses) {
        std::string responseFrame;
        bool Answered = connection.getFrame(responseFrame); 
        std::cout << Answered << std::endl;
        if (!Answered) { //If the server connection was closed and no response receieved - close the client
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            terminateKeyboard = true;
            break;
        }
        else{
            //to see the server responce:
            std::cout << "the frame recieved from server: " << std::endl;
            std::cout << responseFrame << std::endl;
            std::string command = responseFrame.substr(0, responseFrame.find('\n'));
            if(command == "RECEIPT"){
                if(responseFrame.substr(responseFrame.find(":") + 1) == DisconnectId){
                    std::cout << "bye bye" << std::endl;
                    terminateServerResponses = true;
                    connection.setLogedInUser(false);
                    connection.close();
                }
            }
            else if(command == "CONNECTED"){
                std::cout <<"Login Succesfully" << std::endl;
            }

            else if(command == "MESSEGE"){
                //std::cout << "we got here a" << std::endl;
                vector<std::string> lines = split(responseFrame,"\n");
                //std::cout << "we got here b" << std::endl;
                Event newEvent = createEvent(lines);
                //std::cout << "we got here c" << std::endl;
                std::string userNameReported = newEvent.get_userName();
                //std::cout << "we got here d" << std::endl;
                std::string teamAName = newEvent.get_team_a_name();
                std::string teamBName = newEvent.get_team_b_name();
                //std::cout << "we got here e" << std::endl;
                std::string gameName = teamAName+"_"+teamBName;
                connection.addReport(userNameReported, gameName, newEvent);
                //std::cout << "we got here f" << std::endl;

                //need to print the messege to the user
                //std::cout <<responseFrame << std::endl;
            }
            else if(command == "ERROR"){
                //std::cout <<responseFrame << std::endl;
                terminateKeyboard = true;
                terminateServerResponses = true;
            }
        }
    }
}

Event StompProtocol::createEvent(vector<std::string> lines){
    std::string userName;
    std::string team_a_name;
    std::string team_b_name;
    std::string name;
    int time;
    std::map<std::string, std::string> game_updates;
    std::map<std::string, std::string> team_a_updates;
    std::map<std::string, std::string> team_b_updates;
    std::string description;
    for(unsigned i = 0; i < lines.size(); i++){
        if(lines[i].find("user:") != std::string::npos){
            userName = lines[i].substr(5);
        }
        if(lines[i].find("team a:") != std::string::npos){
            team_a_name = lines[i].substr(7);
        }
        if(lines[i].find("team b:") != std::string::npos){
            team_b_name = lines[i].substr(7);
        }
        if(lines[i].find("event name:") != std::string::npos){
            name = lines[i].substr(11);
        }
        if(lines[i].find("time:") != std::string::npos){
            time = stoi(lines[i].substr(5));
        }        
        if(lines[i].find("general game updates:") != std::string::npos){
            while((lines[i + 1] != "team a updates:") & (lines[i + 1].find(":") != std::string::npos)){
                int lineSplitIndex = lines[i + 1].find(":");
                std::string header = lines[i + 1].substr(0,lineSplitIndex);
                std::string restOfLine = lines[i + 1].substr(lineSplitIndex + 1);
                game_updates.insert({header, restOfLine});
                i++;
            }
        }
        
        if(lines[i].find("team a updates:") != std::string::npos){ 
            while((lines[i + 1] != "team b updates:") & (lines[i + 1].find(":") != std::string::npos)){
                int lineSplitIndex = lines[i + 1].find(":");
                std::string header = lines[i + 1].substr(0,lineSplitIndex);
                std::string restOfLine = lines[i + 1].substr(lineSplitIndex + 1);
                team_a_updates.insert({header, restOfLine});
                i++;
            }
        }   
        if(lines[i].find("team b updates:") != std::string::npos){ 
            while((lines[i + 1] != "description:") & (lines[i + 1].find(":") != std::string::npos)){
                int lineSplitIndex = lines[i + 1].find(":");
                std::string header = lines[i + 1].substr(0,lineSplitIndex);
                std::string restOfLine = lines[i + 1].substr(lineSplitIndex + 1);
                team_b_updates.insert({header, restOfLine});
                i++;
            }
        }   
        if(lines[i].find("description:") != std::string::npos){
            description =  lines[i + 1];
        }   
    }
    Event newEvent(userName, team_a_name, team_b_name, name, time, game_updates,team_a_updates,team_b_updates, description);
    return newEvent;
}

vector<std::string> StompProtocol::split(std::string msg, std::string delimiter)
{
    int indexStart = 0;
    int indexEnd = msg.find(delimiter);
    std::vector <std::string> lines;
    //std::cout << "inside split a" << std::endl;
    msg = msg + " ";
    while ((unsigned)indexEnd < msg.length()) {
        std::cout << "inside the while loop a" << std::endl;
        lines.push_back(msg.substr(indexStart, indexEnd - indexStart));
        //std::cout << "inside the while loop b" << std::endl;
        indexStart = indexEnd + delimiter.length();
        //std::cout << "inside the while loop c" << std::endl;
        indexEnd = msg.find(delimiter, indexStart);
        std::cout << "inside the while loop d" << std::endl;
    }
    std::cout << "inside split b" << std::endl;
    //std::cout << lines[0] << std::endl;
    //std::cout << "inside split c" << std::endl;
    return lines;
}




