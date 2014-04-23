#pragma once

#include "Piece.h"
#include "Board.h"
#include "CastlingMove.h"
#include "BasicMove.h"

typedef char DIRECTION;

class Move {
protected:
	//Piece *piece1, *piece2;
	//Position newPosition, oldPosition;
public:
	static Board *board;
	Move();
	virtual void undo() = 0;
	virtual void apply() = 0;
};