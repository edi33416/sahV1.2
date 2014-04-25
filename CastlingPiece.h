#pragma once

#include "Piece.h"

class CastlingPiece : public Piece {
private:
public:
	unsigned int moveCount;

	CastlingPiece(Position, PIECE_COLOR color);
	void undoLastMove(Position oldPosition);
	bool canCastle();
};