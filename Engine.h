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

#define not_found std::string::npos

//using namespace std;

typedef char Color;

class Engine {
private:
	std::string tok(std::string s, std::string delim);
	Command currentCommand;
	Color color;
	Board board;
public:
	void mainLoop();
	void reset();
		
	Command computeCommnandForWinboard(const Position oldPosition, const Position newPosition);
	void getCommand();
	void sendCommand(Command command);
};

#endif