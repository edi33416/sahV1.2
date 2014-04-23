#pragma once

#include "Move.h"
#define LEFT 1
#define RIGHT -1
#define CASTLING_DISTANCE 2
#define opposite_direction(x) ((-1) * (x))
#define ROOK_ORIGINAL_POSITION ((king->currentPosition) + (((1 + castlingDirection) >> 1) + 2) * castlingDirection)
//#define KING_ORIGINAL_POSTION ((king->currentPosition))

class CastlingMove : public Move {
private:
	King *king;
	Rook *rook;
	DIRECTION castlingDirection;	//king castling direction

public:
	CastlingMove(King *k, Rook *r);
	void apply();
	void undo();
};