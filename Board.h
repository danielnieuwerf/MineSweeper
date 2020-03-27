#pragma once
#include "Position.h"
#include "Tile.h"
#include <vector>
#include <iostream>
using namespace std;

class Board
{
private:
	static const int boardLength=20;
	static const int boardWidth=20;
	Tile board[boardLength][boardWidth];
	int remaining;

public:
	//constructors and destructor
	Board(vector<Position> pos) {
		// pos is position of bombs around the board
		// create board of default tiles without bombs using Tile()
		for (int i = 0; i < boardWidth; ++i) {
			for (int j = 0; j < boardLength; ++j) {
				board[i][j] = Tile();
			}
		}
		// now add the bombs 
		for (auto p : pos) {
			board[p.getX()][p.getY()].setBomb(true);
		}
		// now set the neighbours
		remaining = 400;
		setNeighbours(*this);
	}
	Board(){}
	~Board(){}
	
	// getters and setters
	int getRemaining() {
		return remaining;
	}
	void setRemaining() {
		int squares = boardLength * boardWidth;
		int count = 0;

		for (int i = 0; i < boardLength; ++i) {
			for (int j = 0; j < boardWidth; ++j) {
				if (getTile(Position(i, j), *this).isRevealed()
					|| getTile(Position(i, j), *this).isFlagged()) {
					count++;
				}
			}
		}
		remaining = squares - count;
	}
	auto getBoard() {
		return board;
	}
	Tile &getTile(Position pos, Board &b) {
		return b.getBoard()[pos.getX()][pos.getY()];
	}
	void setTileOnBoard(Tile tile, Position pos, Board &b) {
		b.getBoard()[pos.getX()][pos.getY()] = tile;
	}
	void setNeighbours(Board& b) {
		// create copy of board which holds chars 'y' if bomb and 'n' otherwise
		char temp[boardLength][boardWidth];
		for (int i = 0; i < boardLength; ++i) {
			for (int j = 0; j < boardWidth; ++j) {
				if (getTile(Position(i, j), b).isBomb()) {
					temp[i][j] = 'y';
				}
				else {
					temp[i][j] = 'n';
				}
			}
		}
		int temp2[boardLength][boardWidth];	// number of neighbours
		for (int i = 0; i < boardLength; ++i) {
			for (int j = 0; j < boardWidth; ++j) {
				temp2[i][j] = 0;
			}
		}
		//edges
		for (int i = 1; i < boardWidth - 1; ++i) {
			if (temp[i][0] == 'y') {
				temp2[i][1]++;
				temp2[i - 1][0]++;
				temp2[i + 1][0]++;
				temp2[i - 1][1]++;
				temp2[i + 1][1]++;
			}
			if (temp[i][boardWidth - 1] == 'y') {
				temp2[i][boardWidth - 2]++;
				temp2[i - 1][boardWidth - 2]++;
				temp2[i + 1][boardWidth - 2]++;
				temp2[i - 1][boardWidth - 1]++;
				temp2[i + 1][boardWidth - 1]++;
			}
		}
		for (int i = 1; i < boardLength - 1; ++i) {
			if (temp[0][i] == 'y') {
				temp2[1][i + 1]++;
				temp2[0][i - 1]++;
				temp2[0][i + 1]++;
				temp2[1][i - 1]++;
				temp2[1][i]++;
			}
			if (temp[boardWidth - 1][i] == 'y') {
				temp2[boardWidth - 2][i]++;
				temp2[boardWidth - 2][i - 1]++;
				temp2[boardWidth - 2][i + 1]++;
				temp2[boardWidth - 1][i - 1]++;
				temp2[boardWidth - 1][i + 1]++;
			}
		}
		//corners
		if (temp[0][0] == 'y') {
			temp2[0][1]++;
			temp2[1][1]++;
			temp2[1][0]++;
		}
		if (temp[boardLength - 1][0] == 'y') {
			temp2[boardLength - 1][1]++;
			temp2[boardLength - 2][1]++;
			temp2[boardLength - 2][0]++;
		}
		if (temp[0][boardLength - 1] == 'y') {
			temp2[1][boardLength - 1]++;
			temp2[1][boardLength - 2]++;
			temp2[0][boardLength - 2]++;
		}
		if (temp[boardWidth - 1][boardLength - 1] == 'y') {
			temp2[boardWidth - 2][boardLength - 1]++;
			temp2[boardWidth - 2][boardLength - 2]++;
			temp2[boardWidth - 1][boardLength - 2]++;
		}
		//central squares
		for (int i = 1; i < boardLength - 1; ++i) {
			for (int j = 1; j < boardWidth - 1; ++j) {
				if (temp[i][j] == 'y') {
					temp2[i][j - 1]++;
					temp2[i][j + 1]++;
					temp2[i - 1][j]++;
					temp2[i + 1][j + 1]++;
					temp2[i + 1][j - 1]++;
					temp2[i + 1][j]++;
					temp2[i - 1][j + 1]++;
					temp2[i - 1][j - 1]++;
				}
			}
		}
		// set neighbours in board corresponding to temp2
		for (int i = 0; i < boardLength; ++i) {
			for (int j = 0; j < boardWidth; ++j) {
				Tile& tile = getTile(Position(i, j), b);
				tile.setNeighbours(temp2[i][j]);
			}
		}

	}
	
