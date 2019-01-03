#pragma once

#include <string>
#include <winsock2.h>
#define MAX_PLAYER 6

using namespace std;

enum Job {
	NONE, MAFIA, POLICE, CIVIL
};

enum Status {
	LIVE, DEAD
};

class Player {
private:
	string id;
	SOCKET socket;
	Job job;
	Status status;
public:
	Player(string id, SOCKET socket);
	string getId();
	SOCKET getSocket();
};
