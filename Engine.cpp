#include <stdio.h>
#include <stdlib.h>

#include "Knight.h"
#include "Engine.h"
#include <math.h>
#include <time.h>

std::string Engine::tok(std::string s, std::string delim) {
	static size_t next = 0;
	size_t pos;
	std::string token;
	
	if ((pos = s.find(delim, next)) != std::string::npos) {
		token = s.substr(next, pos);
		next = pos + 1;
		return token;
	}
	next = 0;
	return 0;
}

void Engine::getCommand() {
	getline(std::cin, currentCommand);
	currentCommand.push_back(' ');
}

void Engine::reset() {
	//int i, j;
}

Command Engine::computeCommnandForWinboard(const Position oldPosition, const Position newPosition) {
	Command command;
	char toAppend[6];

	command.append("move ");
	toAppend[2] = ((7 - newPosition % 8) + 'a');
	toAppend[3] = ((newPosition / 8 + 1) + '0');
	toAppend[0] = ((7 - oldPosition % 8) + 'a');
	toAppend[1] = ((oldPosition / 8 + 1) + '0');
	toAppend[4] = '\n';
	toAppend[5] = '\0';

	command.append(toAppend);

	return command;
}

void Engine::sendCommand(Command command) {
	std::cout << command;
	std::fflush(stdout);
}

void Engine::mainLoop() {
	std::string token;
	Piece *piece;
	Command command;
	Position oldPosition;
	Position newPosition;
		
	std::cout.setf(std::ios::unitbuf);
	int foo = 0;
	srand((unsigned int)time(NULL));

	while(1) {
		
		getCommand();
					
		if (currentCommand.find("xboard") != not_found) {
//			init();
			continue;
		}

		if (currentCommand.find("quit") != not_found) {
			return;
		}

		if (currentCommand.find("protover") != not_found) {
			std::cout<<"feature usermove=1 debug=1 done=1\n";
			continue;
		}

		if (currentCommand.find("new") != not_found) {
			reset();
			continue;
		}

		if (currentCommand.find("accepted") != not_found)
			continue;

		if(currentCommand.find("usermove") != not_found) {
				
				//update board with user's new move
				currentCommand = currentCommand.substr(9, 14);  //POZITII
				oldPosition = 7 - (currentCommand[0] - 'a') + (currentCommand[1] - '1') * 8;
				newPosition = 7 - (currentCommand[2] - 'a') + (currentCommand[3] - '1') * 8;
				board.movePiece( *(*board.allPieces + oldPosition), newPosition);
				
				if (board.isCheckMate())
					sendCommand(COMMAND_RESIGN);

				//time for engine to move
				if ((board.blackPieces[KNIGHTS].size()) && foo == 0 && board.isMovable(KNIGHTS, BLACK)) {
					piece = board.blackPieces[KNIGHTS][rand() % board.blackPieces[KNIGHTS].size()];
					while ((newPosition = board.getPossiblePosition(piece)) == -1)
						piece = board.blackPieces[KNIGHTS][rand() % board.blackPieces[KNIGHTS].size()];
		
					foo = 1;
				} else  if (board.blackPieces[PAWNS].size()) {
					if (board.isMovable(PAWNS, BLACK)) {
						std::cout << "# " << board.isMovable(PAWNS, BLACK) << "\n";
						piece = board.blackPieces[PAWNS][rand() % board.blackPieces[PAWNS].size()];
						while ((newPosition = board.getPossiblePosition(piece)) == -1)
							piece = board.blackPieces[PAWNS][rand() % board.blackPieces[PAWNS].size()];
						foo = 0;
					} else {
						sendCommand(COMMAND_RESIGN);
					}
				} else {
					sendCommand(COMMAND_RESIGN);
				}
				
				command = computeCommnandForWinboard(piece->currentPosition, newPosition);
				board.movePiece(piece, newPosition);
				sendCommand(command);
				continue;
		}
	}
}