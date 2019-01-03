#include "Player.h"


Player::Player(string id, SOCKET socket) : id(id), socket(socket) {
	job = NONE;
}

string Player::getId() {
	return id;
}

SOCKET Player::getSocket() {
	return socket;
}