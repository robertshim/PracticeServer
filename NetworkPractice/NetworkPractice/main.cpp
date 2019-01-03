#include<iostream>
#include<cstdlib>
#include<process.h>
#include<windows.h>
#include<winsock2.h>

#include"GameServer.h"
#include"io.h"

unsigned WINAPI ThreadProcess(LPVOID param);

int main(void) {

	WSADATA wsaData;
	GameServer* gameServer;
	DWORD recvBytes, flag = 0;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		//에러처리
	}

	gameServer = new GameServer();

	//cp객체 등록 및 스레드 생성
	HANDLE hComport = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	char* param = new char[sizeof(gameServer) + sizeof(hComport)];

	memcpy(param, hComport, sizeof(hComport));
	memcpy(&param[sizeof(hComport) - 1], gameServer, sizeof(gameServer));

	for (int i = 0; i < sysInfo.dwNumberOfProcessors; i++) {
		_beginthreadex(NULL, 0,ThreadProcess,(LPVOID)param, 0, NULL);
	}

	//비동기 소켓 설정
	SOCKET hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN servAdr;
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(PORT);

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, 30);

	//소켓 연결, 소켓 - CP 연결, 비동기 읽기
	while (true) {
	
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);
		
		LPSOCK_INFO_DATA sockInfo = (LPSOCK_INFO_DATA)malloc(sizeof(SOCK_INFO_DATA));
		sockInfo->hClntSock = hClntSock;
		memcpy(&sockInfo->clntAdr, &clntAdr, sizeof(clntAdr));

		CreateIoCompletionPort((HANDLE)hClntSock, hComport, (ULONG_PTR)sockInfo, 0);

		LPIO_DATA ioData = (LPIO_DATA)calloc(1,sizeof(IO_DATA));
		ioData->wsaBuf.buf = ioData->buffer;
		ioData->rwMode = READ;
		ioData->wsaBuf.len = BUF_SIZE;

		WSARecv(sockInfo->hClntSock, &(ioData->wsaBuf), 1, &recvBytes, &flag, &(ioData->overlapped), NULL);

	}
	return 0;
}

unsigned WINAPI ThreadProcess(LPVOID param) {

	char* _param = (char*)param;
	HANDLE hComport;
	DWORD bytesTrans;
	SOCKET sock;
	LPSOCK_INFO_DATA sockInfo;
	LPIO_DATA ioData;
	GameServer* gameServer;

	memcpy(hComport, _param, sizeof(hComport));
	memcpy(gameServer, &_param[sizeof(hComport) - 1], sizeof(gameServer));


	while (true) {
		GetQueuedCompletionStatus(hComport,&bytesTrans,(PULONG_PTR)&sockInfo,(LPOVERLAPPED*)&ioData,INFINITE);
		
		MESSAGE* message = new MESSAGE;
		memcpy(&message, ioData->buffer,sizeof(BUF_SIZE));


		// 완료된 IO 종류 구분

		if (ioData->rwMode == READ) {
			switch (message->type)
			{
			case CREATE:
				gameServer->createRoom();
				break;
			case JOIN:
				PLAYER player = new Player(message->writer,sockInfo->hClntSock);
				gameServer->joinRoom(message->roomId, player);
				break;
			case CHAT:
				Room* room = gameServer->whereAmI(message->writer);
				gameServer->sendBroadCast(room, message);
				break;

			case READY:
				Room* room = gameServer->whereAmI(message->writer);
				PLAYER player = room->getPlayer(message->writer);
				player->setReady();
				room->isAllReady();
				break;
			default:
				break;
			}
			
		}


		else {
		
		}
	}
	return 0;
}