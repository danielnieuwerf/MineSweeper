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
	Board(){
		for (int i = 0; i < boardLength; ++i) {
			for (int j = 0; j < boardWidth; ++j) {
				board[i][j] = Tile();
			}
		}
	}
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

		// set temp2 to the correct values
		for (int i = 0; i < boardLength; ++i) {
			for (int j = 0; j < boardWidth; ++j) {
				if (temp[i][j] == 'y') {
					if (j != 0) {
						temp2[i][j - 1]++;
					}
					if (j != boardWidth - 1) {
						temp2[i][j + 1]++;
					}
					if (i != 0) {
						temp2[i - 1][j]++;
					}
					if (i != boardLength - 1 && j != boardWidth - 1) {
						temp2[i + 1][j + 1]++;
					}
					if (i != boardLength - 1 && j != 0) {
						temp2[i + 1][j - 1]++;
					}
					if (i != boardLength - 1) {
						temp2[i + 1][j]++;
					}
					if (i != 0 && j != boardWidth - 1) {
						temp2[i - 1][j + 1]++;
					}
					if (i != 0 && j != 0) {
						temp2[i - 1][j - 1]++;
					}
				}
			}
		}
		// set neighbours in board as in temp2
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
					board[i][j].getNeighbours()==0 ?
						cout << "| |": 
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

		if (y!=0 && !b.getTile(Position(x, y - 1), b).isRevealed()) {
			b.getTile(Position(x, y - 1), b).setRevealed(true);
			if (b.getTile(Position(x, y - 1), b).getNeighbours() == 0) {
				revealNeighbours(Position(x, y - 1), b);
			}
		}
		if (y!=boardWidth-1 && !b.getTile(Position(x, y + 1), b).isRevealed()) {
			b.getTile(Position(x, y + 1), b).setRevealed(true);
			if (b.getTile(Position(x, y + 1), b).getNeighbours() == 0) {
				revealNeighbours(Position(x, y + 1), b);
			}
		}
		if (x!=0 && y!=0 && !b.getTile(Position(x - 1, y - 1), b).isRevealed()) {
			b.getTile(Position(x - 1, y - 1), b).setRevealed(true);
			if (b.getTile(Position(x - 1, y - 1), b).getNeighbours() == 0) {
				revealNeighbours(Position(x - 1, y - 1), b);
			}
		}
		if (x!=0 && !b.getTile(Position(x - 1, y), b).isRevealed()) {
			b.getTile(Position(x - 1, y), b).setRevealed(true);
			if (b.getTile(Position(x - 1, y), b).getNeighbours() == 0) {
				revealNeighbours(Position(x - 1, y), b);
			}
		}
		if (x!=0 && y!=boardWidth-1 && !b.getTile(Position(x - 1, y + 1), b).isRevealed()) {
			b.getTile(Position(x - 1, y + 1), b).setRevealed(true);
			if (b.getTile(Position(x - 1, y + 1), b).getNeighbours() == 0) {
				revealNeighbours(Position(x - 1, y + 1), b);
			}
		}
		if (x!=boardLength-1 &&y!=0 && !b.getTile(Position(x + 1, y - 1), b).isRevealed()) {
			b.getTile(Position(x + 1, y - 1), b).setRevealed(true);
			if (b.getTile(Position(x + 1, y - 1), b).getNeighbours() == 0) {
				revealNeighbours(Position(x + 1, y - 1), b);
			}
		}
		if (x!=boardLength-1 && !b.getTile(Position(x + 1, y), b).isRevealed()) {
			b.getTile(Position(x + 1, y), b).setRevealed(true);
			if (b.getTile(Position(x + 1, y), b).getNeighbours() == 0) {
				revealNeighbours(Position(x + 1, y), b);
			}
		}
		if (x!=boardLength-1 && y!=boardWidth-1 && !b.getTile(Position(x + 1, y + 1), b).isRevealed()) {
			b.getTile(Position(x + 1, y + 1), b).setRevealed(true);
			if (b.getTile(Position(x + 1, y + 1), b).getNeighbours() == 0) {
				revealNeighbours(Position(x + 1, y + 1), b);
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
	bool boardIsClear() {
		return countBombs() == 0;
	}
};

