#pragma once

#ifndef Queen_H
#define Queen_H

#include "Piece.h"

class Queen : public Piece {
private:
	const static BITBOARD moves;
public:
	Queen(Position position, PIECE_COLOR pieceColor);
};

#endif