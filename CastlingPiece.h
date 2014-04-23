#pragma once

#include "Piece.h"

class CastlingPiece : public Piece {
private:
	unsigned int moveCount;
public:
	CastlingPiece(Position, PIECE_COLOR color);
	void move(Position newPosition);
	void undoLastMove(Position oldPosition);
	bool canCastle();
};