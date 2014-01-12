#ifndef GAME_H
#define GAME_H

#define INDENT "          "

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <stdlib.h>

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
	static bool isLineStartingAt(int x, int y,  char gameTable[][7]);
	static bool lineMatch(int x, int y, int stepX, int stepY, char gameTable[][7]);
	static bool canInsert(int x, int y, char gameTable[][7]);
public:
	game(int play1, int play2);
	void draw();
	void clearScreen();
	bool winCondition();
	bool insertDisc(int column, int player);
	char* serialize();
	void deserialize(char *packet);
};

#endif // GAME_H
