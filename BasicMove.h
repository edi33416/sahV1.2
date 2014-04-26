#pragma once

#include "Move.h"

class BasicMove : public Move {
private:
	Piece *piece1, *piece2;
public:
	BasicMove(Piece *p1, Position newPosition);
	void apply();
	void undo();
};