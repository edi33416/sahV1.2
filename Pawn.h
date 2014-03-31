#pragma once

#ifndef Pawn_H
#define Pawn_H

#include "Piece.h"

class Pawn : public Piece {
private:
	const static BITBOARD blackForwardMoves[];
	const static BITBOARD blackAttackMoves[];
	const static BITBOARD whiteForwardMoves[];
	const static BITBOARD whiteAttackMoves[];

public:
	Pawn(Position position, PIECE_COLOR pieceColor);
	BITBOARD getAllMoves();
	BITBOARD getForwardMoves();
	BITBOARD getAttackMoves();
	void printtmp();
	bool isOnStartingPosition();
};

#endif