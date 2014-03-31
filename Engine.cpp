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
	board.erase();
	board.init();
	isForced = false;
	engineColor = BLACK;
	colorToMove = WHITE;
}

void Engine::processCommand(Command &command) {
	/*if (engineColor == WHITE) {
		if (command.find("move") != not_found) {
			command[5] = 'i' - command[5] + 96;
			command[6] = '9' - command[6] + '0';
			command[7] = 'i' - command[7] + 96;
			command[8] = '9' - command[8] + '0';
		}
		else {
			command[0] = 'i' - command[0] + 96;
			command[1] = '9' - command[1] + '0';
			command[2] = 'i' - command[2] + 96;
			command[3] = '9' - command[3] + '0';
		}
	}*/
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

	processCommand(command);
	std::cout << command;
	std::fflush(stdout);
}

void Engine::force() {
	isForced = true;
	std::cout << "#force\n";
}

void Engine::go() {
	int t;
	isForced = false;
	engineColor = colorToMove;
	std::cout << "#go\n";
	std::cout << "#" << colorToMove << "\n";
	engineMove();
}

void Engine::engineMove() {
	Piece *piece;
	Command command;
	Position newPosition = -1;

	if (isForced)
		board.printDebug();

	if (!isForced) {

		if (board.isCheckMate()) {
			sendCommand(COMMAND_RESIGN);
			return;
		}

		if (board.piecesVector[engineColor][PAWNS].size() == 0 || !board.isMovable(PAWNS, engineColor))
			foo = 0;
		//time for engine to move
		
			if ((board.piecesVector[engineColor][QUEEN].size()) && foo == 0 && board.isMovable(QUEEN, engineColor)) {
				piece = board.piecesVector[engineColor][QUEEN][rand() % board.piecesVector[engineColor][QUEEN].size()];
				while ((newPosition = board.getPossiblePosition(piece)) == -1)
					piece = board.piecesVector[engineColor][QUEEN][rand() % board.piecesVector[engineColor][QUEEN].size()];

				foo = 1;
			}
			else  if (board.piecesVector[engineColor][PAWNS].size()) {
				if (board.isMovable(PAWNS, engineColor)) {
					std::cout << "# " << board.isMovable(PAWNS, engineColor) << "\n";
					piece = board.piecesVector[engineColor][PAWNS][rand() % board.piecesVector[engineColor][PAWNS].size()];
					while ((newPosition = board.getPossiblePosition(piece)) == -1)
						piece = board.piecesVector[engineColor][PAWNS][rand() % board.piecesVector[engineColor][PAWNS].size()];

				}
									foo = 0;
			}
			else {
				sendCommand(COMMAND_RESIGN);
				return;
			}
			command = computeCommnandForWinboard(piece->currentPosition, newPosition);
			board.movePiece(piece, newPosition);
			sendCommand(command);
		
		colorToMove = (colorToMove == WHITE) ? BLACK : WHITE;
	}
}

void Engine::mainLoop() {
	std::string token;
	Command command;
	Position oldPosition;
	Position newPosition;
	time_t t;
		
	std::cout.setf(std::ios::unitbuf);
	foo = 0;
	srand((unsigned int)time(NULL));

	colorToMove = WHITE;
	isForced = false;
	engineIsToMove = false;
	engineColor = BLACK;

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

		if (currentCommand.find("go") != not_found) {
			go();
			continue;
		}

		if (currentCommand.find("accepted") != not_found)
			continue;

		if (currentCommand.find("force") != not_found) {
			force();
			continue;
		}

		if (currentCommand.find("new") != not_found) {
			reset();
			continue;
		}

		if (currentCommand.find("usermove") != not_found) {

			//update board with user's new move

			currentCommand = currentCommand.substr(9, 14);  //POZITII
			processCommand(currentCommand);
			oldPosition = 7 - (currentCommand[0] - 'a') + (currentCommand[1] - '1') * 8;
			newPosition = 7 - (currentCommand[2] - 'a') + (currentCommand[3] - '1') * 8;
			board.movePiece(*(*board.allPieces + oldPosition), newPosition);

			colorToMove = (colorToMove == BLACK) ? WHITE : BLACK;
		}

		if (colorToMove == engineColor) {
			t = time(NULL);
			engineMove();
			t = time(NULL) - t;
			std::cout<<t;
		}
	}
}