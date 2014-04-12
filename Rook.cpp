#include "Rook.h"

Rook::Rook(Position position, PIECE_COLOR pieceColor) : CastlingPiece(position, pieceColor) {
	type = PIECE_TYPES::ROOKS;
}