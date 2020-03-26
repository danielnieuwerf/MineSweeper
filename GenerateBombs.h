#pragma once
#include "Board.h"
#include "Position.h"
#include <vector>
using namespace std;
class GenerateBombs
{
private:
	vector<Position> bombs;
public:
	GenerateBombs(int x) {
		vector<Position> temp;
		while (x) {
			// possibility of duplicate positions
			temp.push_back(Position(rand() % 20, rand() % 20));
			x--;
		}
		bombs = temp;
	}
	~GenerateBombs() {}
	// getter
	auto getBombs() {
		return bombs;
	}
};

