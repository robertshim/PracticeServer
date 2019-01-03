#include "Player.h"


Player::Player(string id, SOCKET socket) : id(id), socket(socket) {
	job = NONE;
	flagReady = false;
}

string Player::getId() {
	return id;
}

SOCKET Player::getSocket() {
	return socket;
}

boolean Player::isReady() {
	return flagReady
}

void Player::setReady() {
	flagReady = !flagReady;
}