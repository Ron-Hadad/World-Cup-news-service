#include "../include/game.h"

game::game(string game_name): game_name(game_name), events(), generalUpdates(), team_aUpdates(), team_bUpdates()
{
}

game::~game()
{
}

void game::addEvent(string user, Event event)
{
	if (events.find(user) == events.end()) {
		vector<Event> eventsList;
		eventsList.push_back(event);
		events.insert({ user, eventsList });
	}
	else {
		events.find(user)->second.push_back(event);
	}
	add_general_stats(user, event);
	add_team_a_stats(user, event);
	add_team_b_stats(user, event);
}

vector<Event> game::getEvents_ByUser(string user)
{
	if (events.find(user) == events.end()) {
		return vector<Event>();
	}
	else {
		return events.find(user)->second;
	}
}

string game::get_name()
{
	return game_name;
}

void game::add_general_stats(string user, Event event)
{
	map<string, string> general_updates = event.get_game_updates();
	if (this->generalUpdates.find(user) == generalUpdates.end()) {
		this->generalUpdates.insert({ user, general_updates });
	}
	else {
		for (auto& update : general_updates) {
			if (this->generalUpdates.find(user)->second.find(update.first) == this->generalUpdates.find(user)->second.end()) {
				this->generalUpdates.find(user)->second.insert({ update.first, update.second });
			}
			else {
				this->generalUpdates.find(user)->second.find(update.first)->second = update.second;
			}
		}
	}
}

void game::add_team_a_stats(string user, Event event)
{
	map<string, string> team_a_updates = event.get_team_a_updates();
	if (this->team_aUpdates.find(user) == team_aUpdates.end()) {
		this->team_aUpdates.insert({ user, team_a_updates });
	}
	else {
		for (auto& update : team_a_updates) {
			if (this->team_aUpdates.find(user)->second.find(update.first) == this->team_aUpdates.find(user)->second.end()) {
				this->team_aUpdates.find(user)->second.insert({ update.first, update.second });
			}
			else {
				this->team_aUpdates.find(user)->second.find(update.first)->second = update.second;
			}
		}
	}
}

void game::add_team_b_stats(string user, Event event)
{
	map<string, string> team_b_updates = event.get_team_b_updates();
	if (this->team_bUpdates.find(user) == team_bUpdates.end()) {
		this->team_bUpdates.insert({ user, team_b_updates });
	}
	else {
		for (auto& update : team_b_updates) {
			if (this->team_bUpdates.find(user)->second.find(update.first) == this->team_bUpdates.find(user)->second.end()) {
				this->team_bUpdates.find(user)->second.insert({ update.first, update.second });
			}
			else {
				this->team_bUpdates.find(user)->second.find(update.first)->second = update.second;
			}
		}
	}
}

map<string, string> game::get_general_stats(string user)
{
	if (this->generalUpdates.find(user) != this->generalUpdates.end()) {
		return this->generalUpdates.find(user)->second;
	}
	else {
		return map<string, string>();
	}
}

map<string, string> game::get_team_a_stats(string user)
{
	if (this->team_aUpdates.find(user) != this->team_aUpdates.end()) {
		return this->team_aUpdates.find(user)->second;
	}
	else {
		return map<string, string>();
	}
}
map<string, string> game::get_team_b_stats(string user)
{
	if (this->team_bUpdates.find(user) != this->team_bUpdates.end()) {
		return this->team_bUpdates.find(user)->second;
	}
	else {
		return map<string, string>();
	}
}
