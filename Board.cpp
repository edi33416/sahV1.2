#include "Board.h"

Board::Board() {

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
	//allPieces[7][0] = p;
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

}


bool Board::isMovable(PIECE_TYPES pieceType, PIECE_COLOR pieceColor) {
	
	return ((nextStep[pieceColor][pieceType] > 0) ? true : false);
}

//DE MODIFICAT
void Board::updateNextMoves(PIECE_TYPES pieceType, PIECE_COLOR pieceColor) {
	int j;

	for (j=0; j<6; j++) {
		nextStep[WHITE][j] = 0;
		for (unsigned int i = 0; i < piecesVector[WHITE][j].size(); i++)
			nextStep[WHITE][j] |= getPossibleMoves(piecesVector[WHITE][j][i]);
		nextStep[BLACK][j] = 0;
		for (unsigned int i = 0; i < piecesVector[BLACK][j].size(); i++)
			nextStep[BLACK][j] |= getPossibleMoves(piecesVector[BLACK][j][i]);
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

}

BITBOARD Board::genNegativeMoves(const Position position, const Position direction) {
	BITBOARD mask = 1;
	BITBOARD result = 0;

	result |= mask << (position - direction);
	return result;
}




BITBOARD Board::getPossibleMoves(Piece *piece) {
	BITBOARD possibleMoves = 0;
	BITBOARD empty;
	BITBOARD mask = 1;
	BITBOARD result;
	BITBOARD notA, notH;

	board = boardsVector[WHITE] | boardsVector[BLACK];
	if (piece->type == PAWNS) {

		empty = ~board;
		Pawn *pawn = (Pawn*)piece;

		possibleMoves = ((pawn->getForwardMoves() ^ board) & (empty)) | (((pawn->getAttackMoves() ^ boardsVector[pawn->color]) & (~boardsVector[pawn->color])) & boardsVector[1 - pawn->color]);
		if (pawn->isOnStartingPosition()) {
			mask = mask << (pawn->currentPosition + (-1) * ((-2) * pawn->color + 1) * 8);
			if (mask & board)
				possibleMoves = possibleMoves & (~pawn->getForwardMoves());
		}
		return possibleMoves;
	}
	if (piece->type == KNIGHTS || piece->type == KING) {
		possibleMoves = ((piece->getAllMoves() ^ boardsVector[piece->color]) & (~boardsVector[piece->color]));
		return possibleMoves;
	}

	if (piece->type == ROOKS) {
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

	if (piece->type == BISHOPS) {
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

	if (piece->type == QUEEN) {
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


std::vector<Position> Board::getPossiblePosition(Piece *piece) {
	char mask = 1;
	BITBOARD possibleMoves = getPossibleMoves(piece);
	std::vector<Position> v;

	possibleMoves = possibleMoves & (~(mask << piece->currentPosition));

	for (Position i=0; i<64; i++) {
		if ((mask & possibleMoves) == 1) {
			//return i;
			v.push_back(i);
		}
		possibleMoves = possibleMoves >> 1;
	}
	return v;
}

void Board::removePiece(Piece *piece) {
	//Piece *piece = *(*(allPieces) + position);

	tempRemovedPieces.push_back(piece);
	*(*(allPieces) + piece->currentPosition) = nullptr;
	
	for (unsigned int i=0; i < piecesVector[piece->color][piece->type].size(); i++) 
			if (piecesVector[piece->color][piece->type][i]->getPosition() == piece->currentPosition) {
				piecesVector[piece->color][piece->type].erase(piecesVector[piece->color][piece->type].begin() + i);
				removeFromBitboards(boardsVector[piece->color], piece->currentPosition);
				break;
			}
	updateNextMoves(piece->type, piece->color);
}

void Board::removeFromBitboards(BITBOARD &bitboard, Position position) {
	BITBOARD mask = 1;
	mask = ~(mask << position);

	bitboard = bitboard & mask;
	board = board & mask;
}

void Board::undoMove(Piece *piece, Position oldPosition) {

	movePiece(piece, oldPosition);
	if (!tempRemovedPieces.empty()) {

		piece = tempRemovedPieces.back();
		movePiece(piece, piece->currentPosition);
		piecesVector[piece->color][piece->type].push_back(piece);
		tempRemovedPieces.pop_back();
	}
}

void Board::putOnBoard(Piece *piece) {

	boardsVector[piece->color] = (boardsVector[piece->color] | (1ULL << piece->currentPosition));
	*(*allPieces + piece->currentPosition) = piece;
	board = boardsVector[WHITE] | boardsVector[BLACK];
	updateNextMoves(piece->type, piece->color);
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
	return removedPiece;
}

void Board::pawnPromotion(Piece *piece) {
	Position currentPosition = piece->currentPosition;
	removePiece(piece);
	tempRemovedPieces.pop_back();

	Piece *queen = new Queen(currentPosition, piece->color);
	delete piece;
	piecesVector[queen->color][QUEEN].push_back(queen);
	movePiece(queen, currentPosition);
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

bool Board::isCheckMate() {
	BITBOARD kingPosition = 1;
	kingPosition = kingPosition << piecesVector[BLACK][KING][0]->currentPosition;
	kingPosition = kingPosition & (nextStep[WHITE][PAWNS] | nextStep[WHITE][KNIGHTS] | nextStep[WHITE][ROOKS] | nextStep[WHITE][BISHOPS] | nextStep[WHITE][QUEEN]);

	/*
	std::cout<<"#cai + pioni albi negri \n";
	printBitboard(nextStep[WHITE][KNIGHTS]);
	printBitboard(nextStep[WHITE][PAWNS]);
	printBitboard(nextStep[BLACK][KNIGHTS]);
	printBitboard(nextStep[BLACK][PAWNS]);

	std::cout<<"#whiteboard\n";
	printBitboard(boardsVector[WHITE]);

	std::cout<<"#blackboard\n";
	printBitboard(boardsVector[BLACK]);

	std::cout<<"#board\n";
	printBitboard(board);

	std::cout << "#no more cai/pioni\n";
	*/
	if (kingPosition == 0)
		return false;
	return true;
}