#ifndef GAME_H
#define GAME_H

#define INDENT "          "

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <stdlib.h>

#define NUMBER_OF_ROUNDS 3
#define GAME_DATA_LENGTH 45

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

	static int pickFirstPlayer();
	static bool isLineStartingAt(int x, int y,  char gameTable[][7]);
	static bool lineMatch(int x, int y, int stepX, int stepY, char gameTable[][7]);
	static bool canInsert(int x, int y, char gameTable[][7]);
public:
	game(int play1 = 1, int play2 = 2);

	void draw();
	void clearScreen();
	void deserialize(char *packet);
	void changePlayer();
	void incrementScore();
	void resetBoard();

	bool insertDisc(int column, int player);
	bool winCondition();
	bool gameOver();

	char* serialize();

	int getCurrentPlayer();
};

#endif // GAME_H
