#include <iostream>
#include <vector>
#include <fstream>

#include "Bitboard.h"
#include "Piece.h"
#include "Knight.h"

std::vector<BITBOARD> generateKnightBitboard() {
	BITBOARD board = 0, mask = 1;
	std::vector<BITBOARD> allMoves;
	
	for (int i = 0; i < 64; i++) {
		//NW Vertical
		board = 0;
		if (i / 8 <= 5)
			if (i % 8 <= 6)
				board = board | (mask << (i + 17));

		//NE Vertical
		if (i / 8 <= 5)
			if (i % 8 >= 1)
				board = board | (mask << (i + 15));

		//SW Vertical
		if (i / 8 >= 2)
			if (i % 8 <= 6)
				board = board | (mask << (i - 15));
		
		//SW Vertical
		if (i / 8 >= 2)
			if (i % 8 >= 1)
				board = board | (mask << (i - 17));

		//NW O
		if (i / 8 <= 6)
			if (i % 8 <= 5)
				board = board | (mask << (i + 10));

		//NE O
		if (i / 8 <= 6)
			if (i % 8 >= 2)
				board = board | (mask << (i + 6));

		//SE O
		if (i / 8 >= 1)
			if (i % 8 >= 2)
				board = board | (mask << (i - 10));

		//SW O
		if (i / 8 >= 1)
			if (i % 8 <= 5)
				board = board | (mask << (i - 6));
		allMoves.push_back(board);
	}
	return allMoves;
}

std::vector<BITBOARD> generateKingMoves() {
	BITBOARD board = 0, mask = 1;
	std::vector<BITBOARD> allMoves;
	for (int i = 0; i < 64; i++) {
		if (i / 8 > 0) {
			if (i % 8 > 0)
				board |= mask << (i-9);
			if (i % 8 < 7)
				board |= mask << (i-7);

			board |= mask << (i-8);
		}

		if (i / 8 < 7) {
			if (i % 8 > 0)
				board |= mask << (i+7);
			if (i % 8 < 7)
				board |= mask << (i+9);

			board |= mask << (i+8);
		}

		if (i % 8 > 0)
			board |= mask << (i-1);
 
		if (i%8 < 7)
			board |= mask << (i+1);
		allMoves.push_back(board);
		board = 0;
	}
	return allMoves;
}

std::vector<BITBOARD> generateBlackPawnForwardBitboard() {
	BITBOARD board = 0, mask = 1;
	std::vector<BITBOARD> allMoves;

	for (int i=0; i<64; i++) {
		if (i / 8 > 0) {
			if ((i / 8) == 6) 
				board = board | (mask << (i - 16));
			board = board | (mask << (i - 8));
		}
		
		allMoves.push_back(board);
		board = 0;
	}
	return allMoves;
}

std::vector<BITBOARD> generateBlackPawnAttackBitboard() {
	BITBOARD board = 0, mask = 1;
	std::vector<BITBOARD> allMoves;

	for (int i=0; i<64; i++) {
		if (i / 8 > 0) {
			if (i % 8 < 7)
				board = board | (mask << (i - 7));

			if (i % 8 > 0)
				board = board | (mask << (i - 9));
		}
		allMoves.push_back(board);
		board = 0;
	}
	return allMoves;
}


std::vector<BITBOARD> generateWhitePawnForwardBitboard() {
	BITBOARD board = 0, mask = 1;
	std::vector<BITBOARD> allMoves;

	for (int i=0; i < 64; i++) {
		if ((i / 8) < 7) {
			if ((i / 8) == 1)
				board = board | (mask << (i + 16));
			board = board | (mask << (i + 8));
		}
	
		allMoves.push_back(board);
		board = 0;
	}
	return allMoves;
}

std::vector<BITBOARD> generateWhitePawnAttackBitboard() {
	BITBOARD board = 0, mask = 1;
	std::vector<BITBOARD> allMoves;

	for (int i=0; i<64; i++) {
		if (i / 8 < 7) {
			if (i % 8 > 0)
				board = board | (mask << (i + 7));

			if (i % 8 < 7)
				board = board | (mask << (i + 9));
		}
		allMoves.push_back(board);
		board = 0;
	}
	return allMoves;
}

void printBitboard(BITBOARD boardToPrint, std::ofstream &f) {
	BITBOARD mask=1;
		BITBOARD newm;

		newm = boardToPrint;
		mask = mask<<63;

		for (int i=0; i<64; i++) {
			if (i % 8 == 0)
				f<<std::endl;
			if ((mask & newm) > 0)
				f<<1<<" ";
			else
				f<<0<<" ";
			mask=mask>>1;
		}
		f << "\n\n---------\n" << std::endl;
}

/*
int main() {

	std::ofstream f("miscariRege.txt");
	std::ofstream debug("matriceDebugRege.txt");
	std::vector<BITBOARD> allMoves;

	allMoves = generateKingMoves();
	//printBitboard(allForwardMoves[0], test);

	for (int i=0; i<64; i++) {
		printBitboard(allMoves[i], debug);
		f<<"0x"<<std::hex<<allMoves[i]<<"ULL, ";
	}

	f.close();
	debug.close();
	getchar();
	return 0;
}
*/