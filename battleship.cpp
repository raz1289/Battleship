#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <limits>
#include "battleship.h"
#include "screen.h"

using namespace std;


//A function that calls other functions that creates 2-dimensional dynamic grids, initializes
//them, generates randomcoordinates, populates the grid with the random coordinates.
void battleship::initializeGame()
{
	enemyGridView = createGrid(row, column);
	hiddenEnemyGrid = createGrid(row, column);
	playerGrid = createGrid(row, column);

	initializeGrid(enemyGridView, row, column);
	initializeGrid(hiddenEnemyGrid, row, column);
	initializeGrid(playerGrid, row, column);

	generateRandomCoordinates(all, enemyBattleships, amountOfShips, row, column);
	generateRandomCoordinates(all, playerBattleships, amountOfShips, row, column);

	random_shuffle(all, all + (row * column));

	populateGrid(hiddenEnemyGrid, row, column, enemyBattleships, amountOfShips);
	populateGrid(playerGrid, row, column, playerBattleships, amountOfShips);
}

//Creates a 2-dimensional dynamic grid.
char **battleship::createGrid(int row, int column)
{
	char **grid;
	grid = new char*[column];

	for (int x = 0; x < column; x++)
	{
		grid[x] = new char[row];
	}

	return grid;
}

//Initializes a 2-dimensional grid.
void battleship::initializeGrid(char **grid, int row, int column)
{
	for (int x = 0; x < column; x++)
	{
		for (int y = 0; y < row; y++)
		{
			grid[x][y] = '-';
		}
	}
}

//Given row and column, this function generates all possible coordinates in a grid
//and stores them in an array of all possible coordinates.
//Array of all possible coordinates is shuffled randomly.
//Takes array of all possible shuffled coordinates and chooses coordinates.
//Amount of coordinates is equal to the number of ships per game participant.
//Chosen coordinates are stored in a separate array of coordinates.
void battleship::generateRandomCoordinates(coordinates all[], coordinates battleships[], int amountOfShips, int row, int column)
{
	int x = 0, xCount = 0, y = 0;

	for (int i = 0; i < (row * column); i++)
	{
		all[i].x = x;
		all[i].y = y;

		xCount++;
		y++;

		if ((xCount % static_cast<int>(sqrt(row * column)) == 0) || (y % static_cast<int>(sqrt(row * column)) == 0))
		{
			x++;
			y = 0;
		}
	}

	random_shuffle(&all[0], &all[row*column]);

	for (int i = 0; i < amountOfShips; i++)
	{
		battleships[i].x = all[i].x;
		battleships[i].y = all[i].y;
	}
}

//A grid is populated with ships located on randomly generated coordinates
void battleship::populateGrid(char **grid, int row, int column, coordinates battleships[], int amountOfShips)
{
	for (int k = 0; k < amountOfShips; k++)
	{
		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				if ((x == battleships[k].x) && (y == battleships[k].y))
				{
					grid[x][y] = '8';
				}
			}
		}
	}
}

//Displays a grid
void battleship::displayGrid(char **grid, int row, int column)
{
	int c = 0;

	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < column; x++)
		{
			cout << grid[x][y] << "  ";

			if ((x != 0) && (x % (column - 1) == 0))
			{
				cout << "| " << c++;
				cout << endl;
			}
		}
	}
}

//Shows the Heads Up Display of the game, which consists of the enemy grid and player grid.
void battleship::displayHUD(char **gridOne, char **gridTwo, int row, int column)
{
	displayGrid(gridOne, row, column);

	for (int i = 0; i < column; i++)
	{
		cout << "---";
	}

	cout << endl;

	for (int i = 0; i < column; i++)
	{
		cout << i << "  ";
	}

	cout << endl;

	for (int i = 0; i < column; i++)
	{
		cout << "---";
	}

	cout << endl;

	displayGrid(gridTwo, row, column);
}

//Fires a player missile at the enemy grid.
void battleship::fireMissile(char **hiddenEnemyGrid, char **gridView, int row, int column)
{
	string coordinates = "---";
	int x, y;

	cout << "Enter coordinates to fire a missile at enemy (format: x,y): ";
	getline(cin, coordinates);

	while ((coordinates.empty()) || (coordinates.length() > 3) || (coordinates.length() < 3) || (!isdigit(coordinates.at(0))) ||
		(!isdigit(coordinates.at(2))) || (coordinates.at(1) != ',') || (coordinates.at(0) - '0' >= row) || (coordinates.at(2) - '0' >= column))
	{
		cout << "Entry is invalid or out of bounds. Enter a valid coordinate (format: x,y): ";
		getline(cin, coordinates);
	}

	cout << endl;

	x = coordinates.at(0) - '0';
	y = coordinates.at(2) - '0';

	if (hiddenEnemyGrid[x][y] == '8')
	{
		cout << "Your missile has hit an enemy battleship. Battleship sunk!" << endl;
		hiddenEnemyGrid[x][y] = 'X';
		gridView[x][y] = hiddenEnemyGrid[x][y];
	}
	else if (hiddenEnemyGrid[x][y] == '-')
	{
		cout << "You missed! No battleships hit!" << endl;
		hiddenEnemyGrid[x][y] = '+';
		gridView[x][y] = hiddenEnemyGrid[x][y];
	}
	else
	{
		cout << "You've already fired a missile here, pick another coordinate!" << endl;
		fireMissile(hiddenEnemyGrid, gridView, row, column);
	}
}

