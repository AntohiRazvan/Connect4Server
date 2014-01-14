#include "networking.h"

networking::networking()
{
}

int networking::createSocket(){
	if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror ("[server]Eroare la socket().\n");
		return errno;
	}

	/* utilizarea optiunii SO_REUSEADDR */
	int on=1;
	setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	/* pregatirea structurilor de date */
	bzero (&server, sizeof (server));
	bzero (&from, sizeof (from));

	/* umplem structura folosita de server */
	/* stabilirea familiei de socket-uri */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl (INADDR_ANY);
	server.sin_port = htons (port);

	/* atasam socketul */
	if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
	{
		perror ("[server]Eroare la bind().\n");
		return errno;
	}

	/* punem serverul sa asculte daca vin clienti sa se conecteze */
	if (listen (sd, 10) == -1)
	{
		perror ("[server]Eroare la listen().\n");
		return errno;
	}
	return 1;
}

void networking::startServer(){
	while (1)
	{
		int client;
		gameData* gd; //parametru functia executata de thread
		socklen_t length = sizeof (from);

		printf ("[server]Asteptam la portul %d...\n",port);
		fflush (stdout);

		if ((client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
		{

		  perror ("[server]Eroare la accept().\n");
		  continue;
		}
		clients.push_back(client);
		while((clients.size() % 2 == 0) && (clients.size() > 0))
		{
			gd = new gameData;
			gd->player1 = clients.back();
			clients.pop_back();
			gd->player2 = clients.back();
			clients.pop_back();
			gd->idThread = threadCounter++;
			if(threadCounter < MAX_NR_THREADS)
			{
				pthread_create(&threads[threadCounter], NULL, createGame, gd);
			}
			else
			{
				std::cout << "Numarul maxim de jocuri simultane atins.";
				break;
			}
		}
	}

}

void* networking::createGame(void *arg){
	struct gameData gd = *((struct gameData*)arg);
	game Game;
	pthread_detach(pthread_self());
	notifyPlayers(gd);
	while(!gameOverCheck(gd, Game))
	{
		while(!winConditionCheck(gd, Game))
		{
			sendGameState(gd, Game.serialize());
			getMove(gd, Game);
			Game.changePlayer();
		}
		Game.incrementScore();
		Game.resetBoard();
		sendGameState(gd, Game.serialize());
	}
	close(gd.player1);
	close(gd.player2);
	return NULL;
}

void networking::notifyPlayers(gameData gd){
	int message_player1 = 1;
	int message_player2 = 2;
	if(write(gd.player1, &message_player1, sizeof(int)) == -1)
	{
		perror("[server]Eroare la scriere in socket");
	}
	if(write(gd.player2, &message_player2, sizeof(int)) == -1)
	{
		perror("[server]Eroare la scriere in socket");
	}
}

void networking::sendGameState(gameData gd, char* state){
	if(write(gd.player1, state, GAME_DATA_LENGTH) == -1)
	{
		perror("[server]Eroare la scriere in socket");
	}
	if(write(gd.player2, state, GAME_DATA_LENGTH) == -1)
	{
		perror("[server]Eroare la scriere in socket");
	}
}

void networking::getMove(gameData gd, game &Game){
	int position;
	bool response;
	if(Game.getCurrentPlayer() == 1)
	{
		if(read(gd.player1, &position, sizeof(int)) == -1)
		{
			perror("[server]Eroare la citire din socket");
		}
		while(!(response = Game.insertDisc(position, Game.getCurrentPlayer())))
		{
			if(write(gd.player1, &response, sizeof(bool)) == -1)
			{
				perror("[server]Eroare la scriere in socket");
			}
			if(read(gd.player1, &position, sizeof(int)) == -1)
			{
				perror("[server]Eroare la citire din socket");

			}
		}
		if(write(gd.player1, &response, sizeof(bool)) == -1)
		{
			perror("[server]Eroare la scriere in socket");
		}
	}
	else if(Game.getCurrentPlayer() == 2)
	{
		if(read(gd.player2, &position, sizeof(int)) == -1)
		{
			perror("[server]Eroare la citire din socket");
		}
		while(!(response = Game.insertDisc(position, Game.getCurrentPlayer())))
		{
			if(write(gd.player2, &response, sizeof(bool)) == -1)
			{
				perror("[server]Eroare la scriere in socket");
			}
			if(read(gd.player2, &position, sizeof(int)) == -1)
			{
				perror("[server]Eroare la citire din socket");
			}
		}
		if(write(gd.player2, &response, sizeof(bool)) == -1)
		{
			perror("[server]Eroare la scriere in socket");
		}
	}
}

bool networking::gameOverCheck(gameData gd, game Game){
	bool gameOver = Game.gameOver();
	if(write(gd.player1, &gameOver, sizeof(bool)) == -1)
	{
		perror("[server]Eroare la scriere in socket");
	}
	if(write(gd.player2, &gameOver, sizeof(bool)) == -1)
	{
		perror("[server]Eroare la scriere in socket");
	}
	return gameOver;
}

bool networking::winConditionCheck(gameData gd, game Game){
	bool winCondition = Game.winCondition();
	if(write(gd.player1, &winCondition, sizeof(bool)) == -1)
	{
		perror("[server]Eroare la scriere in socket");
	}
	if(write(gd.player2, &winCondition, sizeof(bool)) == -1)
	{
		perror("[server]Eroare la scriere in socket");
	}
	return winCondition;
}
