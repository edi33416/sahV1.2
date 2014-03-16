#include "King.h"

King::King(Position position, PIECE_COLOR pieceColor) : Piece(position, pieceColor) {
	type = PIECE_TYPES::KING;
}