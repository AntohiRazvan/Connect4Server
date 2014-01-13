#include "game.h"

game::game(int play1, int play2){
	player1 = play1;
	player1Score = 0;
	player2 = play2;
	player2Score = 0;
	currentPlayer = pickFirstPlayer();

	for(int row = 0; row < nrRows; row++)
		for(int column = 0; column < nrColumns; column++)
		{
			this->gameTable[row][column] = ' ';
		}
}

void game::changePlayer(){
	if(currentPlayer == 1)
		currentPlayer = 2;
	else
		currentPlayer = 1;
}

int game::getCurrentPlayer(){
	return currentPlayer;
}

void game::draw(){
	std::cout << INDENT << " \033[31mPlayer 1\033[0m  " << player1Score << "  -  "
				 << player2Score << "  \033[34mPlayer 2\033[0m" << std::endl;
	std::cout << INDENT << INDENT << " Player " << currentPlayer << std::endl << std::endl;
	std::cout << std::endl << INDENT << "_____________________________" << std::endl;
	for(int row = 0; row < nrRows; row++)
	{
		std::cout << INDENT << "| ";
		for(int column = 0; column < nrColumns; column++)
		{
			if(gameTable[row][column] == '1')
			{
				std::cout << "\033[1;31mo\033[0m" << " | ";
			}
			else if(gameTable[row][column] == '2')
			{
				std::cout << "\033[1;34mo\033[0m" << " | ";
			}
			else
			{
				std::cout << gameTable[row][column] << " | ";
			}
		}

		std::cout << std::endl << INDENT << "_____________________________" << std::endl;
	}
	std::cout << INDENT << "| ";
	for(int column = 1; column <= nrColumns; column++)
	{
		std::cout << column << " | ";
	}
	std::cout << std::endl << std::endl;
}

int game::pickFirstPlayer(){
	srand((unsigned)time(0));
	int choice = rand();
	return choice%2+1;
}

bool game::lineMatch(int x, int y, int stepX, int stepY, char gameTable[][7]){
	const int startValue = gameTable[x][y];

	for (int i = 0; i < 4; i++)
	{
		if (gameTable[x + i * stepX][y + i * stepY] != startValue || startValue == ' ')
		{
			return false;
		}
	}
	return true;
}

bool game::isLineStartingAt(int x, int y, char gameTable[][7]){
		 return lineMatch(x, y, 1,  0, gameTable) || // Horizontal
				  lineMatch(x, y, 0,  1, gameTable) || // Vertical
				  lineMatch(x, y, 1,  1, gameTable) || // Diagonal Down
				  lineMatch(x, y, 1, -1, gameTable);   // Diagonal Up
}

bool game::winCondition(){
	for(int row = 0; row < nrRows; row++)
		for(int column = 0; column < nrColumns; column++)
			if(isLineStartingAt(row, column, gameTable) == 1)
				return true;
	return false;
}

void game::clearScreen(){
	std::cout << std::endl << std::endl;
}

bool game::canInsert(int x, int y, char gameTable[][7]){
	if(gameTable[x][y] == ' ')
		return true;
	return false;
}

bool game::insertDisc(int column, int player){
	column -= 1;
	for(int row = nrRows-1; row >= 0; row--)
	{
		if( canInsert(row, column, gameTable))
		{
			gameTable[row][column] = (char)(((int)'0')+player);
			return true;
		}
	}
	return false;
}

char* game::serialize(){
	int size = 0;
	char* packet = new char;
	packet[size] = (char)(((int)'0')+player1Score);
	size++;
	packet[size] = (char)(((int)'0')+player2Score);
	size++;
	packet[size] = (char)(((int)'0')+currentPlayer);
	size++;
	for(int row = 0; row < nrRows; row++)
		for(int column = 0; column < nrColumns; column++)
		{
			packet[size] = gameTable[row][column];
			size++;
		}
	return packet;
}

void game::deserialize(char* packet){
	player1Score = (int)(packet[0] - '0');
	player2Score = (int)(packet[1] - '0');
	currentPlayer = (int)(packet[2] - '0');
	int row = -1;
	int column = -1;
	for(int index = 3; index < GAME_DATA_LENGTH; index++)
	{
		if((index-3)%7 == 0)
		{
			row++;
			column = 0;
		}
		gameTable[row][column] = packet[index];
		column++;
	}
}

void game::incrementScore(){
	if(currentPlayer == 1)
		player1Score++;
	if(currentPlayer == 2)
		player2Score++;
}

bool game::gameOver(){
	if(player1Score >= NUMBER_OF_ROUNDS / 2 ||
		player2Score >= NUMBER_OF_ROUNDS / 2 )
		return true;
	return false;
}

void game::resetBoard(){
	currentPlayer = pickFirstPlayer();

	for(int row = 0; row < nrRows; row++)
		for(int column = 0; column < nrColumns; column++)
		{
			this->gameTable[row][column] = ' ';
		}
}
