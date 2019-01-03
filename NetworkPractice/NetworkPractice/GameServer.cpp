#include"GameServer.h"
#include<ctime>
boolean GameServer::createRoom() {
	
	//lock
	unsigned int totalSecond = time(0);
	Room* room = new Room(totalSecond,INADDR_ANY,PORT);
	roomList.push_back(*room);
	//unlock
}

boolean GameServer::joinRoom(unsigned int id, PLAYER player) {
	Room* room = searchRoom(id);
	if (room != NULL) {
		
		//lock
		boolean flag = room->addPlayer(player);
		//unlock
		
		if (flag) {
			table[player->getId()] = room;
			return true;
		}
		else
			return false;
	}
	return false;
}


vector<Room> GameServer::getRoomList() {
	return roomList;
}

Room* GameServer::whereAmI(string id) {
	Room* room = table.find(id)->second;
	return room;
}


Room* GameServer::searchRoom(unsigned int id) {
	vector<Room>::iterator itr;
	
	for (itr = roomList.begin; itr != roomList.end(); itr++) {
		if (id == itr->getRoomId()) {
			return &(*itr);
		}
	}
	return NULL;
}


boolean GameServer::sendBroadCast(Room* room, MESSAGE* message) {
	PLAYER* players = room->getPlayers;

	for (int i = 0; i < sizeof(players); i++) {
		SOCKET socket = players[i]->getSocket();
		LPIO_DATA ioData = (LPIO_DATA)calloc(1, sizeof(IO_DATA));
		ioData->wsaBuf.buf = ioData->buffer;
		ioData->wsaBuf.len = BUF_SIZE;
		ioData->rwMode = WRITE;

		memcpy(ioData->buffer,message,BUF_SIZE);

		WSASend(socket,&ioData->wsaBuf,1,NULL,0,&ioData->overlapped,NULL);
	}
}