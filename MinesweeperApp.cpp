#include <iostream>
#include "Board.h" 
#include "GenerateBombs.h"
#include "Position.h"

using namespace std;

int main()
{
	int i, j;
	char k;
	bool gameover = false;
	auto board = Board(GenerateBombs(40).getBombs());
	board.printBoard();
	while (!gameover) {
		do {
			cin >> k>> i >> j;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore();
			}
		} while (i < 0 || i>19 || j < 0 || j>19);
		if (k == 'r') {
			board.getTile(Position(i, j), board).setRevealed(true);
			if (board.getTile(Position(i, j), board).isBomb()) {
				gameover = true;
				board.revealAllTiles();
			}
		}
		else if (k == 'f') {
			board.getTile(Position(i, j), board).setFlagged(true);
		}
		else if (k == 'u') {
			board.getTile(Position(i, j), board).setFlagged(false);
		}
		system("cls");
		board.printBoard();
	}
	
	cout << "Game over";



}
