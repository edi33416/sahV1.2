#include "Board.h"


const int Board::valueOfAttack[6] = { 10, 20, 40, 20, 0, 80 };
const int Board::attackWeight[12] = { 0, 0, 50, 75, 88, 94, 97, 99, 101, 103, 105, 107 };

const int Board::directions[8] = { 9, 8, 7,
									1, -1,
								-7, -8, -9 };
const int Board::materialScores[6] = { 100, 320, 500, 330, 0, 900 };

Board::Board() {

	srand(1000);
	for (int i = 1; i > -1; --i) {
		for (int j = 0; j < 6; j++) {
			for (int k = 0; k < 64; k++) {
				pieceKeys[i][j][k] = rand() * rand();
			}
		}
	}
	Move::board = this;
	nextStep[WHITE] = new BITBOARD[6]();
	nextStep[BLACK] = new BITBOARD[6]();
	init();
}

Board::~Board() {
}

void Board::erase() {
	int i;

	for (i = 0; i < 6; i++) {
		piecesVector[WHITE][i].clear();
		piecesVector[BLACK][i].clear();
	}

	for (i = 0; i < 6; i++) {
		nextStep[WHITE][i] = 0;
		nextStep[BLACK][i] = 0;
	}
}

void Board::init() {
	Piece *p;

	for (int i = 0; i<8; i++)
		for (int j = 0; j<8; j++)
			allPieces[i][j] = nullptr;

	for (int i = 0; i<8; i++) {
		p = new Pawn(i + 48, PIECE_COLOR::BLACK);
		piecesVector[BLACK][PAWNS].push_back(p);
		allPieces[6][i] = p;

		p = new Pawn(i + 8, PIECE_COLOR::WHITE);
		piecesVector[WHITE][PAWNS].push_back(p);
		allPieces[1][i] = p;
	}

	boardsVector[WHITE] = 0xffffULL;
	boardsVector[BLACK] = 0xffff000000000000ULL;
	board = boardsVector[WHITE] | boardsVector[BLACK];

	p = new Rook(56, PIECE_COLOR::BLACK);
	piecesVector[BLACK][ROOKS].push_back(p);
	*(*allPieces + 56) = p;

	p = new Rook(63, PIECE_COLOR::BLACK);
	piecesVector[BLACK][ROOKS].push_back(p);
	allPieces[7][7] = p;

	p = new Rook(0, PIECE_COLOR::WHITE);
	piecesVector[WHITE][ROOKS].push_back(p);
	*(*(allPieces)) = p;

	p = new Rook(7, PIECE_COLOR::WHITE);
	piecesVector[WHITE][ROOKS].push_back(p);
	*(*(allPieces)+7) = p;

	p = new Knight(57, PIECE_COLOR::BLACK);
	piecesVector[BLACK][KNIGHTS].push_back(p);
	allPieces[7][1] = p;

	p = new Knight(62, PIECE_COLOR::BLACK);
	piecesVector[BLACK][KNIGHTS].push_back(p);
	allPieces[7][6] = p;

	p = new Knight(1, PIECE_COLOR::WHITE);
	piecesVector[WHITE][KNIGHTS].push_back(p);
	*(*(allPieces)+1) = p;

	p = new Knight(6, PIECE_COLOR::WHITE);
	piecesVector[WHITE][KNIGHTS].push_back(p);
	*(*(allPieces)+6) = p;

	p = new Bishop(58, PIECE_COLOR::BLACK);
	piecesVector[BLACK][BISHOPS].push_back(p);
	allPieces[7][2] = p;

	p = new Bishop(61, PIECE_COLOR::BLACK);
	piecesVector[BLACK][BISHOPS].push_back(p);
	allPieces[7][5] = p;

	p = new Bishop(2, PIECE_COLOR::WHITE);
	piecesVector[WHITE][BISHOPS].push_back(p);
	*(*(allPieces)+2) = p;

	p = new Bishop(5, PIECE_COLOR::WHITE);
	piecesVector[WHITE][BISHOPS].push_back(p);
	*(*(allPieces)+5) = p;

	p = new Queen(60, PIECE_COLOR::BLACK);
	piecesVector[BLACK][QUEEN].push_back(p);
	allPieces[7][4] = p;

	p = new Queen(4, PIECE_COLOR::WHITE);
	piecesVector[WHITE][QUEEN].push_back(p);
	*(*(allPieces)+4) = p;

	p = new King(59, PIECE_COLOR::BLACK);
	piecesVector[BLACK][KING].push_back(p);
	allPieces[7][3] = p;

	p = new King(3, PIECE_COLOR::WHITE);
	piecesVector[WHITE][KING].push_back(p);
	*(*(allPieces)+3) = p;


	// Constructing BITBOARD sets for next step
	for (int i = 0; i < 8; i++) {
		nextStep[WHITE][PAWNS] |= ((Pawn*)piecesVector[WHITE][PAWNS][i])->getForwardMoves();
		nextStep[BLACK][PAWNS] |= ((Pawn*)piecesVector[BLACK][PAWNS][i])->getForwardMoves();
	}

	for (int i = 0; i < 2; i++) {
		nextStep[WHITE][KNIGHTS] |= piecesVector[WHITE][KNIGHTS][i]->getAllMoves();
		nextStep[BLACK][KNIGHTS] |= piecesVector[BLACK][KNIGHTS][i]->getAllMoves();
	}

	//updateNextMoves(PAWNS, BLACK);
	
	// Pawns
	material[BLACK] = piecesVector[BLACK][0].size() * 100;
	// Knigths
	material[BLACK] += piecesVector[BLACK][1].size() * 320;
	// Rooks
	material[BLACK] += piecesVector[BLACK][2].size() * 500;
	// Bishops
	material[BLACK] += piecesVector[BLACK][3].size() * 330;
	// King
	material[BLACK] += piecesVector[BLACK][4].size() * 0;
	// Queen
	material[BLACK] += piecesVector[BLACK][5].size() * 900;

	// Switch players color

	// Pawns
	material[WHITE] = piecesVector[WHITE][0].size() * 100;
	// Knigths
	material[WHITE] += piecesVector[WHITE][1].size() * 320;
	// Rooks
	material[WHITE] += piecesVector[WHITE][2].size() * 500;
	// Bishops
	material[WHITE] += piecesVector[WHITE][3].size() * 330;
	// King
	material[WHITE] += piecesVector[WHITE][4].size() * 0;
	// Queen
	material[WHITE] += piecesVector[WHITE][5].size() * 900;


}


