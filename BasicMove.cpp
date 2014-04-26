#include "BasicMove.h"

void BasicMove::apply() {

	oldPosition = piece1->currentPosition;
	piece2 = board->movePiece(piece1, newPosition);
}

void BasicMove::undo() {

	board->movePiece(piece1, oldPosition);
	if (piece2 != nullptr) {
		board->piecesVector[piece2->color][piece2->type].push_back(piece2);
		board->putOnBoard(piece2);
	}
}

BasicMove::BasicMove(Piece *p1, Position newPosition) : piece1(p1) {
	this->newPosition = newPosition;
}