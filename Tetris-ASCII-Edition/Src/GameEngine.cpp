#include <exception>
#include <random>
#include "GameEngine.h"
#include "Tetromino.h"

using namespace std;

int int_input;
mt19937 rng((uint8_t) chrono::system_clock::now().time_since_epoch().count());

void GameEngine::startEngine() {
	state->running = true;

	try {
		renderer->showMenu();

		switch (int_input) {
		case 1:
			renderer->initGameUI();
			time_mgr->startClock();
			break;
		case 2:
			renderer->initSettingsUI();
			renderer->windowPrint(MAIN_MENU, "SETTINGS\n");
			break;
		case 3:
			state->stop_flag = true;
			break;
		}
	}
	catch (const std::exception& err) {
		string message = string("[DEBUG]: ") + err.what() + "\n";
		renderer->errPrint(message);
	}
	catch (...) {
      renderer->errPrint("[DEBUG]: Unknown exception\n");
	}
}

uint8_t target_x = 255, target_y = 0;

void GameEngine::update() {
	// create refs for easier access
	array<char, BOARD_W* BOARD_H>& board = state->board;
	Tetromino& active_piece = state->active_piece;
	Tetromino& ghost_piece = state->ghost_piece;

	try {
		if (active_piece.is_falling) {
			active_piece.soft_drop(board);
		} 
		else {
			// ==== INPUT ====
			int k_input = input_mgr->getKeyboardInput(GAME_WIN);
			switch (k_input) {
				case (int)'q':
				case (int)'j':
					active_piece.rotateL(board);
					break;

				case (int)'e':
				case (int)'k':
					active_piece.rotateR(board);
					break;

				case (int)'w':
				case KEY_UP:
					active_piece.hard_drop(board);
					break;

				case (int)'a':
				case KEY_LEFT:
					active_piece.moveL(board);
					break;

				case (int)'s':
				case KEY_DOWN:
					active_piece.soft_drop(board);
					break;

				case (int)'d':
				case KEY_RIGHT:
					active_piece.moveR(board);
					break;

				case 27: // ESC
					stopEngine();
			}

			// ==== GAME LOGIC ====

			// gravity
			uint16_t drop_interval = BASE_SPEED - ((state->level - 1) * LEVEL_DECR);
			if (!drop_interval) {
				throw std::invalid_argument("<engine::update> Level got too high");
			}
			if (state->tick % drop_interval == 0) {
				active_piece.soft_drop(board);
			}

			if (active_piece.get_piece_id() == NULL) {
				// generate a new piece if needed
				active_piece.next_piece(rng);
				ghost_piece.set_piece_id( active_piece.get_piece_id() );

				// also, since the last piece has just been merged, check for filled lines
				// for line in board
				// if it's full: 
				// clear line
				// give points
				// flash screen
				// wait for 1s
				// fill in the blanks
	
				bool is_full;
				uint8_t fill_count{ 0 };

				for (uint8_t row = 0; row < BOARD_H; row++) {
					is_full = true;

					// check line
					for (uint8_t col = 0; col < BOARD_W; col++) {
						if (SAMPLE_BOARD(board, col, row) == '.') {
							is_full = false;
						}
					}

					//clear line
					if (is_full) {
						fill_count++;
						for (uint8_t i = 0; i < BOARD_W; i++) {
							SAMPLE_BOARD(board, i, row) = '.';
						}
					}
				}

				if (fill_count) {
					//increment points
					state->score += SCORE_DEF[fill_count] * (static_cast<uint64_t>((floor(state->level / 2.0))) + 1);
					renderer->blink();
				}
			}

			// refresh the ghost piece
			ghost_piece.y_pos = 0;
			ghost_piece.set_xpos(active_piece.x_pos);
			ghost_piece.set_rotation(active_piece.get_rotation());
			ghost_piece.ghost_drop(board);
		}

		// ==== RENDER OUTPUT ====
		renderer->renderFrame();
		renderer->refreshGameUI();
	}
	catch (const std::exception& err) {
		string message = string("[DEBUG]: ") + err.what() + "\n";
		renderer->errPrint(message);
		while (true) {};
	}
}

void GameEngine::stopEngine() {
	if (!state->stop_flag) {
		state->stop_flag = true;
		return;
	}
	else if (!state->running) {
		return;
	}

	state->running = false;
	renderer->showEndScreen();
	time_mgr->stopClock();
}

void GameEngine::nextLevel() {
	state->level++;
}

// This function conducts the main mediation logic
void GameEngine::notify (const Event& event) {
	switch (event.id) {
	case EventId::CLK:
		update();
		state->tick++;
		break;
	case EventId::INPUT_ERR:
		renderer->windowPrint(INPUT_WIN, "Invalid input\n");
		break;
	case EventId::INT_INPUT:
		 int_input = input_mgr->getIntInput(event.args);
		break;
	case EventId::GENERAL_ERR:
		renderer->errPrint("[DEBUG] an error has occured\n");
		break;
	default:
		renderer->errPrint("[DEBUG] GameEngine has recieved an undefined event\n");
		break;
	}
}