bool Board::isMovable(PIECE_TYPES pieceType, PIECE_COLOR pieceColor) {
	
	return ((nextStep[pieceColor][pieceType] > 0) ? true : false);
}

//DE MODIFICAT
void Board::updateNextMoves(PIECE_TYPES pieceType, PIECE_COLOR pieceColor) {
	int j;
	BITBOARD pieceMoves;
	
	
	for (j=5; j >= 0; j--) {
		nextStep[WHITE][j] = 0;
		for (unsigned int i = 0; i < piecesVector[WHITE][j].size(); i++) {
			pieceMoves = getPossibleMoves(piecesVector[WHITE][j][i]);
			nextStep[WHITE][j] |= pieceMoves;
		}
		
		nextStep[BLACK][j] = 0;
		for (unsigned int i = 0; i < piecesVector[BLACK][j].size(); i++) {
			pieceMoves = getPossibleMoves(piecesVector[BLACK][j][i]);
			nextStep[BLACK][j] |= pieceMoves;
		}
	}
}

void Board::printBitboard(BITBOARD boardToPrint) {

	BITBOARD mask = 1;
	BITBOARD newm;

	newm = boardToPrint;
	mask = mask << 63;

	std::cout << "# ";
	for (int i = 0; i<64; i++) {
		if (i % 8 == 0)
			std::cout << std::endl << "# ";
		if ((mask & newm) > 0)
			std::cout << 1 << " ";
		else
			std::cout << 0 << " ";
		mask = mask >> 1;
	}
	std::cout << "\n#\n#---------\n# " << std::endl;
	fflush(stdout);

}

void Board::printPointerBoard(PIECE_COLOR color) {
	std::cout << "#";
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
		if (*(*(allPieces) + 63 - (i * 8 + j)) != nullptr && (*(*(allPieces) + 63 - (i * 8 + j)))->color == color)
			std::cout << (*(*(allPieces) + 63 - (i * 8 + j)))->type + 1 << " ";
		else
			std::cout << "0 ";
		std::cout << "\n#";
	}
	std::cout << "\n";
	fflush(stdout);
}

BITBOARD Board::genNegativeMoves(const Position position, const Position direction) {
	BITBOARD mask = 1;
	BITBOARD result = 0;

	result |= mask << (position - direction);
	return result;
}


bool Board::pathClearForCastl(Rook *rook) {
	Piece *king = piecesVector[rook->color][KING][0];
	/*
	if (king->currentPosition < rook->currentPosition)
		return (((1ULL << rook->currentPosition) - (1ULL << (king->currentPosition + 1))) & (board & (255ULL << ((king->currentPosition >> 3) << 3)))) == 0;
	else
		return (((1ULL << (king->currentPosition)) - (1ULL << (rook->currentPosition + 1))) & (board & (255ULL << ((king->currentPosition >> 3) << 3)))) == 0;*/
	if (king->currentPosition < rook->currentPosition) {
		for (Position i = king->currentPosition + 1; i < rook->currentPosition; i++)
			if ( *(*(allPieces) + i) != nullptr)
				return false;
		return true;
	}

	for (Position i = king->currentPosition - 1; i > rook->currentPosition; i--)
		if ( *(*(allPieces) + i) != nullptr)
			return false;
	return true;
}


