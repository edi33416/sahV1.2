#include "Board.h"

Board::Board() {
	Piece *p;

	whitesNextStep = new BITBOARD[6]();
	blacksNextStep = new BITBOARD[6]();

	for (int i=0; i<8; i++)
		for (int j=0; j<8; j++)
			allPieces[i][j] = nullptr;

	for (int i=0; i<8; i++) {
		p = new Pawn(i + 48, PIECE_COLOR::BLACK);
		blackPieces[PAWNS].push_back(p);
		allPieces[6][i] = p; 

		p = new Pawn(i + 8, PIECE_COLOR::WHITE);
		whitePieces[PAWNS].push_back(p);
		allPieces[1][i] = p;
	}

	whiteBoard = 0xffffULL;
	blackBoard = 0xffff000000000000ULL;
	board = whiteBoard | blackBoard;

	p = new Rook(56, PIECE_COLOR::BLACK); 
	blackPieces[ROOKS].push_back(p);
	//allPieces[7][0] = p;
	*(*allPieces + 56) = p;

	p = new Rook(63, PIECE_COLOR::BLACK);
	blackPieces[ROOKS].push_back(p);
	allPieces[7][7] = p;

	p = new Rook(0, PIECE_COLOR::WHITE);
	whitePieces[ROOKS].push_back(p);
	*(*(allPieces)) = p;

	p = new Rook(7, PIECE_COLOR::WHITE);
	whitePieces[ROOKS].push_back(p);
	*(*(allPieces) + 7) = p;

	p = new Knight(57, PIECE_COLOR::BLACK);
	blackPieces[KNIGHTS].push_back(p);
	allPieces[7][1] = p;

	p = new Knight(62, PIECE_COLOR::BLACK);
	blackPieces[KNIGHTS].push_back(p);
	allPieces[7][6] = p;

	p = new Knight(1, PIECE_COLOR::WHITE);
	whitePieces[KNIGHTS].push_back(p);
	*(*(allPieces) + 1) = p;

	p = new Knight(6, PIECE_COLOR::WHITE);
	whitePieces[KNIGHTS].push_back(p);
	*(*(allPieces) + 6) = p;

	p = new Bishop(58, PIECE_COLOR::BLACK);
	blackPieces[BISHOPS].push_back(p);
	allPieces[7][2] = p;

	p = new Bishop(61, PIECE_COLOR::BLACK);
	blackPieces[BISHOPS].push_back(p);
	allPieces[7][5] = p;

	p = new Bishop(2, PIECE_COLOR::WHITE);
	whitePieces[BISHOPS].push_back(p);
	*(*(allPieces) + 2) = p;

	p = new Bishop(5, PIECE_COLOR::WHITE);
	whitePieces[BISHOPS].push_back(p);
	*(*(allPieces) + 5) = p;

	p = new Queen(60, PIECE_COLOR::BLACK);
	blackPieces[QUEEN].push_back(p);
	allPieces[7][4] = p;

	p = new Queen(4, PIECE_COLOR::WHITE);
	whitePieces[QUEEN].push_back(p);
	*(*(allPieces) + 4) = p;

	p = new King(59, PIECE_COLOR::BLACK);
	blackPieces[KING].push_back(p);
	allPieces[7][3] = p;

	p = new King(3, PIECE_COLOR::WHITE);
	whitePieces[KING].push_back(p);
	*(*(allPieces) + 3) = p;


	// Constructing BITBOARD sets for next step
	for (int i = 0; i < 8; i++) {
		whitesNextStep[PAWNS] |= ((Pawn*)whitePieces[PAWNS][i])->getForwardMoves();
		blacksNextStep[PAWNS] |= ((Pawn*)blackPieces[PAWNS][i])->getForwardMoves();
	}

	for (int i = 0; i < 2; i++) {
		whitesNextStep[KNIGHTS] |= whitePieces[KNIGHTS][i]->getAllMoves();
		blacksNextStep[KNIGHTS] |= blackPieces[KNIGHTS][i]->getAllMoves();
	}
	
}

Board::~Board() {
}

