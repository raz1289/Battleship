#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include "battleship.h"
#include "screen.h"

#define WELCOME_MESSAGE "Welcome to Battleship!"

using namespace std;

void coinFlip(char *argv[], int &random, string &coinChoice, string &coin);


int main(int argc, char *argv[])
{
	battleship gameSession;
	string coin = "HT", coinChoice;
	int random;

	srand(static_cast<int>(time(0)));

	cout << WELCOME_MESSAGE << endl << endl;

	
	do
	{
		coinFlip(argv, random, coinChoice, coin);

		gameSession.initializeGame();

		//Evaluate the results of the Coin Flip game
		if (((coinChoice[0] == coin[random]) || (toupper(coinChoice[0]) == coin[random])) || (argv[1] != NULL))
		{
			cout << "You have won the coin toss! You will strike first!" << endl << endl;
			cout << "Press enter to continue.";

			pauseScreen();

			clearScreen();

			cout << "Battleship legend: - -> Water" << endl;
			cout << "Battleship legend: 8 -> Battleship" << endl;
			cout << "Battleship legend: X -> Missile fired here" << endl;
			cout << "Battleship legend: + -> Missile missed here" << endl << endl;
			cout << "Press enter to continue.";
			
			pauseScreen();

			gameSession.playerStartGame();
		}
		else
		{
			cout << "You have lost the coin toss! Enemy will strike first!" << endl;
			cout << "Press enter to continue.";

			pauseScreen();

			clearScreen();

			cout << "Battleship legend: - -> Water" << endl;
			cout << "Battleship legend: 8 -> Battleship" << endl;
			cout << "Battleship legend: X -> Missile fired here" << endl;
			cout << "Battleship legend: + -> Missile missed here" << endl << endl;
			cout << "Press enter to continue.";

			pauseScreen();

			gameSession.enemyStartGame();
		}

		gameSession.endGame();

	} while (gameSession.playAgain() == true);

	return 0;
}

//Coin Flip mini game to determine if player or computer attacks first
//Takes into consideration any command line argument as a "cheat mode"
void coinFlip(char *argv[], int &random, string &coinChoice, string &coin)
{
	cout << "Choose heads or tails (H/T): ";
	getline(cin, coinChoice);

	while ((coinChoice.empty()) || (coinChoice.length() > 1) || (!isalpha(coinChoice[0])) ||
		!(((coinChoice == "H")) || ((coinChoice == "h")) || ((coinChoice == "T")) || ((coinChoice == "t"))))
	{
		cout << "Invalid input. Choose heads or tails (H/T): ";
		getline(cin, coinChoice);
	}

	cout << "Coin is being flipped! Press enter to continue.";

	pauseScreen();

	cout << endl;

	if (argv[1] != NULL)
	{
		cout << "Your command line argument " << "\"" << argv[1] << "\"" << " has activated cheat mode!" << endl;
		cout << "You will automatically win the coin toss!" << endl << endl;

		cout << "Cheat mode will persist for all games." << endl;
		cout << "To turn off, exit and restart the game without any command line arguments.";

		pauseScreen();

		cout << endl;
	}

	random = ((rand() % 2) + 0);

	cout << endl;

	cout << "The coin toss result is: " << coin[random] << "!" << endl << endl << endl;
}