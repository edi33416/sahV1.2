#pragma once

#ifndef Piece_H
#define Piece_H

#include <vector>

#include "Bitboard.h"
#include "utils.h"

enum PIECE_TYPES {
	PAWNS, KNIGHTS, ROOKS, BISHOPS, KING, QUEEN
};

enum PIECE_COLOR {
	BLACK, WHITE
};

class Piece {

protected:
	
	
public:
	Position currentPosition;
	Piece (Position position, PIECE_COLOR pieceColor);
	void move(Position newPosition);
	virtual void undoLastMove(Position oldPosition);
	Position getPosition();
	virtual BITBOARD getAllMoves() { return 0; }


	PIECE_TYPES type;
	PIECE_COLOR color;

};

#endif