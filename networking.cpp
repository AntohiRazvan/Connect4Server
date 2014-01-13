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
			if(threadCounter<100)
			{
				pthread_create(&threads[threadCounter], NULL, createGame, gd);
			}
			else
			{
				perror("Numarul maxim de jocuri simultane atins.");
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
	while(!Game.gameOver())
	{
		while(!Game.winCondition())
		{
			sendGameState(gd, Game.serialize());
			Game.insertDisc(getMove(gd, Game.getCurrentPlayer()), Game.getCurrentPlayer());
			Game.changePlayer();
		}
		Game.incrementScore();
		Game.resetBoard();
	}
	close(gd.player1);
	close(gd.player2);
	return NULL;
}

void networking::notifyPlayers(gameData gd){
	char *message_player1 = new char;
	char *message_player2 = new char;
	strcpy(message_player1, "Game started you are player 1.");
	strcpy(message_player2, "Game started you are player 2.");
	if(write(gd.player1, message_player1, strlen(message_player1)) == -1)
	{
		perror("[server]Eroare la scriere in socket");
	}
	if(write(gd.player2, message_player2, strlen(message_player2)) == -1)
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

int networking::getMove(gameData gd, int currentPlayer){
	int position;
	if(currentPlayer == 1)
	{
		if(read(gd.player1, &position, sizeof(int)) == -1)
		{
			perror("[server]Eroare la citire din socket");

		}
	}
	else
	{
		if(read(gd.player2, &position, sizeof(int)) == -1)
		{
			perror("[server]Eroare la citire din socket");
		}
	}
	return position;
}

