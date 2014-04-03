#pragma once

#ifndef King_H
#define King_H

#include "utils.h"
#include "Piece.h"

class King : public Piece {
private:
public:
	bool wasMoved;
	const static BITBOARD movesMatrix[];
	BITBOARD getAllMoves();
	King();
	King(Position position, PIECE_COLOR pieceColor);
};

#endif