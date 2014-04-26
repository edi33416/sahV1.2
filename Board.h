#pragma once

#ifndef Board_H
#define Board_H

#include <vector>
#include "Piece.h"
#include "Pieces.h"
#include <fstream>
//#include "Engine.h"
#include "utils.h"
//#include "Move.h"
//#include "BasicMove.h"
//#include "CastlingMove.h"

typedef std::vector<Piece*> PIECE_SET;
typedef BITBOARD* BITBOARD_SET;
typedef char DIRECTION;

#define LEFT 1
#define RIGHT -1
#define CASTLING_DISTANCE 2
#define opposite_direction(x) (-(x))
#define ROOK_ORIGINAL_POSITION ((king->currentPosition) + (((1 + castlingDirection) >> 1) + 2) * castlingDirection)

class Board {
private:
	void removeFromBitboards(BITBOARD &bitboard, Position position);
	BITBOARD genNegativeMoves(const Position position, const Position direction);
	BITBOARD genPositiveMoves(const Position position, const Position direction);

	inline Piece* createPiece(PIECE_TYPES type, Piece *oldPiece);

public:
	bool canCastle;
	class Move {
	public:
		Position newPosition, oldPosition;
		static Board *board;
		bool isCastling;
		//Move();
		virtual void undo() = 0;
		virtual void apply() = 0;
	};

	class BasicMove : public Move {
	private:
		inline bool isCastlingPiece();
	public:
		Piece *piece1, *piece2;
		BasicMove(Piece *p1, Position newPosition);
		void apply();
		void undo();
	};

	class CastlingMove : public Move {
	private:
		King *king;
		Rook *rook;
		DIRECTION castlingDirection;	//king castling direction

	public:
		CastlingMove(King *k, Rook *r);
		void apply();
		void undo();
	};

	class PawnPromotion : public Move {
	public:
		Piece *piece, *newPiece;
		PIECE_TYPES newPieceType;
		PawnPromotion(Piece *_piece, Position newPosition, PIECE_TYPES _newPieceType);
		void apply();
		void undo();
	};

public:

	void printBitboard(BITBOARD boardToPrint);
	void printDebug();
	//TODO PRIVATE

	void init();
	void erase();

	BITBOARD board, boardsVector[2];
	// Moves for each type of piece in one step
	// 
	BITBOARD_SET nextStep[2];
	PIECE_SET piecesVector[2][6];
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
	void putOnBoard(Piece *piece);
	Board();
	~Board();
};

#endif