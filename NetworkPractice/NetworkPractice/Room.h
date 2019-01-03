#pragma once

#include <string>
#include "player.h"

using namespace std;
typedef Player* PLAYER;


class Room {
private:
	unsigned int id;
	string ip;
	unsigned short port;
	unsigned char numOfPlayer;
	PLAYER* players;

public:
	Room(unsigned int id, string ip, unsigned short port);
	unsigned int getRoomId();
	boolean addPlayer(PLAYER player);
	PLAYER* getPlayers();
	PLAYER getPlayer(string id);
	boolean isFull();

	boolean isAllReady();
};
