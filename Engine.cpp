#include <stdio.h>
#include <stdlib.h>

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
	board.tempRemovedPieces.clear();
}

void Engine::force() {
	isForced = true;
	std::cout << "#force\n";
}

void Engine::go() {
	isForced = false;
	engineColor = colorToMove;
	engineMove();
}

Board::MoveScore Engine::negamax(PIECE_COLOR playerColor, int depth, int alpha, int beta) {
	/*
	if (board.hasBeenEvald(playerColor)) {
		return board.getMove();
	}*/

	if (depth == 0) {
		return Board::MoveScore(nullptr, board.evaluate(playerColor));
	}


	Board::MoveScore bestMove(nullptr, INT_MIN);
	for (int i = 0; i < 6; i++) {
		for (unsigned int j = 0; j < board.piecesVector[playerColor][i].size(); j++) {

			std::vector<Board::Move*> moves = board.getPossiblePosition(board.piecesVector[playerColor][i][j]);

			for (unsigned int k = 0; k < moves.size(); k++) {

				moves[k]->apply();

				if (board.isCheckMate(engineColor)) {
					moves[k]->undo();
					continue;
				}

				Board::MoveScore currentMove = negamax(((playerColor == WHITE) ? BLACK : WHITE), depth - 1, -beta, -alpha);
				currentMove.score = -currentMove.score;
			
				currentMove.move = moves[k];
				//if (dynamic_cast<Board::EnPassant*>(moves[k]) != 0) {
				//	currentMove.second += 200000;
				//}

				if (currentMove.score > alpha) {
					alpha = currentMove.score;
					/*
					bestMove.first = currentMove.first;
					bestMove.second = alpha;
					*/
					bestMove = currentMove;
				}

				if (alpha >= beta) {
					bestMove.score = beta;
					moves[k]->undo();
					board.addToHash(bestMove, playerColor);
					return bestMove;
				}
				moves[k]->undo();
			}
		}
	}
	if (bestMove.move != nullptr)
		board.addToHash(bestMove, playerColor);
	return bestMove;
}

void Engine::engineMove() {
	Command command;

	if (!isForced) {
		Board::MoveScore bestMove = negamax(engineColor, DEPTH, -200000, 200000);

		board.printHash();

		if (bestMove.score == INT_MIN) {
			sendCommand("resign");
			return;
		}
		bestMove.move->apply();
		//std::cout << "# " << ((CastlingPiece*)board.piecesVector[BLACK][KING][0])->moveCount << "rege\n";

		//board.printBitboard(board.boardsVector[BLACK]);
		//board.printPointerBoard(BLACK);
		command = computeCommnandForWinboard(bestMove.move->oldPosition, bestMove.move->newPosition);

		
		colorToMove = (colorToMove == WHITE) ? BLACK : WHITE;
		//board.printBitboard(board.boardsVector[WHITE]);
		//board.printPointerBoard(WHITE);
		sendCommand(command);

	}
}

void Engine::mainLoop() {
	std::string token;
	Command command;
	Position oldPosition;
	Position newPosition;
		
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

			currentCommand = currentCommand.substr(9, 14);  //POZITII
			processCommand(currentCommand);
			oldPosition = 7 - (currentCommand[0] - 'a') + (currentCommand[1] - '1') * 8;
			newPosition = 7 - (currentCommand[2] - 'a') + (currentCommand[3] - '1') * 8;

			board.applyInputMove(oldPosition, newPosition, currentCommand[4]);
			colorToMove = (colorToMove == BLACK) ? WHITE : BLACK;
		}

		if (colorToMove == engineColor) {
			engineMove();
		}
	}
}