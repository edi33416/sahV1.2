#include "CastlingPiece.h"

CastlingPiece::CastlingPiece(Position position, PIECE_COLOR color) : Piece(position, color), moveCount(0) {}

void CastlingPiece::move(Position newPosition) {
	moveCount++;
	Piece::move(newPosition);
}

void CastlingPiece::undoLastMove(Position oldPosition) {
	moveCount--;
	Piece::move(oldPosition);
}

bool CastlingPiece::canCastle() {
	return moveCount == 0;
}