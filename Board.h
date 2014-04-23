#pragma once

#ifndef Board_H
#define Board_H

#include <vector>
#include "Piece.h"
#include "Pieces.h"
#include <fstream>
//#include "Engine.h"
#include "utils.h"
#include "Move.h"
#include "IBoard.h"
#include "BasicMove.h"
#include "CastlingMove.h"

typedef BITBOARD* BITBOARD_SET;

class Board : public IBoard {
private:
	void removeFromBitboards(BITBOARD &bitboard, Position position);
	BITBOARD genNegativeMoves(const Position position, const Position direction);
	BITBOARD genPositiveMoves(const Position position, const Position direction);

public:
	std::vector<Piece*> tempRemovedPieces;

	
	void printBitboard(BITBOARD boardToPrint);
	void printDebug();
	//TODO PRIVATE

	void init();
	void erase();

	BITBOARD board, boardsVector[2];
	// Moves for each type of piece in one step
	// 
	BITBOARD_SET nextStep[2];
	Piece *allPieces[8][8];

	Command moveKnight();
	bool isMovable(PIECE_TYPES pieceType, PIECE_COLOR pieceColor);
	bool isCheckMate();
	Piece* movePiece(Piece *piece, Position newPosition);
	void tempMovePiece(Piece *piece, Position newPosition); 
	void pawnPromotion(Piece *piece);

	//MODFICAT
	inline bool pathClearForCastl(Rook *rook);
	std::vector<Move*> getPossiblePosition(Piece *piece);
	void removePiece(Piece *piece);
	void updateNextMoves(PIECE_TYPES pieceType, PIECE_COLOR pieceColor);
	BITBOARD getPossibleMoves(Piece *piece);
	void undoMove(Piece *piece, Position oldPosition);
	void putOnBoard(Piece *piece);
	Board();
	~Board();
};

#endif