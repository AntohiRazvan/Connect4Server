#include "game.h"

game::game(int play1, int play2){
	player1 = play1;
	player1Score = 0;
	player2 = play2;
	player2Score = 0;

	for(int row = 0; row < nrRows; row++)
		for(int column = 0; column < nrColumns; column++)
		{
			this->gameTable[row][column] = ' ';
		}
}

void game::draw(){
	std::cout << std::string(5, '\n');
	std::cout << INDENT << " \033[31mPlayer 1\033[0m  " << player1Score << "  -  " << player2Score << "  \033[34mPlayer 2\033[0m" << std::endl << std::endl;

	std::cout << std::endl << INDENT << "_____________________________" << std::endl;
	for(int row = 0; row < nrRows; row++)
	{
		std::cout << INDENT << "| ";
		for(int column = 0; column < nrColumns; column++)
		{
			if(gameTable[row][column] == '1' && row != 0 && column != 0)
			{
				std::cout << "\033[1;31mo\033[0m" << " | ";
			}
			else if(gameTable[row][column] == '2' && row != 0 && column != 0)
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
}

int game::pickFirstPlayer(){
	srand((unsigned)time(0));
	int choice = rand();
}
