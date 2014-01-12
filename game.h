#ifndef GAME_H
#define GAME_H

#define INDENT "          "

#include <iostream>
#include <cstdlib>
#include <ctime>

const int nrRows = 6;
const int nrColumns = 7;

class game
{
	int player1;
	int player1Score;
	int player2;
	int player2Score;
	char gameTable[nrRows][nrColumns];
	int currentPlayer;

	int pickFirstPlayer();
public:
	game(int play1, int play2);
	void draw();
};

#endif // GAME_H
