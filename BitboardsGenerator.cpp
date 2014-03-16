#include <iostream>
#include <vector>
#include <fstream>

#include "Bitboard.h"
#include "Piece.h"
#include "Knight.h"

std::vector<BITBOARD> generateBitboard() {
	//if (pieceType inscanceOf Knight)
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

		//BITBOARD mask=1;
		BITBOARD newm;

		/*newm = allMoves[0];
		mask = mask<<63;

		std::cout<<board<<std::endl;
		for (int i=0; i<64; i++) {
			if (i % 8 == 0)
				std::cout<<std::endl;
			if ((mask & newm) > 0)
				std::cout<<1<<" ";
			else
				std::cout<<0<<" ";

			mask=mask>>1;
		}*/
		return allMoves;
}

int main() {

	std::ofstream f("miscari.txt");
	BITBOARD b;

	std::vector<BITBOARD> allMoves;

	allMoves = generateBitboard();
	for (int i=0; i<64; i++)
		f<<"0x"<<std::hex<<allMoves[i]<<"ULL, ";

	f.close();

		BITBOARD mask=1;
		BITBOARD newm;

		newm = Knight::movesMatrix[27];
		mask = mask<<63;

		for (int i=0; i<64; i++) {
			if (i % 8 == 0)
				std::cout<<std::endl;
			if ((mask & newm) > 0)
				std::cout<<1<<" ";
			else
				std::cout<<0<<" ";

			mask=mask>>1;
		}
	getchar();
	return 0;
}