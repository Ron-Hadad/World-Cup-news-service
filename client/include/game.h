#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "../include/ConnectionHandler.h"
#include "../include/event.h"

using namespace std;

/* This class represents a game (channel) & saves data of reports receieved from each player about this game */
class game
{
private:
	string game_name;
	map<string, vector<Event>> events;
	map<string, map<string, string>> generalUpdates; //map<user, map<stat_name, stat_val>>
	map<string, map<string, string>> team_aUpdates; //map<user, map<stat_name, stat_val>>
	map<string, map<string, string>> team_bUpdates; //map<user, map<stat_name, stat_val>>

public:
	game(string game_name);
	virtual ~game();
	void addEvent(string user, Event event);
	vector<Event> getEvents_ByUser(string user);
	string get_name();
	void add_general_stats(string user, Event event); //Adding General Game Stats from the Event object
	void add_team_a_stats(string user, Event event); //Adding Team a Game Stats from the Event object
	void add_team_b_stats(string user, Event event); //Adding Team b Game Stats from the Event object
	map<string, string> get_general_stats(string user); //Getting General Game Stats from the Event object
	map<string, string> get_team_a_stats(string user); //Getting Team a Game Stats from the Event object
	map<string, string> get_team_b_stats(string user); //Getting Team b Game Stats from the Event object


};