BITBOARD Board::getPossibleMoves(Piece *piece) {
	BITBOARD possibleMoves = 0;
	BITBOARD empty;
	BITBOARD mask = 1;
	BITBOARD result = 0;
	BITBOARD notA, notH;
	std::vector<Move*> results;

	board = boardsVector[WHITE] | boardsVector[BLACK];
	switch (piece->type) {
	case PAWNS: {
					empty = ~board;
					Pawn *pawn = (Pawn*)piece;

					result = ((pawn->getForwardMoves() ^ board) & (empty)) | (((pawn->getAttackMoves() ^ boardsVector[pawn->color]) & (~boardsVector[pawn->color])) & boardsVector[1 - pawn->color]);
					if (pawn->isOnStartingPosition()) {
						mask = mask << (pawn->currentPosition + (-1) * ((-2) * pawn->color + 1) * 8);
						if (mask & board)
							result = result & (~pawn->getForwardMoves());
					}
					return result;
	}
	case KNIGHTS: {
					  result = ((piece->getAllMoves() ^ boardsVector[piece->color]) & (~boardsVector[piece->color]));
					  return result;
	}
	case KING: {
				   result = ((piece->getAllMoves() ^ boardsVector[piece->color]) & (~boardsVector[piece->color]));
				   return result;
	}
	case ROOKS: {
					empty = ~board;

					mask = mask << piece->currentPosition;
					possibleMoves = mask;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;

					result = possibleMoves;
					result |= ((possibleMoves >> 8) & boardsVector[1 - piece->color]);

					mask = 1;
					mask = mask << piece->currentPosition;


					possibleMoves = mask;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;

					result |= possibleMoves;
					result |= ((possibleMoves << 8) & boardsVector[1 - piece->color]);

					mask = 1;
					mask = mask << piece->currentPosition;
					empty = ~board;
					notA = 0x7f7f7f7f7f7f7f7f;
					empty &= notA;

					possibleMoves = mask;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					//possibleMoves |= (possibleMoves >> 1) & empty;
					// possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= ((possibleMoves >> 1) & boardsVector[1 - piece->color]);
					possibleMoves &= notA;
					result |= possibleMoves;

					//result |= (possibleMoves >> 1) & notH;
					mask = 1;
					mask = mask << piece->currentPosition;
					empty = ~board;
					notH = 0xfefefefefefefefe;
					empty &= notH;

					possibleMoves = mask;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= ((possibleMoves << 1) & boardsVector[1 - piece->color]);
					possibleMoves &= notH;

					// possibleMoves |= mask = (mask << 1) & empty;

					result |= possibleMoves;

					mask = 1;
					result = result & (~(mask << piece->currentPosition));

					//mask = 1;
					//result = result & (~(mask<<piece->currentPosition));
					//	std::cout << "# Miscari ture\n";
					//printBitboard(result);
					return result;
	}

	case BISHOPS: {
					  mask = 1;
					  notA = 0x7f7f7f7f7f7f7f7f;
					  mask = mask << piece->currentPosition;
					  empty = ~board;
					  empty &= notA;

					  //jos-dreapta
					  possibleMoves = mask;
					  possibleMoves |= mask = (mask >> 9) & empty;
					  possibleMoves |= mask = (mask >> 9) & empty;
					  possibleMoves |= mask = (mask >> 9) & empty;
					  possibleMoves |= mask = (mask >> 9) & empty;
					  possibleMoves |= mask = (mask >> 9) & empty;
					  possibleMoves |= mask = (mask >> 9) & empty;
					  possibleMoves |= mask = (mask >> 9) & empty;
					  possibleMoves |= ((possibleMoves >> 9) & boardsVector[1 - piece->color]);
					  possibleMoves &= notA;
					  result = possibleMoves;


					  //dreapta-sus
					  mask = 1;
					  mask = mask << piece->currentPosition;
					  empty = ~board;
					  empty &= notA;

					  possibleMoves = mask;
					  possibleMoves |= mask = (mask << 7) & empty;
					  possibleMoves |= mask = (mask << 7) & empty;
					  possibleMoves |= mask = (mask << 7) & empty;
					  possibleMoves |= mask = (mask << 7) & empty;
					  possibleMoves |= mask = (mask << 7) & empty;
					  possibleMoves |= mask = (mask << 7) & empty;
					  possibleMoves |= mask = (mask << 7) & empty;
					  possibleMoves |= ((possibleMoves << 7) & boardsVector[1 - piece->color]);
					  possibleMoves &= notA;
					  result |= possibleMoves;

					  mask = 1;
					  notH = 0xfefefefefefefefe;
					  mask = mask << piece->currentPosition;
					  empty = ~board;
					  empty &= notH;

					  //jos-stanga
					  possibleMoves = mask;
					  possibleMoves |= mask = (mask >> 7) & empty;
					  possibleMoves |= mask = (mask >> 7) & empty;
					  possibleMoves |= mask = (mask >> 7) & empty;
					  possibleMoves |= mask = (mask >> 7) & empty;
					  possibleMoves |= mask = (mask >> 7) & empty;
					  possibleMoves |= mask = (mask >> 7) & empty;
					  possibleMoves |= mask = (mask >> 7) & empty;
					  possibleMoves |= ((possibleMoves >> 7) & boardsVector[1 - piece->color]);
					  possibleMoves &= notH;
					  result |= possibleMoves;


					  //sus-stanga
					  mask = 1;
					  mask = mask << piece->currentPosition;
					  empty = ~board;
					  empty &= notH;

					  possibleMoves = mask;
					  possibleMoves |= mask = (mask << 9) & empty;
					  possibleMoves |= mask = (mask << 9) & empty;
					  possibleMoves |= mask = (mask << 9) & empty;
					  possibleMoves |= mask = (mask << 9) & empty;
					  possibleMoves |= mask = (mask << 9) & empty;
					  possibleMoves |= mask = (mask << 9) & empty;
					  possibleMoves |= mask = (mask << 9) & empty;
					  possibleMoves |= ((possibleMoves << 9) & boardsVector[1 - piece->color]);
					  possibleMoves &= notH;
					  result |= possibleMoves;


					  mask = 1;
					  result = result & (~(mask << piece->currentPosition));

					  //std::cout<<"# NEBUNI\n";
					  //printBitboard(result);
					  return result;
	}
	case QUEEN: {
					empty = ~board;

					mask = mask << piece->currentPosition;
					possibleMoves = mask;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;
					possibleMoves |= mask = (mask >> 8) & empty;

					result = possibleMoves;
					result |= ((possibleMoves >> 8) & boardsVector[1 - piece->color]);

					mask = 1;
					mask = mask << piece->currentPosition;


					possibleMoves = mask;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;
					possibleMoves |= mask = (mask << 8) & empty;

					result |= possibleMoves;
					result |= ((possibleMoves << 8) & boardsVector[1 - piece->color]);

					mask = 1;
					mask = mask << piece->currentPosition;
					empty = ~board;
					notA = 0x7f7f7f7f7f7f7f7f;
					empty &= notA;

					possibleMoves = mask;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= mask = (mask >> 1) & empty;
					//possibleMoves |= (possibleMoves >> 1) & empty;
					// possibleMoves |= mask = (mask >> 1) & empty;
					possibleMoves |= ((possibleMoves >> 1) & boardsVector[1 - piece->color]);
					possibleMoves &= notA;
					result |= possibleMoves;

					//result |= (possibleMoves >> 1) & notH;
					mask = 1;
					mask = mask << piece->currentPosition;
					empty = ~board;
					notH = 0xfefefefefefefefe;
					empty &= notH;

					possibleMoves = mask;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= mask = (mask << 1) & empty;
					possibleMoves |= ((possibleMoves << 1) & boardsVector[1 - piece->color]);
					possibleMoves &= notH;

					// possibleMoves |= mask = (mask << 1) & empty;

					result |= possibleMoves;

					mask = 1;
					notA = 0x7f7f7f7f7f7f7f7f;
					mask = mask << piece->currentPosition;
					empty = ~board;
					empty &= notA;

					//jos-dreapta
					possibleMoves = mask;
					possibleMoves |= mask = (mask >> 9) & empty;
					possibleMoves |= mask = (mask >> 9) & empty;
					possibleMoves |= mask = (mask >> 9) & empty;
					possibleMoves |= mask = (mask >> 9) & empty;
					possibleMoves |= mask = (mask >> 9) & empty;
					possibleMoves |= mask = (mask >> 9) & empty;
					possibleMoves |= mask = (mask >> 9) & empty;
					possibleMoves |= ((possibleMoves >> 9) & boardsVector[1 - piece->color]);
					possibleMoves &= notA;
					result |= possibleMoves;


					//dreapta-sus
					mask = 1;
					mask = mask << piece->currentPosition;
					empty = ~board;
					empty &= notA;

					possibleMoves = mask;
					possibleMoves |= mask = (mask << 7) & empty;
					possibleMoves |= mask = (mask << 7) & empty;
					possibleMoves |= mask = (mask << 7) & empty;
					possibleMoves |= mask = (mask << 7) & empty;
					possibleMoves |= mask = (mask << 7) & empty;
					possibleMoves |= mask = (mask << 7) & empty;
					possibleMoves |= mask = (mask << 7) & empty;
					possibleMoves |= ((possibleMoves << 7) & boardsVector[1 - piece->color]);
					possibleMoves &= notA;
					result |= possibleMoves;

					mask = 1;
					notH = 0xfefefefefefefefe;
					mask = mask << piece->currentPosition;
					empty = ~board;
					empty &= notH;

					//jos-stanga
					possibleMoves = mask;
					possibleMoves |= mask = (mask >> 7) & empty;
					possibleMoves |= mask = (mask >> 7) & empty;
					possibleMoves |= mask = (mask >> 7) & empty;
					possibleMoves |= mask = (mask >> 7) & empty;
					possibleMoves |= mask = (mask >> 7) & empty;
					possibleMoves |= mask = (mask >> 7) & empty;
					possibleMoves |= mask = (mask >> 7) & empty;
					possibleMoves |= ((possibleMoves >> 7) & boardsVector[1 - piece->color]);
					possibleMoves &= notH;
					result |= possibleMoves;


					//sus-stanga
					mask = 1;
					mask = mask << piece->currentPosition;
					empty = ~board;
					empty &= notH;

					possibleMoves = mask;
					possibleMoves |= mask = (mask << 9) & empty;
					possibleMoves |= mask = (mask << 9) & empty;
					possibleMoves |= mask = (mask << 9) & empty;
					possibleMoves |= mask = (mask << 9) & empty;
					possibleMoves |= mask = (mask << 9) & empty;
					possibleMoves |= mask = (mask << 9) & empty;
					possibleMoves |= mask = (mask << 9) & empty;
					possibleMoves |= ((possibleMoves << 9) & boardsVector[1 - piece->color]);
					possibleMoves &= notH;
					result |= possibleMoves;


					mask = 1;
					result = result & (~(mask << piece->currentPosition));
					return result;
	}
	}

	
}

