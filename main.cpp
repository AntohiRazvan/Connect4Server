#include <iostream>
#include <stdlib.h>
#include "game.h"
#include "networking.h"

using namespace std;

int main()
{
	game game1(1,2);
	game1.draw();
	sleep(4);
	game1.draw();
	std::cout << '1';
	return 0;
}

