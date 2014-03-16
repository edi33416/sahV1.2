#include "Bishop.h"

Bishop::Bishop(Position position, PIECE_COLOR pieceColor) : Piece(position, pieceColor) {
	type = PIECE_TYPES::BISHOPS;
}