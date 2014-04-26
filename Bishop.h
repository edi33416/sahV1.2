#pragma once

#ifndef Bishop_H
#define Bishop_H

#include "Piece.h"

class Bishop : public Piece {
private:
	const static BITBOARD moves;

public:
	Bishop(Position position, PIECE_COLOR pieceColor);
	Bishop();
};

#endif