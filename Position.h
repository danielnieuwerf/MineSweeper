#pragma once
class Position
{
private:
	int x, y;
public:
	// constructors and destructor
	Position(int a, int b) {
		x = a;
		y = b;
	}
	Position() {
		x = 0;
		y = 0;
	}
	~Position() {

	}

	//getters and setters
	void setX(int a) {
		x = a;
	}
	void setY(int b) {
		y = b;
	}
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}


};

