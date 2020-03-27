#pragma once
class Tile
{
	bool revealed;
	bool flagged;
	bool bomb;
	int neighbours;

public:
	//getters
	bool &isRevealed() {
		return revealed;
	}
	bool &isFlagged() {
		return flagged;
	}
	bool &isBomb() {
		return bomb;
	}
	int &getNeighbours() {
		return neighbours;
	}
	bool isFlaggedOrBomb() {
		return (isFlagged() || isBomb());
	}
	//setters
	void setRevealed(bool r) {
		revealed = r;
	}
	void setFlagged(bool r) {
		flagged = r;
	}
	void setBomb(bool r) {
		bomb = r;
	}
	void setNeighbours(int x) {
		neighbours = x;
	}
	//constructors
	Tile(bool a, bool b, bool c, int d) {
		revealed = a;
		flagged = b;
		bomb = c;
		neighbours = d;
	}
	Tile() {
		// contructs tile that is not a bomb and is neither revealed nor flagged
		revealed = false;
		flagged = false;
		bomb = false;
		neighbours = 0;
	}
	// destructor
	~Tile() {}
};

