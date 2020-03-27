#include <iostream>
#include "Board.h" 
#include "GenerateBombs.h"
#include "Position.h"

using namespace std;

int main()
{
	int i, j, bombs;
	char k;
	bool gameover = false;
	cout << "Choose difficulty: ";
	char d = 'e';
	cin >> d;
	switch (d) {
	case 'e': bombs=20;
		break;
	case 'm': bombs=40;
		break;
	case 'h': bombs=60;
		break;
	default:
		bombs = 40;
	}
	auto board = Board(GenerateBombs(bombs).getBombs());
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
			// if reveal a 0 tile also reveal neighbours
			// if any of these are also 0 reveal all their neighbours
			if (board.getTile(Position(i, j), board).getNeighbours() == 0) {
				board.revealNeighbours(Position(i,j),board);
			
			}
		}
		else if (k == 'f') {
			board.getTile(Position(i, j), board).setFlagged(true);
		}
		else if (k == 'u') {
			board.getTile(Position(i, j), board).setFlagged(false);
		}
		if (board.getRemaining() == 0) {
			gameover = true;
		}
		system("cls");
		board.printBoard();
	}
	
	cout << "Game over";



}

/*
TODO LIST

win game 

create menu with how to play section and new game


*/