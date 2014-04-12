#pragma once

#ifndef Rook_H
#define Rook_H

#include "CastlingPiece.h"

class Rook : public CastlingPiece {
private:
	const static BITBOARD moves;

public:
	Rook(Position position, PIECE_COLOR pieceColor);
};

#endif