//Fires an enemy missile at the player grid.
//The array of all possible coordinates is previously shuffled in the function initializeGame() and is passed in.
//This essentially the battleship computer AI.
void battleship::fireEnemyMissile(char **playerGrid, coordinates all[], int enemyTargetCount)
{
	int x, y;

	x = all[enemyTargetCount].x;
	y = all[enemyTargetCount].y;

	if (playerGrid[x][y] == '8')
	{
		playerGrid[x][y] = 'X';

		cout << "Enemy missile has hit your battleship! Your battleship has sunk!" << endl;
	}
	else
	{
		playerGrid[x][y] = '+';

		cout << "Enemy attack! Enemy missile missed! No battleships hit!" << endl;
	}
}

//Checks for the winning condition
bool battleship::checkWin(char **grid, int row, int column, int amountOfShips)
{
	int deadBattleships = 0;

	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < column; x++)
		{
			if (grid[x][y] == 'X')
			{
				deadBattleships++;
			}
		}
	}

	if (deadBattleships == amountOfShips)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//If player won the Flip Coin mini game, then the player fires the first missile.
void battleship::playerStartGame()
{
	int enemyTargetCount = 0;
	bool winningCondition;

	clearScreen();

	do
	{
		displayHUD(enemyGridView, playerGrid, row, column);

		fireMissile(hiddenEnemyGrid, enemyGridView, row, column);

		winningCondition = checkWin(hiddenEnemyGrid, row, column, amountOfShips);

		if (winningCondition == true)
		{
			pauseScreen();
			break;
		}

		fireEnemyMissile(playerGrid, all, enemyTargetCount);

		enemyTargetCount++;

		winningCondition = checkWin(playerGrid, row, column, amountOfShips);

		pauseScreen();
		clearScreen();

	} while (winningCondition == false);

	clearScreen();

	displayHUD(enemyGridView, playerGrid, row, column);

	cout << endl;
}

//If player loses the Flip Coin game, then the enemy (computer AI) fires the first missile.
void battleship::enemyStartGame()
{
	int enemyTargetCount = 0;
	bool winningCondition;

	clearScreen();

	do
	{
		displayHUD(enemyGridView, playerGrid, row, column);

		fireEnemyMissile(playerGrid, all, enemyTargetCount);

		enemyTargetCount++;

		winningCondition = checkWin(playerGrid, row, column, amountOfShips);

		if (winningCondition == true)
		{
			pauseScreen();
			break;
		}

		fireMissile(hiddenEnemyGrid, enemyGridView, row, column);

		winningCondition = checkWin(hiddenEnemyGrid, row, column, amountOfShips);

		pauseScreen();
		clearScreen();

	} while (winningCondition == false);

	clearScreen();

	displayHUD(enemyGridView, playerGrid, row, column);

	cout << endl;
}

//Evaluates to see who won the game. Announces the winner.
void battleship::announceWinner(char **grid, int row, int column, int amountOfShips)
{
	bool winner;

	winner = checkWin(grid, row, column, amountOfShips);

	if (winner == true)
	{
		cout << "All enemy ships destroyed!" << endl;
		cout << "Congratulations! You have won the game!" << endl << endl;
	}
	else
	{
		cout << "Enemy has destroyed all your ships!" << endl;
		cout << "You have been defeated! You lost the game!" << endl << endl;
	}
}

//Deletes dynamic memory.
void battleship::freeDynamicMemory(char **grid, int column)
{
	for (int x = 0; x < column; x++)
	{
		delete grid[x];
	}

	delete[] grid;
}

//A function that calls other functions that announces the winner and
//then deletes 2-dimensional dynamic grids.
void battleship::endGame()
{
	announceWinner(hiddenEnemyGrid, row, column, amountOfShips);

	freeDynamicMemory(enemyGridView, column);
	freeDynamicMemory(hiddenEnemyGrid, column);
	freeDynamicMemory(playerGrid, column);
}

//Asks user to play again.
bool battleship::playAgain()
{
	string input;

	cout << "Would you like to play again? (Y/N): ";
	getline(cin, input);

	while ((input.empty()) || (input.length() > 1) || (!isalpha(input.at(0))) || 
		!((input == "y") || (input == "Y") || (input == "n") || (input == "N")))
	{
		cout << "Invalid input. Would you like to play again? (Y/N): ";
		getline(cin, input);
	}

	cout << endl;

	if ((input == "Y") || (input == "y"))
	{
		return true;
	}
	else
	{
		cout << "Game is exiting. Thanks for playing!";
		pauseScreen();

		return false;
	}
}