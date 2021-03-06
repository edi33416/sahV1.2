#pragma once

#ifndef Engine_H
#define Engine_H

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "Piece.h"
#include "Board.h"
#include "utils.h"

#define COMMAND_RESIGN "resign\n"

#define not_found std::string::npos

typedef char Color;

class Engine {
private:

	PIECE_COLOR engineColor;
	PIECE_COLOR colorToMove;
	int evals;
	int quiescence;
	bool sorted, openings = true;
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
	Board::MoveScore negamax(PIECE_COLOR playerColor, int depth, int alpha, int beta);

public:
	int DEPTH = 4;
	void mainLoop();		
	Command computeCommnandForWinboard(const Position oldPosition, const Position newPosition);
	void getCommand();
	void sendCommand(Command command);
};

#endif