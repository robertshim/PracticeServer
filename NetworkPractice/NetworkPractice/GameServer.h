#pragma once

#include<vector>
#include<map>
#include<string>
#include<mutex>
#include"Room.h"
#include"Player.h"
#include"io.h"
using namespace std;

#define PORT 3303

class GameServer {
private:
	vector<Room> roomList;
	map<string, Room*> table;
	Room* searchRoom(unsigned int id);
public:
	boolean createRoom();
	boolean joinRoom(unsigned int id, PLAYER player);
	vector<Room> getRoomList();
	Room* whereAmI(string id);

	boolean sendEveryOne(Room* room,MESSAGE* message);
};