#include "Board.h"

Board::Board() {

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
	/*if (pieceColor == BLACK)
		return ((blacksNextStep[pieceType] > 0) ? true : false);
	else
		return ((whitesNextStep[pieceType] > 0) ? true : false);
		*/

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

BITBOARD Board::getPossibleMoves(Piece *piece) {
	BITBOARD possibleMoves;

	if (piece->type == PAWNS) {
		BITBOARD mask = 1;

		board = boardsVector[WHITE] | boardsVector[BLACK];
		Pawn *pawn = (Pawn*)piece;

		possibleMoves = ((pawn->getForwardMoves() ^ board) & (~board)) | (((pawn->getAttackMoves() ^ boardsVector[pawn->color]) & (~boardsVector[pawn->color])) & boardsVector[1 - pawn->color]);
		if (pawn->isOnStartingPosition()) {
			mask = mask << (pawn->currentPosition + (-1) * ((-2) * pawn->color + 1) * 8);
			if (mask & board)
				possibleMoves = possibleMoves & (~pawn->getForwardMoves());
		}

	}
	else {
		possibleMoves = ((piece->getAllMoves() ^ boardsVector[piece->color]) & (~boardsVector[piece->color]));
	}
	
	return possibleMoves;
}


Position Board::getPossiblePosition(Piece *piece) {
	char mask = 1;
	BITBOARD possibleMoves = getPossibleMoves(piece);
	for (int i=0; i<64; i++) {
		if ((mask & possibleMoves) == 1)
			return i;
		possibleMoves = possibleMoves >> 1;
	}
	return -1;
}

void Board::removePiece(Position position) {

		Piece *piece = *(*(allPieces) + position);
		*(*(allPieces) + position) = nullptr;

		for (unsigned int i=0; i < piecesVector[piece->color][piece->type].size(); i++) 
				if (piecesVector[piece->color][piece->type][i]->getPosition() == position) {
					piecesVector[piece->color][piece->type].erase(piecesVector[piece->color][piece->type].begin() + i);
					removeFromBitboards(boardsVector[piece->color], position);
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

void Board::movePiece(Piece *piece, Position newPosition) {
	
	BITBOARD mask = 1;
	Position oldPosition;
	oldPosition = piece->getPosition();
	
	if ((mask << newPosition) & board)
		removePiece(newPosition);

	*(*allPieces + oldPosition) = nullptr;
	*(*allPieces + newPosition) = piece;

	boardsVector[piece->color] = (boardsVector[piece->color] | (mask << newPosition)) & (~(mask << oldPosition));

	board = boardsVector[WHITE] | boardsVector[BLACK];

	piece->move(newPosition);
	updateNextMoves(piece->type, piece->color);
}

void Board::printDebug() {
	std::cout << "#cai + pioni albi negri \n";
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
}

bool Board::isCheckMate() {
	BITBOARD kingPosition = 1;
	kingPosition = kingPosition << piecesVector[BLACK][KING][0]->currentPosition;
	kingPosition = kingPosition & (nextStep[WHITE][PAWNS] | nextStep[WHITE][KNIGHTS]);

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
	if (kingPosition == 0)
		return false;
	return true;
}