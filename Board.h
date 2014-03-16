#pragma once

#ifndef Board_H
#define Board_H

#include <vector>
#include "Piece.h"
#include "Pieces.h"
#include <fstream>
//#include "Engine.h"
#include "utils.h"

typedef std::vector<Piece*> PIECE_SET;
typedef BITBOARD* BITBOARD_SET;

class Board {
private:
	void removeFromBitboards(BITBOARD &bitboard, Position position);


public:
	
	void printBitboard(BITBOARD boardToPrint);
	void printDebug();
	//TODO PRIVATE

	void init();
	void erase();

	BITBOARD whiteBoard, blackBoard, board;
	// Moves for each type of piece in one step
	// 
	BITBOARD_SET whitesNextStep, blacksNextStep;
	PIECE_SET whitePieces[6], blackPieces[6];
	Piece *allPieces[8][8];

	Command moveKnight();
	bool isMovable(PIECE_TYPES pieceType, PIECE_COLOR pieceColor);
	bool isCheckMate();
	void movePiece(Piece *piece, Position newPosition);
	Position getPossiblePosition(Piece *piece);
	void removePiece(Position position);
	void updateNextMoves(PIECE_TYPES pieceType, PIECE_COLOR pieceColor);
	BITBOARD getPossibleMoves(Piece *piece);
	Board();
	~Board();
};

#endif