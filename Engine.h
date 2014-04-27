#pragma once

#ifndef Engine_H
#define Engine_H

#include <iostream>
#include <string>
#include <fstream>
#include "Piece.h"
#include "Board.h"
#include "utils.h"

#define COMMAND_RESIGN "resign\n"
#define DEPTH 4

#define not_found std::string::npos

//using namespace std;

typedef char Color;

class Engine {
private:

	std::string tok(std::string s, std::string delim);
	Command currentCommand;
	Color color;
	Board board;
	bool isForced;
	void force();
	bool engineIsToMove;
	void go();
	void processCommand(Command &command);
	void engineMove();
	void reset();
	std::pair<Board::Move*, int> negamax(PIECE_COLOR playerColor, int depth);

	PIECE_COLOR engineColor;
	PIECE_COLOR colorToMove;
	int foo;
public:
	void mainLoop();		
	Command computeCommnandForWinboard(const Position oldPosition, const Position newPosition);
	void getCommand();
	void sendCommand(Command command);
};

#endif