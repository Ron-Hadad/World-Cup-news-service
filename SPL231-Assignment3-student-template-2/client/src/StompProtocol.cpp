#include "../include/StompProtocol.h"
#include "../include/event.h"
#include <vector>
#include <map>


StompProtocol::StompProtocol(ConnectionHandler& connection) : connection(connection), terminateKeyboard(false), terminateServerResponses(false) 
, uniqueSubID(0), uniqueRecieptID(0), SubIdToChan{}, ChanToSubId{}, DisconnectId("-1")
{
}

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
            else {
                std::cout << "Command is invalid. Enter a new command" << std::endl;
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
    std::string frame = "SEND\ndestination:/" + game_name;
    frame += "user:" + connection.getLogedInUser();
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
        //connection.addReport(connection.getLoginedUser(), game_name, evn); //--> need to add an update to the reports
    }
}
 
std::string StompProtocol::SubscribeFrame(std::string messege){
    std::string frame = "SUBSCRIBE\ndestination:/";
    std::string gameName = messege.substr(messege.find(' ') + 1);
    frame += gameName + "id:" + StompProtocol::getuniqueSubID() + "\n\n\0";
    ChanToSubId[gameName] =  StompProtocol::getuniqueSubID();
    SubIdToChan[StompProtocol::getuniqueSubID()] = gameName;
    uniqueSubID++;
    return frame;
}

std::string StompProtocol::UnsubscribeFrame(std::string messege){
    std::string frame = "UBSUBSCRIBE\n/";
    std::string gameName = messege.substr(messege.find(' ') + 1);
    frame += "id:" + ChanToSubId[gameName];
    frame += "reciept:" + getuniqueRecieptID() +"\n\n\0";
    uniqueRecieptID++;
    return frame;
}

std::string StompProtocol::DisconnectFrame(std::string messege){
    std::string frame = "DISCONNECT\nreciept:/" + StompProtocol::getuniqueRecieptID() + "\n\n\0";
    DisconnectId = StompProtocol::getuniqueRecieptID();
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
                vector<std::string> lines = split(responseFrame);
                Event newEvent = createEvent(lines);
                std::string userNameReported = newEvent.get_userName();
                std::string teamAName = newEvent.get_team_a_name();
                std::string teamBName = newEvent.get_team_b_name();
                std::string gameName = teamAName+"_"+teamBName;
                connection.addReport(userNameReported, gameName, newEvent);

            }



            else if(command == "ERROR"){
                std::cout <<responseFrame << std::endl;
                terminateKeyboard = true;
                terminateServerResponses = true;
            }

        }
    }


}

Event createEvent(vector<std::string> lines){
    std::string userName;
    std::string team_a_name;
    std::string team_b_name;
    std::string name;
    int time;
    std::map<std::string, std::string> game_updates;
    std::map<std::string, std::string> team_a_updates;
    std::map<std::string, std::string> team_b_updates;
    std::string description;
    for(int i = 0; i < lines.size(); i++){
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
        if(lines[i].find("team a updates:") != std::string::npos){ 
            while(lines[i + 1] != "team b updates:" & lines[i + 1].find(":")){
                int lineSplitIndex = lines[i + 1].find(":");
                std::string header = lines[i + 1].substr(0,lineSplitIndex);
                std::string restOfLine = lines[i + 1].substr(lineSplitIndex + 1);
                team_a_updates.insert({header, restOfLine});
                i++;
            }
        }   
        if(lines[i].find("team b updates:") != std::string::npos){ 
            while(lines[i + 1] != "description:" & lines[i + 1].find(":")){
                int lineSplitIndex = lines[i + 1].find(":");
                std::string header = lines[i + 1].substr(0,lineSplitIndex);
                std::string restOfLine = lines[i + 1].substr(lineSplitIndex + 1);
                team_a_updates.insert({header, restOfLine});
                i++;
            }
            
        }   
        if(lines[i].find("description:") != std::string::npos){
            description =  lines[i + 1];
        }   
        

    }
    int time;
    Event newEvent(userName, team_a_name, team_b_name, name, time, game_updates,team_a_updates,team_b_updates, description);
    return newEvent;
}

vector<std::string> StompProtocol::split(std::string msg)
{
    int indexStart = 0;
    std::string delimiter = "\n";
    int indexEnd = msg.find(delimiter);
    std::vector <std::string> lines;
    while (indexEnd != std::string::npos) {
        lines.push_back(msg.substr(indexStart, indexEnd - indexStart));
        indexStart = indexEnd + delimiter.length();
        indexEnd = msg.find(delimiter, indexStart);
    }
    return lines;
}




