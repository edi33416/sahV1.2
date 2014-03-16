#include "Piece.h"

void Piece::move(Position newPosition) {
	currentPosition = newPosition;
}

Piece::Piece(Position position, PIECE_COLOR pieceColor) : currentPosition(position), color(pieceColor) {}

Position Piece::getPosition() {
	return currentPosition;
}
