#include <iostream>
#include <stdlib.h>
#include "game.h"
#include "networking.h"

using namespace std;

int main()
{
	game game1(1,2);
	game1.insertDisc(7,1);
	game1.insertDisc(1,2);
	game1.insertDisc(1,1);
	game1.draw();
	auto a = game1.serialize();
	game1.deserialize(a);
	game1.draw();

	return 0;
}