	// other methods
	void printBoard() {
		setRemaining();
		cout << "Remaining: " << remaining <<"\t Bombs:"<<countBombs()<< endl;
		cout << "   0  1  2  3  4  5  6  7  8  9 10"<<
			" 11 12 13 14 15 16 17 18 19"<<endl;
		for (int i = 0; i < boardLength; ++i) {
			if (i < 10) {
				cout << " " << i;
			}
			else{ 
				cout << i; 
			}
			for (int j = 0; j < boardWidth; ++j) {
				if (!board[i][j].isRevealed() && !board[i][j].isFlagged()) {
					cout << "|*|";
				}
				else if (board[i][j].isRevealed() && board[i][j].isBomb()) {
					cout << "|B|";
				}
				else if (board[i][j].isRevealed() && !board[i][j].isBomb()) {
					cout << "|" << board[i][j].getNeighbours() << "|";
				}
				else if (board[i][j].isFlagged()) {
					cout << "|F|";
				}									
				
			}
			cout << endl;
		}
	}
	void revealAllTiles() {
		for (int i = 0; i < boardLength; ++i) {
			for (int j = 0; j < boardWidth; ++j) {
				getTile(Position(i, j), *this).setRevealed(true);
			}
		}
	}
	void revealNeighbours(Position pos, Board& b) {
		/*reveal all neighbours of the tile at position pos 
		 If a neighbour is revealed to be a 0: call this function again on 
		 that square. Note to avoid calling this function infinitely 
		 we only check unrevealed squares*/

		int x = pos.getX();
		int y = pos.getY();
		// if pos is a corner square
		if (x == 0 && y == 0) {
			if (!b.getTile(Position(1, 0), b).isRevealed()) {
				b.getTile(Position(1, 0), b).setRevealed(true);
				if (b.getTile(Position(1,0), b).getNeighbours() == 0) {
					revealNeighbours(Position(1,0), b);
				}
			}
			if (!b.getTile(Position(1, 1), b).isRevealed()) {
				b.getTile(Position(1, 1), b).setRevealed(true);
				if (b.getTile(Position(1, 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(1, 1), b);
				}
			}
			if (!b.getTile(Position(0, 1), b).isRevealed()) {
				b.getTile(Position(0, 1), b).setRevealed(true);
				if (b.getTile(Position(0,1), b).getNeighbours() == 0) {
					revealNeighbours(Position(0,1), b);
				}
			}
		}
		else if (x == 0 && y == boardWidth-1) {
			if(!b.getTile(Position(0, y-1), b).isRevealed()) {
				b.getTile(Position(0, y-1), b).setRevealed(true);
				if (b.getTile(Position(0, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(0, y - 1), b);
				}
			}
			if (!b.getTile(Position(1, y-1), b).isRevealed()) {
				b.getTile(Position(1, y - 1), b).setRevealed(true);
				if (b.getTile(Position(1, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(1, y - 1), b);
				}
			}
			if (!b.getTile(Position(1, y), b).isRevealed()) {
				b.getTile(Position(1, y), b).setRevealed(true);
				if (b.getTile(Position(1, y), b).getNeighbours() == 0) {
					revealNeighbours(Position(1, y), b);
				}
			}
		}
		else if (x == boardLength-1 && y == 0) {
			if (!b.getTile(Position(x-1, 0), b).isRevealed()) {
				b.getTile(Position(x-1, 0), b).setRevealed(true);
				if (b.getTile(Position(x-1, 0), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, 0), b);
				}
			}
			if (!b.getTile(Position(x-1, 1), b).isRevealed()) {
				b.getTile(Position(x-1, 1), b).setRevealed(true);
				if (b.getTile(Position(x-1, 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, 1), b);
				}
			}
			if (!b.getTile(Position(x, 1), b).isRevealed()) {
				b.getTile(Position(x, 1), b).setRevealed(true);
				if (b.getTile(Position(x, 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x, 1), b);
				}
			}
		}
		else if (x == boardLength-1 && y == boardWidth-1) {
			if (!b.getTile(Position(x, y-1), b).isRevealed()) {
				b.getTile(Position(x, y - 1), b).setRevealed(true);
				if (b.getTile(Position(x, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x, y - 1), b);
				}
			}
			if (!b.getTile(Position(x-1, y), b).isRevealed()) {
				b.getTile(Position(x-1, y), b).setRevealed(true);
				if (b.getTile(Position(x-1, y), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, y), b);
				}
			}
			if (!b.getTile(Position(x-1, y-1), b).isRevealed()) {
				b.getTile(Position(x-1, y-1), b).setRevealed(true);
				if (b.getTile(Position(x-1, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, y - 1), b);
				}
			}
		}
		// central square
		else if (x > 0 && x < boardLength - 1 && y > 0 && y < boardWidth - 1) {
			if (!b.getTile(Position(x, y - 1), b).isRevealed()) {
				b.getTile(Position(x, y - 1), b).setRevealed(true);
				if (b.getTile(Position(x, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x, y - 1), b);
				}
			}
			if (!b.getTile(Position(x, y + 1), b).isRevealed()) {
				b.getTile(Position(x, y + 1), b).setRevealed(true);
				if (b.getTile(Position(x, y + 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x, y + 1), b);
				}
			}
			if (!b.getTile(Position(x - 1, y - 1), b).isRevealed()) {
				b.getTile(Position(x - 1, y - 1), b).setRevealed(true);
				if (b.getTile(Position(x - 1, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x - 1, y - 1), b);
				}
			}
			if (!b.getTile(Position(x - 1, y), b).isRevealed()) {
				b.getTile(Position(x - 1, y), b).setRevealed(true);
				if (b.getTile(Position(x - 1, y), b).getNeighbours() == 0) {
					revealNeighbours(Position(x - 1, y), b);
				}
			}
			if (!b.getTile(Position(x - 1, y + 1), b).isRevealed()) {
				b.getTile(Position(x - 1, y + 1), b).setRevealed(true);
				if (b.getTile(Position(x - 1, y + 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x - 1, y + 1), b);
				}
			}
			if (!b.getTile(Position(x + 1, y - 1), b).isRevealed()) {
				b.getTile(Position(x + 1, y - 1), b).setRevealed(true);
				if (b.getTile(Position(x + 1, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x + 1, y - 1), b);
				}
			}
			if (!b.getTile(Position(x + 1, y), b).isRevealed()) {
				b.getTile(Position(x + 1, y), b).setRevealed(true);
				if (b.getTile(Position(x + 1, y), b).getNeighbours() == 0) {
					revealNeighbours(Position(x + 1, y), b);
				}
			}
			if (!b.getTile(Position(x + 1, y + 1), b).isRevealed()) {
				b.getTile(Position(x + 1, y + 1), b).setRevealed(true);
				if (b.getTile(Position(x + 1, y + 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x + 1, y + 1), b);
				}
			}		
		}
		// edge but not corner
		else if (x == 0 && y != 0 && y != boardWidth - 1) {
			if (!b.getTile(Position(0, y - 1), b).isRevealed()) {
				b.getTile(Position(0, y - 1), b).setRevealed(true);
				if (b.getTile(Position(0, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(0, y - 1), b);
				}
			}
			if (!b.getTile(Position(1, y - 1), b).isRevealed()) {
				b.getTile(Position(1, y - 1), b).setRevealed(true);
				if (b.getTile(Position(1, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(1, y - 1), b);
				}
			}
			if (!b.getTile(Position(1, y), b).isRevealed()) {
				b.getTile(Position(1, y), b).setRevealed(true);
				if (b.getTile(Position(1, y), b).getNeighbours() == 0) {
					revealNeighbours(Position(1, y), b);
				}
			}
			if (!b.getTile(Position(0, y + 1), b).isRevealed()) {
				b.getTile(Position(0, y + 1), b).setRevealed(true);
				if (b.getTile(Position(0, y + 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(0, y + 1), b);
				}
			}
			if (!b.getTile(Position(1, y + 1), b).isRevealed()) {
				b.getTile(Position(1, y + 1), b).setRevealed(true);
				if (b.getTile(Position(1, y + 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(1, y + 1), b);
				}
			}
		}
		else if (x == boardLength-1 && y != 0 && y != boardWidth - 1) {
			if (!b.getTile(Position(x, y - 1), b).isRevealed()) {
				b.getTile(Position(x, y - 1), b).setRevealed(true);
				if (b.getTile(Position(x, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x, y - 1), b);
				}
			}
			if (!b.getTile(Position(x-1, y - 1), b).isRevealed()) {
				b.getTile(Position(x-1, y - 1), b).setRevealed(true);
				if (b.getTile(Position(x-1, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, y - 1), b);
				}
			}
			if (!b.getTile(Position(x-1, y), b).isRevealed()) {
				b.getTile(Position(x-1, y), b).setRevealed(true);
				if (b.getTile(Position(x-1, y), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, y), b);
				}
			}
			if (!b.getTile(Position(x, y + 1), b).isRevealed()) {
				b.getTile(Position(x, y + 1), b).setRevealed(true);
				if (b.getTile(Position(x, y + 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x, y + 1), b);
				}
			}
			if (!b.getTile(Position(x-1, y + 1), b).isRevealed()) {
				b.getTile(Position(x-1, y + 1), b).setRevealed(true);
				if (b.getTile(Position(x-1, y +1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, y + 1), b);
				}
			}
		}
		else if (y == 0 && x != 0 && x != boardLength - 1) {
			if (!b.getTile(Position(x+1, 0), b).isRevealed()) {
				b.getTile(Position(x+1, 0), b).setRevealed(true);
				if (b.getTile(Position(x+1, 0), b).getNeighbours() == 0) {
					revealNeighbours(Position(x+1, 0), b);
				}
			}
			if (!b.getTile(Position(x-1, 0), b).isRevealed()) {
				b.getTile(Position(x - 1, 0), b).setRevealed(true);
				if (b.getTile(Position(x-1, 0), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, 0), b);
				}
			}
			if (!b.getTile(Position(x-1, 1), b).isRevealed()) {
				b.getTile(Position(x - 1, 1), b).setRevealed(true);
				if (b.getTile(Position(x-1, 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, 1), b);
				}
			}
			if (!b.getTile(Position(x, 1), b).isRevealed()) {
				b.getTile(Position(x, 1), b).setRevealed(true);
				if (b.getTile(Position(x, 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x, 1), b);
				}
			}
			if (!b.getTile(Position(x+1, 1), b).isRevealed()) {
				b.getTile(Position(x + 1, 1), b).setRevealed(true);
				if (b.getTile(Position(x+1, 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x+1, 1), b);
				}
			}
		}
		else if (y == boardWidth - 1 && x != 0 && x != boardLength - 1) {
			if (!b.getTile(Position(x-1, y), b).isRevealed()) {
				b.getTile(Position(x-1, y), b).setRevealed(true);
				if (b.getTile(Position(x-1, y), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, y), b);
				}
			}
			if (!b.getTile(Position(x+1, y), b).isRevealed()) {
				b.getTile(Position(x + 1, y), b).setRevealed(true);
				if (b.getTile(Position(x+1, y), b).getNeighbours() == 0) {
					revealNeighbours(Position(x+1, y), b);
				}
			}
			if (!b.getTile(Position(x-1, y - 1), b).isRevealed()) {
				b.getTile(Position(x - 1, y-1), b).setRevealed(true);
				if (b.getTile(Position(x-1, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x-1, y - 1), b);
				}
			}
			if (!b.getTile(Position(x, y - 1), b).isRevealed()) {
				b.getTile(Position(x, y - 1), b).setRevealed(true);
				if (b.getTile(Position(x, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x, y - 1), b);
				}
			}
			if (!b.getTile(Position(x+1, y - 1), b).isRevealed()) {
				b.getTile(Position(x + 1, y - 1), b).setRevealed(true);
				if (b.getTile(Position(x+1, y - 1), b).getNeighbours() == 0) {
					revealNeighbours(Position(x+1, y - 1), b);
				}
			}
		}	
	}
	int countBombs() {
		// count number of unflagged bombs
		int count = 0;
		for (int i = 0; i < boardLength; ++i) {
			for (int j = 0; j < boardWidth; ++j) {
				if (getTile(Position(i, j), *this).isBomb()&&
					!getTile(Position(i,j),*this).isFlagged()) {
					count++;
				}
					
			}
		}
		return count;
	}
};

