#include "Rook.h"

Rook::Rook(Position position, PIECE_COLOR pieceColor) : Piece(position, pieceColor) {
	type = PIECE_TYPES::ROOKS;
}