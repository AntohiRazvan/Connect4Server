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

int networking::startServer(){
	while (1)
	{
		int client;
		gameData* gd; //parametru functia executata de thread
		socklen_t length = sizeof (from);

		printf ("[server]Asteptam la portul %d...\n",port);
		fflush (stdout);

		/* acceptam un client (stare blocanta pina la realizarea conexiunii) */
		if ((client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
		{
		  perror ("[server]Eroare la accept().\n");
		  continue;
		}

		clients.push_back(client);
		while((clients.size() % 2 == 0) && (clients.size() != 0))
		{
			gd = new gameData;
			gd->player1 = *clients.end();
			gd->player2 = *clients.end() - 1;
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

}
