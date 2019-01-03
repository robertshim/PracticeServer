#pragma once

#include<winsock2.h>

#define BUF_SIZE 152
#define READ 3
#define WRITE 5

typedef struct {
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
}SOCK_INFO_DATA, *LPSOCK_INFO_DATA;

typedef struct {
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode;
}IO_DATA, *LPIO_DATA;

struct MESSAGE
{
	int type;
	int roomId;
	char writer[16];
	char text[128];
};

enum MESSAGE_TYPE {
	CREATE = 1, CREATE_ACK, JOIN_REQ, JOIN_REQ_ACK,
	JOIN, JOIN_ACK, CHAT, VOTE, POLLING, QUIT, JOB, START,
	ACTION, ACTION_RESULT, VOTE_RESULT, GAME_RESULT, READY
};