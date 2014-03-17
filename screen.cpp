#include <iostream>
#include <string>
#include "screen.h"

using namespace std;


//Prints 100 newline characters to clear the screen.
void clearScreen()
{
	cout << string(100, '\n');
}

//Getline into a string buffer to pause the screen.
void pauseScreen()
{
	string pauseBuffer;

	getline(cin, pauseBuffer);
}