std::vector<Board::Move*> Board::getPossiblePosition(Piece *piece) {
	char mask = 1;
	//auto it = piecesMap.find(piece);
	//if (it == piecesMap.end())
		//std::cout << "Mare caca" << std::endl;
	BITBOARD possibleMoves = getPossibleMoves(piece);
		//= piecesMap.find(piece)->second;
	std::vector<Move*> v;

	//printBitboard(possibleMoves);
	possibleMoves = possibleMoves & (~(mask << piece->currentPosition));

	if (piece->type == PAWNS) {
		for (Position i = 0; possibleMoves != 0; i++) {
			if ((mask & possibleMoves) == 1) {
				if (isOnLastRow(i))
					v.push_back(new PawnPromotion(piece, i, QUEEN));
				else
					v.push_back(new BasicMove(piece, i));
			}
			possibleMoves = possibleMoves >> 1;
		}
		if (_EPpawn != nullptr)
			v.push_back(new EnPassant(piece, _EPpawn));
		return v;
	}

	for (Position i=0; possibleMoves != 0; i++) {
		if ((mask & possibleMoves) == 1) {
			v.push_back(new BasicMove(piece, i));
		}
		possibleMoves = possibleMoves >> 1;
	}

	if (piece->type == KING) {
		if (((King*)piece)->canCastle() && !isCheckMate(piece->color)) {
			for (int i = 0; i < piecesVector[piece->color][ROOKS].size(); i++) {
				if (((Rook*)piecesVector[piece->color][ROOKS][i])->canCastle()) {
					if (pathClearForCastl((Rook*)piecesVector[piece->color][ROOKS][i])) {
						canCastle = true;
						v.push_back(new CastlingMove((King*)piece, (Rook*)piecesVector[piece->color][ROOKS][i]));
					}
				}
			}
		}
	}

	return v;
}