bool Board::isMovable(PIECE_TYPES pieceType, PIECE_COLOR pieceColor) {
	if (pieceColor == BLACK)
		return ((blacksNextStep[pieceType] > 0) ? true : false);
	else
		return ((whitesNextStep[pieceType] > 0) ? true : false);
}

//DE MODIFICAT
void Board::updateNextMoves(PIECE_TYPES pieceType, PIECE_COLOR pieceColor) {
	int j;

	for (j=0; j<6; j++) {
		whitesNextStep[j] = 0;
		for (unsigned int i = 0; i < whitePieces[j].size(); i++)
			whitesNextStep[j] |= getPossibleMoves(whitePieces[j][i]);
		blacksNextStep[j] = 0;
		for (unsigned int i = 0; i < blackPieces[j].size(); i++)
			blacksNextStep[j] |= getPossibleMoves(blackPieces[j][i]);
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

		board = whiteBoard | blackBoard;
		Pawn *pawn = (Pawn*)piece;
		if (pawn->color == BLACK) {				
			possibleMoves = ((pawn->getForwardMoves() ^ board) & (~board)) | (((pawn->getAttackMoves() ^ blackBoard) & (~blackBoard)) & whiteBoard);
			if (pawn->currentPosition > 47 && pawn->currentPosition < 56) {
				mask = mask << (pawn->currentPosition - 8);
				if (mask & board)
					possibleMoves = possibleMoves & (~pawn->getForwardMoves());
			}
		}

		else {
			possibleMoves = ((pawn->getForwardMoves() ^ board) & (~board)) | (((pawn->getAttackMoves() ^ whiteBoard) & (~whiteBoard)) & blackBoard);
			if (pawn->currentPosition > 7 && pawn->currentPosition < 16) {
				mask = mask << (pawn->currentPosition + 8);
				if (mask & board)
					possibleMoves = possibleMoves & (~pawn->getForwardMoves());
			}
		}
	}
	else {
		if (piece->color == BLACK)
			possibleMoves = ((piece->getAllMoves() ^ blackBoard) & (~blackBoard));
		else
			possibleMoves = ((piece->getAllMoves() ^ whiteBoard) & (~whiteBoard));
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
		if (piece->color == PIECE_COLOR::WHITE) {
			for (unsigned int i=0; i < whitePieces[piece->type].size(); i++) 
				if (whitePieces[piece->type][i]->getPosition() == position) {
					whitePieces[piece->type].erase(whitePieces[piece->type].begin() + i);
					removeFromBitboards(whiteBoard, position);
					break;
				}
			updateNextMoves(piece->type, piece->color);
			return;
		}

		for (unsigned int i=0; i < blackPieces[piece->type].size(); i++) 
			if (blackPieces[piece->type][i]->getPosition() == position) {
				blackPieces[piece->type].erase(blackPieces[piece->type].begin() + i);
				removeFromBitboards(blackBoard, position);
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


	if (piece->color == BLACK)
		blackBoard = (blackBoard | (mask << newPosition)) & (~(mask << oldPosition));
	else 
		whiteBoard = (whiteBoard | (mask << newPosition)) & (~(mask << oldPosition));

	board = whiteBoard | blackBoard;

	piece->move(newPosition);
	updateNextMoves(piece->type, piece->color);
}

bool Board::isCheckMate() {
	BITBOARD kingPosition = 1;
	kingPosition = kingPosition << blackPieces[KING][0]->currentPosition;
	kingPosition = kingPosition & (whitesNextStep[PAWNS] | whitesNextStep[KNIGHTS]);

	std::cout<<"#cai + pioni albi negri \n";
	printBitboard(whitesNextStep[KNIGHTS]);
	printBitboard(whitesNextStep[PAWNS]);
	printBitboard(blacksNextStep[KNIGHTS]);
	printBitboard(blacksNextStep[PAWNS]);

	std::cout<<"#whiteboard\n";
	printBitboard(whiteBoard);

	std::cout<<"#blackboard\n";
	printBitboard(blackBoard);

	std::cout<<"#board\n";
	printBitboard(board);

	std::cout << "#no more cai/pioni\n";
	if (kingPosition == 0)
		return false;
	return true;
}