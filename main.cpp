#include <iostream>
#include <stdlib.h>
#include "networking.h"

using namespace std;

int main()
{
	networking net;
	net.createSocket();
	net.startServer();
	return 0;
}
