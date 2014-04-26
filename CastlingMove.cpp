#include "CastlingMove.h"

CastlingMove::CastlingMove(King *k, Rook *r) : king(k), rook(r) {
	if (k->currentPosition - r->currentPosition > 0)
		castlingDirection = RIGHT;
	else
		castlingDirection = LEFT;

	oldPosition = king->currentPosition;
}

void CastlingMove::apply() {
	board->movePiece(king, king->currentPosition + castlingDirection * CASTLING_DISTANCE);
	board->movePiece(rook, king->currentPosition + opposite_direction(castlingDirection));
	newPosition = king->currentPosition;
}

void CastlingMove::undo() {
	board->movePiece(rook, ROOK_ORIGINAL_POSITION);
	board->movePiece(king, king->currentPosition + CASTLING_DISTANCE * opposite_direction(castlingDirection));
}