//BasicMove
Board::BasicMove::BasicMove(Piece *p1, Position newPosition) {
	set(p1, newPosition);
}

void Board::BasicMove::set(Piece *piece, Position newPosition) {
	piece1 = piece;
	this->newPosition = newPosition;
	isCastling = false;
	oldPosition = piece1->currentPosition;
}

bool Board::BasicMove::isCastlingPiece() {
	return (piece1->type == ROOKS || piece1->type == KING);
}

void Board::BasicMove::apply() {
	// Canceling previous en passant
	oldEligableEnPassant = board->_EPpawn;
	board->_EPpawn = nullptr;

	if (isCastlingPiece())
		((CastlingPiece*)piece1)->moveCount++;

	piece2 = board->movePiece(piece1, newPosition);
}

void Board::BasicMove::undo() {
	if (isCastlingPiece()) {
		((CastlingPiece*)piece1)->moveCount--;
	}

	board->movePiece(piece1, oldPosition);
	if (piece2 != nullptr) {
		board->piecesVector[piece2->color][piece2->type].push_back(piece2);
		board->putOnBoard(piece2);
		board->material[piece2->color] += materialScores[piece2->type];
	}

	// Restoring old en passant
	board->_EPpawn = oldEligableEnPassant;
	oldEligableEnPassant = nullptr;
}
//end BasicMove

//PawnMove

void Board::PawnMove::apply() {
	Board::BasicMove::apply();

	direction = newPosition < oldPosition ? -1 : 1;

	if (newPosition - oldPosition == 16 * direction) {
		board->_EPpawn = piece1;
	}
}

void Board::PawnMove::undo() {

	if (newPosition - oldPosition == 16 * direction) {
		if (newPosition % 7 != 0 && (*(*(board->allPieces) + 1)) != nullptr)
		if ((*(*(board->allPieces) + 1))->type = PAWNS) {
			((Pawn*)(*(*(board->allPieces) + 1)))->eligibleForEP = false;
			((Pawn*)(*(*(board->allPieces) + 1)))->EP1 = nullptr;
		}

		if (newPosition % 8 != 0 && (*(*(board->allPieces) - 1)) != nullptr)
		if ((*(*(board->allPieces) - 1))->type = PAWNS) {
			((Pawn*)(*(*(board->allPieces) + 1)))->eligibleForEP = false;
			((Pawn*)(*(*(board->allPieces) + 1)))->EP2 = nullptr;
		}
	}
	Board::BasicMove::undo();
}

//end PawnMove


//CastlingMove
Board::CastlingMove::CastlingMove(King *k, Rook *r) {
	isCastling = true;
	set(k, r);
}

void Board::CastlingMove::set(King *k, Rook *r) {
	king = k;
	rook = r;

	if (k->currentPosition > r->currentPosition)
		castlingDirection = RIGHT;
	else
		castlingDirection = LEFT;

	oldPosition = king->currentPosition;
}

void Board::CastlingMove::apply() {
	// Canceling previous en passant
	oldEligableEnPassant = board->_EPpawn;
	board->_EPpawn = nullptr;

	board->movePiece(king, king->currentPosition + castlingDirection * CASTLING_DISTANCE);
	board->movePiece(rook, king->currentPosition + opposite_direction(castlingDirection));
	newPosition = king->currentPosition;
	king->moveCount++;
	rook->moveCount++;
}

void Board::CastlingMove::undo() {
	board->movePiece(king, king->currentPosition + CASTLING_DISTANCE * opposite_direction(castlingDirection));
	board->movePiece(rook, ROOK_ORIGINAL_POSITION);
	king->moveCount--;
	rook->moveCount--;

	// Restoring old en passant
	board->_EPpawn = oldEligableEnPassant;
	oldEligableEnPassant = nullptr;
}
//end CastlingMove

//PawnPromotion
Board::PawnPromotion::PawnPromotion(Piece *p, Position newPosition, PIECE_TYPES newType) {
	set(p, newPosition, newType);
}

void Board::PawnPromotion::set(Piece *p, Position newPosition, PIECE_TYPES newType) {
	pawn = p;
	this->newPosition = newPosition;
	this->newType = newType;
	oldPosition = pawn->currentPosition;
	removed = nullptr;
}

void Board::PawnPromotion::apply() {
	// Canceling previous en passant
	oldEligableEnPassant = board->_EPpawn;
	board->_EPpawn = nullptr;

	removed = board->movePiece(pawn, newPosition);
	newPiece = board->createPiece(newType, pawn);
	board->removePiece(pawn);
	board->piecesVector[pawn->color][newType].push_back(newPiece);
	board->putOnBoard(newPiece);
	board->material[newPiece->color] += materialScores[newPiece->type];
	//delete pawn;
}

void Board::PawnPromotion::undo() {
	board->removePiece(newPiece);
	board->piecesVector[pawn->color][PAWNS].push_back(pawn);
	pawn->currentPosition = oldPosition;
	board->putOnBoard(pawn);
	if (removed != nullptr) {
		board->piecesVector[removed->color][removed->type].push_back(removed);
		board->putOnBoard(removed);
		board->material[removed->color] += materialScores[removed->type];
	}
	//delete newPiece;

	// Restoring old en passant
	board->_EPpawn = oldEligableEnPassant;
	oldEligableEnPassant = nullptr;
}

//end PawnPromotion

//EnPassant
Board::EnPassant::EnPassant(Piece *p, Piece *removed) {
	set(p, removed);
}

void Board::EnPassant::set(Piece *p, Piece *removed) {
	pawn = p;
	oldPosition = pawn->currentPosition;
	this->removed = removed;
	newPosition = removed->currentPosition + 8 * (2 * pawn->color - 1);
}

