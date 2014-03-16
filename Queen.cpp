#include "Queen.h"

Queen::Queen(Position position, PIECE_COLOR pieceColor) : Piece(position, pieceColor) {
	type = PIECE_TYPES::QUEEN;
}