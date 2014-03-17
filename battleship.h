#ifndef BATTLESHIP_H
#define BATTLESHIP_H

class battleship
{
	public:

		struct coordinates
		{
			//ship coordinates
			int x, y;
		};

		void initializeGame();
		void playerStartGame();
		void enemyStartGame();
		void endGame();
		bool playAgain();
		
	private:

		static const int row = 10, column = 10, amountOfShips = 30;
		char **enemyGridView, **hiddenEnemyGrid, **playerGrid;
		coordinates all[row * column], enemyBattleships[amountOfShips], playerBattleships[amountOfShips];
		
		char **createGrid(int row, int column);
		void initializeGrid(char **grid, int row, int column);
		void generateRandomCoordinates(coordinates all[], coordinates battleships[], int amountOfShips, int row, int column);
		void populateGrid(char **grid, int row, int column, coordinates battleships[], int amountOfShips);

		void displayGrid(char **grid, int row, int column);
		void displayHUD(char **gridOne, char **gridTwo, int row, int column);

		void fireMissile(char **hiddenEnemyGrid, char **gridView, int row, int column);
		void fireEnemyMissile(char **playerGrid, coordinates all[], int enemyTargetCount);

		bool checkWin(char **grid, int row, int column, int amountOfShips);
		void announceWinner(char **grid, int row, int column, int amountOfShips);
		void freeDynamicMemory(char **grid, int column);
};

#endif