void Board::EnPassant::apply() {
	// Canceling previous en passant
	oldEligableEnPassant = board->_EPpawn;
	board->_EPpawn = nullptr;

	board->movePiece(pawn, newPosition);
	board->removePiece(removed);
}

void Board::EnPassant::undo() {
	board->movePiece(pawn, oldPosition);
	board->piecesVector[removed->color][removed->type].push_back(removed);
	board->putOnBoard(removed);
	board->material[removed->color] += materialScores[removed->type];

	// Restoring old en passant
	board->_EPpawn = oldEligableEnPassant;
	oldEligableEnPassant = nullptr;

}

Piece* Board::createPiece(PIECE_TYPES type, Piece *oldPiece) {
	switch (type) {
	case QUEEN: return new Queen(oldPiece->currentPosition, oldPiece->color);
	case BISHOPS: return new Bishop(oldPiece->currentPosition, oldPiece->color);
	case KING: return new King(oldPiece->currentPosition, oldPiece->color);
	case KNIGHTS: return new Knight(oldPiece->currentPosition, oldPiece->color);
	case PAWNS: return new Pawn(oldPiece->currentPosition, oldPiece->color);
	case ROOKS: return new Rook(oldPiece->currentPosition, oldPiece->color);
	}
}

void Board::removePiece(Piece *piece) {
	tempRemovedPieces.push_back(piece);

	*(*(allPieces) + piece->currentPosition) = nullptr;
	
	for (unsigned int i = 0; i < piecesVector[piece->color][piece->type].size(); i++) {
		if (piecesVector[piece->color][piece->type][i]->getPosition() == piece->currentPosition) {
			piecesVector[piece->color][piece->type].erase(piecesVector[piece->color][piece->type].begin() + i);
			removeFromBitboards(boardsVector[piece->color], piece->currentPosition);
			break;
		}
	}
	material[piece->color] -= materialScores[piece->type];
	updateNextMoves(piece->type, piece->color);
}

void Board::removeFromBitboards(BITBOARD &bitboard, Position position) {
	BITBOARD mask = 1;
	mask = ~(mask << position);

	bitboard = bitboard & mask;
	board = board & mask;
}

void Board::putOnBoard(Piece *piece) {

	boardsVector[piece->color] = (boardsVector[piece->color] | (1ULL << piece->currentPosition));
	*(*allPieces + piece->currentPosition) = piece;
	board = boardsVector[WHITE] | boardsVector[BLACK];
	updateNextMoves(piece->type, piece->color);
	recalcMoves(piece->currentPosition);

}

void Board::applyInputMove(Position oldPosition, Position newPosition, char lastChar) {
	Piece *piece = pieceAt(oldPosition);
	static BasicMove bMove;
	static CastlingMove cMove;
	static PawnPromotion pPromotion;
	
	if (piece->type == KING && oldPosition - newPosition == 2) {

		if (newPosition > oldPosition)
			cMove.set((King*)piece, (Rook*)pieceAt(piece->currentPosition + 4));
		else
			cMove.set((King*)piece, (Rook*)pieceAt(piece->currentPosition - 3));

		cMove.apply();
		return;
	}

	if (piece->type == PAWNS && isOnLastRow(newPosition)) {
		PIECE_TYPES newt = QUEEN;

		if (lastChar != '\0')
			newt = getPieceType(lastChar);
		pPromotion.set(piece, newPosition, newt);

		pPromotion.apply();
		return;
	}

	bMove.set(piece, newPosition);
	bMove.apply();
}

PIECE_TYPES Board::getPieceType(char c) {
	switch (c) {
		case 'b': return BISHOPS;
		case 'r': return ROOKS;
		case 'k': return KNIGHTS;
		case 'q': return QUEEN;
		case 'p': return PAWNS;
	}
}

Piece* Board::movePiece(Piece *piece, Position newPosition) {
	Piece *removedPiece = nullptr;
	BITBOARD mask = 1;

	*(*allPieces + piece->currentPosition) = nullptr;
	boardsVector[piece->color] = (boardsVector[piece->color] & (~(mask << piece->currentPosition)));

	if ((mask << newPosition) & board) {
		removedPiece = (*(*allPieces + newPosition));
		removePiece(removedPiece);
	}
	piece->move(newPosition);
	putOnBoard(piece);

	//DE MODIFICAT
	/*
	if (oldPosition != newPosition)
		boardsVector[piece->color] = (boardsVector[piece->color] | (mask << newPosition)) & (~(mask << oldPosition));
	else 
		boardsVector[piece->color] = (boardsVector[piece->color] | (mask << newPosition));
		*/
	_EPpawn = nullptr;	//cannot perform en passant after a move
	return removedPiece;
}

ULL Board::calcBoardKey(PIECE_COLOR playerColor) {
	ULL hashKey = 0;

	if (playerColor == BLACK) {
		for (int k = 0; k<2; ++k){
			for (int i = 5; i > -1; --i) {
				for (int j = 0; j < piecesVector[k][i].size(); j++) {
					hashKey ^= pieceKeys[k][i][piecesVector[k][i][j]->currentPosition];
				}
			}
		}
		return hashKey;
	}

	for (int k = 0; k<2; ++k) {
		for (int i = 5; i > -1; --i) {
			for (int j = 0; j < piecesVector[k][i].size(); j++) {
				hashKey ^= pieceKeys[k][i][63 - piecesVector[k][i][j]->currentPosition];
			}
		}
	}

	return hashKey;
}

