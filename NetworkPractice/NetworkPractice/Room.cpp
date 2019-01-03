#include "Room.h"

Room::Room(unsigned int id, string ip, unsigned short port): id(id), ip(ip), port(port){
	players = new PLAYER[MAX_PLAYER];
	numOfPlayer = 0;
}

unsigned int Room::getRoomId() {
	return id;
}

boolean Room::addPlayer(PLAYER player) {
	if (!isFull) {
		this->players[numOfPlayer++] = player;
		return true;
	}
	delete player;
	return false;
}

boolean Room::isFull() {
	if (numOfPlayer != MAX_PLAYER) {
		return false;
	}
	return true;
}

PLAYER* Room::getPlayers() {
	return players;
}

PLAYER Room::getPlayer(string id) {
	
}


boolean Room::isAllReady() {

	for (int i = 0; i < sizeof(players); i++) {
		if (!players[i]->isReady()) {
			return false;
		}
	}

	return true;
}