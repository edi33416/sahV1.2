#pragma once

#ifndef Pawn_H
#define Pawn_H

#include "Piece.h"

class Pawn : public Piece {
private:
	/*const static BITBOARD blackForwardMoves[];
	const static BITBOARD blackAttackMoves[];
	const static BITBOARD whiteForwardMoves[];
	const static BITBOARD whiteAttackMoves[];
	*/

	const static BITBOARD forwardMoves[2][64];
	const static BITBOARD attackMoves[2][64];
public:
	Pawn(Position position, PIECE_COLOR pieceColor);
	BITBOARD getAllMoves();
	BITBOARD getForwardMoves();
	BITBOARD getAttackMoves();
	bool isOnStartingPosition();
	bool eligibleForEP;
	Piece *EP1 = nullptr, *EP2 = nullptr;
};

#endif