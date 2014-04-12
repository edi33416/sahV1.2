#pragma once

#ifndef King_H
#define King_H

#include "utils.h"
#include "CastlingPiece.h"

class King : public CastlingPiece {
private:
public:
	const static BITBOARD movesMatrix[];
	BITBOARD getAllMoves();
	King();
	King(Position position, PIECE_COLOR pieceColor);
};

#endif