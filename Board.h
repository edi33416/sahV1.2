#pragma once

#ifndef Board_H
#define Board_H

#include <vector>
#include "Piece.h"
#include "Pieces.h"
#include <fstream>
#include "utils.h"
#include <unordered_map>
#include <random>
//#include "directions.h"

typedef std::vector<Piece*> PIECE_SET;
typedef BITBOARD* BITBOARD_SET;
typedef char DIRECTION;

#define LEFT 1
#define RIGHT -1
#define CASTLING_DISTANCE 2
#define opposite_direction(x) (-(x))
#define ROOK_ORIGINAL_POSITION ((king->currentPosition) + (((1 + castlingDirection) >> 1) + 3) * castlingDirection)
#define pieceAt(position) (*(*(allPieces) + (position)))
#define isOnLastRow(position) ((((position) >> 3) == 0) || (((position) >> 3) == 7))
#define otherColor(color) ((PIECE_COLOR) (WHITE - (color)))

class Board {
public:
	bool canCastle;
	class Move {
	public:
		Position newPosition, oldPosition;
		static Board *board;
		bool isCastling;
		Move() {};
		virtual void undo() = 0;
		virtual void apply() = 0;
		virtual bool isCapture() = 0;
		Piece *oldEligableEnPassant = nullptr;
	};

	class BasicMove : public Move {
	private:
		inline bool isCastlingPiece();
	public:
		BasicMove() {};
		Piece *piece1, *piece2;
		BasicMove(Piece *p1, Position newPosition);
		virtual void apply();
		virtual void undo();
		bool isCapture();
		void set(Piece *piece, Position newPosition);
	};

	class CastlingMove : public Move {
	private:
		DIRECTION castlingDirection;	//king castling direction

	public:
		CastlingMove() {};
		CastlingMove(King *k, Rook *r);
		King *king;
		Rook *rook;
		void set(King *k, Rook *r);
		void apply();
		void undo();
		bool isCapture();
	};

	class PawnPromotion : public Move {
	public:

		PawnPromotion(){}
		PawnPromotion(Piece *p, Position newPosition, PIECE_TYPES newType);
		Piece *pawn, *newPiece, *removed;
		PIECE_TYPES newType;
		void set(Piece *p, Position newPosition, PIECE_TYPES newType);
		void apply();
		void undo();
		bool isCapture();
	};

	class PawnMove : public BasicMove {
	private:
		char direction;
	public:
		PawnMove() {}
		PawnMove(Piece *p1, Position newPosition) : BasicMove(p1, newPosition){}
		void apply();
		void undo();
		bool isCapture();
	};

	class EnPassant : public Move {
	private:
		Piece *pawn, *removed;
		Position oldPosition;
	public:

		EnPassant() {}
		EnPassant(Piece *p, Piece *removed);
		void set(Piece *p, Piece *removed);
		void apply();
		void undo();
		bool isCapture();
	};

	typedef struct MoveScore {
		Move *move;
		int score;
		PIECE_COLOR playerColor = BLACK;
		MoveScore(Move *m, int s) {
			move = m;
			score = s;
		}
		void rotatePositions() {

			move->oldPosition = 63 - move->oldPosition;
			move->newPosition = 63 - move->newPosition;
		}
		MoveScore() {}

	};


public:
	// Adding piece square tables
	static const short int pieceSquareTables[7][64];
	static const char KING_END_GAME = 6;

	std::vector<Piece*> tempRemovedPieces;
	
	void printBitboard(BITBOARD boardToPrint);
	void printDebug();
	//TODO PRIVATE
	void printPointerBoard(PIECE_COLOR);
	std::unordered_map<Piece*, std::vector<Move*> > movesMap;

	void init();
	void erase();

	BITBOARD board, boardsVector[2];

	// Moves for each type of piece in one step
	BITBOARD_SET nextStep[2];

	PIECE_SET piecesVector[2][6];

	// A matrix of pointers to all the chess pieces
	Piece *allPieces[8][8];

	bool isMovable(PIECE_TYPES pieceType, PIECE_COLOR pieceColor);
	bool isCheckMate(PIECE_COLOR playerColor);
	Piece* movePiece(Piece *piece, Position newPosition);
	int evaluate(PIECE_COLOR playerColor);

	//MODFICAT
	inline bool pathClearForCastl(Rook *rook);
	std::vector<Move*> getPossiblePosition(Piece *piece);
	void removePiece(Piece *piece);
	void updateNextMoves(PIECE_TYPES pieceType, PIECE_COLOR pieceColor);
	BITBOARD getPossibleMoves(Piece *piece);
	Board();
	~Board();
	void applyInputMove(Position oldPosition, Position newPosition, char lastChar);
	PIECE_TYPES getPieceType(char c);
	Piece *_EPpawn = nullptr;	//en passant pawn

	// For en passant
	void setEnPassant(Pawn* pawn);
	void addToHash(MoveScore m, PIECE_COLOR playerColor);
	bool hasBeenEvald(PIECE_COLOR playerColor);
	MoveScore getMove();
	void printHash();

private:
	typedef struct HashVal {
		MoveScore move;
		bool stillUseful = true;
		HashVal(MoveScore &m) {
			move = m;
		}

	};

	ULL pieceKeys[2][6][64];
	MoveScore evaldMove;
	void putOnBoard(Piece *piece);
	Piece* createPiece(PIECE_TYPES type, Piece *oldPiece);
	void removeFromBitboards(BITBOARD &bitboard, Position position);
	BITBOARD genNegativeMoves(const Position position, const Position direction);
	BITBOARD genPositiveMoves(const Position position, const Position direction);
	static const int valueOfAttack[6];
	static const int attackWeight[12];
	static const int directions[8];
	static const int materialScores[6];

	int material[2];
	int getPieceScore(Piece *p);

	std::unordered_map<ULL, HashVal> evalMap;
	ULL calcBoardKey(PIECE_COLOR playerColor);
	//void saveMove(HashVal v);

	void recalcMoves(Position position);
	std::vector<Piece*> dependentPieces[64];
	void setDependences(Piece *piece);
	//std::vector<Move*> ownColorDepend(Piece *piece);
};

#endif