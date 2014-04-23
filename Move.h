#pragma once

#include "Piece.h"
#include "IBoard.h"

typedef char DIRECTION;

class Move {
protected:
		//Piece *piece1, *piece2;
	
public:
	Position newPosition, oldPosition;
	//static int *a;
	static IBoard *board;
	Move();
	virtual void undo() = 0;
	virtual void apply() = 0;

};