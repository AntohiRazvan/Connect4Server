#ifndef NETWORKING_H
#define NETWORKING_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <vector>
#include "game.h"

#define MAX_NR_THREADS 100
typedef int socket_t;
extern int errno;

struct gameData{
	int idThread;
	int player1;
	int player2;
};

class networking
{
	static const int port = 7834;
	struct sockaddr_in server;	// structura folosita de server
	struct sockaddr_in from;

	int sd;		//descriptorul de socket
	int pid;
	int threadCounter = 0;

	pthread_t threads[MAX_NR_THREADS];    //Identificatorii thread-urilor care se vor crea
	std::vector<int> clients;


	static void *createGame(void *arg);
	static void sendGameState(gameData gd, char* state);
	static void sendGameOverCheck(gameData gd, bool gameOver);
	static void sendRoundOverCheck(gameData gd, bool roundOver);
	static void notifyPlayers(gameData gd);
	static int getMove(gameData gd, int currentPlayer);

public:
	networking();
	char* conv_addr(sockaddr_in address);
	int createSocket(void);
	void startServer();
};

#endif // NETWORKING_H
