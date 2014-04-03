#pragma once

#ifndef Rook_H
#define Rook_H

#include "Piece.h"

class Rook : public Piece {
private:
	const static BITBOARD moves;
	

public:
	bool wasMoved;
	Rook(Position position, PIECE_COLOR pieceColor);
};

#endif