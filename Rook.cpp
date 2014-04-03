#include "Rook.h"

Rook::Rook(Position position, PIECE_COLOR pieceColor) : Piece(position, pieceColor), wasMoved(false) {
	type = PIECE_TYPES::ROOKS;
}