void Board::addToHash(Board::MoveScore m, PIECE_COLOR playerColor) {
	ULL hashKey = calcBoardKey(playerColor);
	unsigned int buck = evalMap.bucket(hashKey);

	if (playerColor == WHITE)
		m.rotatePositions();

	for (auto it = evalMap.begin(buck); it != evalMap.end(buck); ++it) {
		if (it->second.stillUseful)
			continue;
		it->second = HashVal(m);
		const_cast<ULL&>(it->first) = hashKey;
		return;
	}

	evalMap.insert(std::pair<ULL, MoveScore>(hashKey, m));
}

bool Board::hasBeenEvald(PIECE_COLOR playerColor) {
	ULL hashKey = calcBoardKey(playerColor);


	auto it = evalMap.find(hashKey);

	if (it == evalMap.end())
		return false;

	evaldMove = it->second.move;
	it->second.stillUseful = true;
	if (playerColor != evaldMove.playerColor) {
		evaldMove.playerColor = playerColor;
		evaldMove.rotatePositions();
	}

	return true;

}

void Board::printHash() {
	for (auto& x : evalMap) {
		std::cout << "#";
		std::cout << x.first << " " << (int)x.second.move.move->oldPosition << " " <<(int) x.second.move.move->newPosition << " \n";
	}
}

Board::MoveScore Board::getMove() {
	return evaldMove;
}

void Board::printDebug() {
	/*std::cout << "#cai + pioni albi negri \n";
	printBitboard(nextStep[WHITE][KNIGHTS]);
	printBitboard(nextStep[WHITE][PAWNS]);
	printBitboard(nextStep[BLACK][KNIGHTS]);
	printBitboard(nextStep[BLACK][PAWNS]);

	std::cout << "#whiteboard\n";
	printBitboard(boardsVector[WHITE]);

	std::cout << "#blackboard\n";
	printBitboard(boardsVector[BLACK]);

	std::cout << "#board\n";
	printBitboard(board);

	std::cout << "#no more cai/pioni\n";
	*/
}

bool Board::isCheckMate(PIECE_COLOR playerColor) {
	BITBOARD kingPosition = 1;
	kingPosition = kingPosition << piecesVector[playerColor][KING][0]->currentPosition;

	kingPosition &= (nextStep[otherColor(playerColor)][PAWNS] | nextStep[otherColor(playerColor)][KNIGHTS] |
					 nextStep[otherColor(playerColor)][ROOKS] | nextStep[otherColor(playerColor)][BISHOPS] |
					 nextStep[otherColor(playerColor)][QUEEN] | nextStep[otherColor(playerColor)][KING]);

	if (kingPosition == 0)
		return false;
	return true;
}

// TODO
int Board::evaluate(PIECE_COLOR playerColor) {

	int s = 0;
	int mobility[2] = { 0, 0 };
	int bonus[2] = { 0, 0 };
	PIECE_COLOR otherPlayerColor = otherColor(playerColor);

	BITBOARD mask = 1;

	for (int i = 63; i >= 0; i--) {
		Piece* piece = pieceAt(i);
		if (piece != nullptr) {
			bonus[piece->color] += getPieceScore(piece);
		}

		mask = (1ULL) << i;
		for (int pieceType = 5; pieceType >= 0; pieceType--) {
			if (mask & nextStep[WHITE][pieceType])
				mobility[WHITE]++;
			if (mask & nextStep[BLACK][pieceType])
				mobility[BLACK]++;
		}
	}

	int mobilityScore = mobility[playerColor] * (mobility[playerColor] - mobility[otherPlayerColor]);
	int bonusScore = (bonus[playerColor] - bonus[otherPlayerColor]);

	s += bonusScore;
	s += material[playerColor] - material[otherPlayerColor];
	s += mobilityScore;

	// King safety
	if (piecesVector[playerColor][4].size() == 0 || piecesVector[otherPlayerColor][4].size() == 0)
		return s;

	std::vector<Position> kingsPosiblePositions[2];
	Position kingsCurrentPosition[2] = { piecesVector[playerColor][4][0]->currentPosition,
		piecesVector[otherPlayerColor][4][0]->currentPosition
	};

	kingsPosiblePositions[playerColor].push_back(kingsCurrentPosition[playerColor]);
	kingsPosiblePositions[otherPlayerColor].push_back(kingsCurrentPosition[otherPlayerColor]);

	for (int i = 7; i >= 0; i--) {
		Position tmp = kingsCurrentPosition[playerColor] + directions[i];
		Position tmp2 = kingsCurrentPosition[otherPlayerColor] + directions[i];
		if (tmp < 64 && tmp >= 0) {
			kingsPosiblePositions[playerColor].push_back(tmp);
		}
		if (tmp2 < 64 && tmp2 >= 0) {
			kingsPosiblePositions[otherPlayerColor].push_back(tmp2);
		}
	}

	int encounterdAttacks[2] = { 0, 0 };
	int totalValueOfAttacks[2] = { 0, 0 };

	for (auto i : kingsPosiblePositions[playerColor]) {
		BITBOARD mask = 1;
		for (int j = 5; j >= 0; j--) {
			if ((nextStep[otherPlayerColor][j] & (mask << i))) {
				encounterdAttacks[playerColor]++;
				totalValueOfAttacks[playerColor] += valueOfAttack[j];
			}
		}
	}

	s -= ((totalValueOfAttacks[playerColor] * attackWeight[encounterdAttacks[playerColor]]) / 100);

	for (auto i : kingsPosiblePositions[otherPlayerColor]) {
		BITBOARD mask = 1;
		for (int j = 5; j >= 0; j--) {
			if ((nextStep[playerColor][j] & (mask << i))) {
				encounterdAttacks[otherPlayerColor]++;
				totalValueOfAttacks[otherPlayerColor] += valueOfAttack[j];
			}
		}
	}

	s += ((totalValueOfAttacks[otherPlayerColor] * attackWeight[encounterdAttacks[otherPlayerColor]]) / 100);
	
	return s;
}

