#pragma once

#ifndef Knight_H
#define Knight_H

#include <iostream>

#include "Piece.h"
#include "Bitboard.h"

class Knight : public Piece {
private:

public:
	static const BITBOARD movesMatrix[];
	Knight();
	Knight(Position position, PIECE_COLOR pieceColor);
	BITBOARD getAllMoves();

};


#endif