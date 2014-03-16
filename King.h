#pragma once

#ifndef King_H
#define King_H

#include "utils.h"
#include "Piece.h"

class King : public Piece {
private:
	const static BITBOARD allMoves;
public:
	King();
	King(Position position, PIECE_COLOR pieceColor);
};

#endif