Board* Board::Move::board = 0;

/* Piece square tables initialisation*/

int Board::getPieceScore(Piece *p) {

	if (p->color == BLACK) {
		return pieceSquareTables[p->type][63 - p->currentPosition];
	}

	return pieceSquareTables[p->type][p->currentPosition];	
}

/*
std::vector<Board::Move*> Board::ownColorDepend(Piece *piece) {
	std::vector<Move*> v;
	return v;
}
*/

void Board::recalcMoves(Position p) {}

void Board::setDependences(Piece *piece) {
	std::vector<Move*>& v = movesMap.find(piece)->second;

	for (int i = 0; i < v.size(); i++) {
		dependentPieces[v[i]->newPosition].push_back(piece);
	}

	//own color dependecies
	BITBOARD empty, result, mask = 1;
	char m = 1;

	if (piece->type == PAWNS) {
		empty = ~board;
		Pawn *pawn = (Pawn*)piece;

		result = ((pawn->getForwardMoves() ^ board) & (empty)) | ((pawn->getAttackMoves() ^ boardsVector[pawn->color]));
		if (pawn->isOnStartingPosition()) {
			//mask = mask << (pawn->currentPosition + (-1) * ((-2) * pawn->color + 1) * 8);
			result = result & (~pawn->getForwardMoves());
		}

		for (Position i = 0; result != 0; i++){
			if (m & result)
				dependentPieces[i].push_back(piece);
		}
		return;
	}

	else {
		piece->color = otherColor(piece->color);
		result = getPossibleMoves(piece);
		piece->color = otherColor(piece->color);
	}

	for (Position i = 0; result != 0; i++) {
		if ((m & boardsVector[piece->color])) {
			dependentPieces[i].push_back(piece);
		}
	}
}

/* Piece square tables initialisation*/

const short int Board::pieceSquareTables[7][64] = {
	// Pawns
	{
	0, 0, 0, 0, 0, 0, 0, 0,
	5, 10, 10, -20, -20, 10, 10, 5,
	5, -5, -10, 0, 0, -10, -5, 5,
	0, 0, 0, 20, 20, 0, 0, 0,
	5, 5, 10, 25, 25, 10, 5, 5,
	10, 10, 20, 30, 30, 20, 10, 10,
	50, 50, 50, 50, 50, 50, 50, 50,
	300, 300, 300, 300, 300, 300, 300, 300
	},

	// Knights
	{
		-50, -40, -30, -30, -30, -30, -40, -50,
		-40, -20, 0, 5, 5, 0, -20, -40,
		-30, 5, 10, 15, 15, 10, 5, -30,
		-30, 0, 15, 20, 20, 15, 0, -30,
		-30, 5, 15, 20, 20, 15, 5, -30,
		-30, 0, 10, 15, 15, 10, 0, -30,
		-40, -20, 0, 0, 0, 0, -20, -40,
		-50, -40, -30, -30, -30, -30, -40, -50
	},

	// Rooks
	{
		0, 0, 0, 5, 5, 0, 0, 0,
		-5, 0, 0, 0, 0, 0, 0, -5,
		-5, 0, 0, 0, 0, 0, 0, -5,
		-5, 0, 0, 0, 0, 0, 0, -5,
		-5, 0, 0, 0, 0, 0, 0, -5,
		-5, 0, 0, 0, 0, 0, 0, -5,
		5, 10, 10, 10, 10, 10, 10, 5,
		-1, 0, 0, 0, 0, 0, 0, -1
	},

	// Bishops
	{
		-20, -10, -10, -10, -10, -10, -10, -20,
		-10, 5, 0, 0, 0, 0, 5, -10,
		-10, 10, 10, 10, 10, 10, 10, -10,
		-10, 0, 10, 10, 10, 10, 0, -10,
		-10, 5, 5, 10, 10, 5, 5, -10,
		-10, 0, 5, 10, 10, 5, 0, -10,
		-10, 0, 0, 0, 0, 0, 0, -10,
		-20, -10, -10, -10, -10, -10, -10, -20
	},

	// King middle game
	// Towards the end of the game it will be replaced by
	// the king end game square table
	{
		20, 30, 10, 0, 0, 10, 30, 20,
		20, 20, 0, 0, 0, 0, 20, 20,
		-10, -20, -20, -20, -20, -20, -20, -10,
		-20, -30, -30, -40, -40, -30, -30, -20,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30,
		-30, -40, -40, -50, -50, -40, -40, -30
	},

	// Queen
	{
		-20, -10, -10, -5, -5, -10, -10, -20,
		-10, 0, 0, 0, 0, 5, 0, -10,
		-10, 0, 5, 5, 5, 5, 5, -10,
		-5, 0, 5, 5, 5, 5, 0, 0,
		-5, 0, 5, 5, 5, 5, 0, -5,
		-10, 0, 5, 5, 5, 5, 0, -10,
		-10, 0, 0, 0, 0, 0, 0, -10,
		-20, -10, -10, -5, -5, -10, -10, -20
	},

	// King end-game
	{
		-50, -30, -30, -30, -30, -30, -30, -50,
		-30, -30, 0, 0, 0, 0, -30, -30,
		-30, -10, 20, 30, 30, 20, -10, -30,
		-30, -10, 30, 40, 40, 30, -10, -30,
		-30, -10, 30, 40, 40, 30, -10, -30,
		-30, -10, 20, 30, 30, 20, -10, -30,
		-30, -20, -10, 0, 0, -10, -20, -30,
		-50, -40, -30, -20, -20, -30, -40, -50
	}
};
