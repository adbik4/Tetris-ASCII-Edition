#include <exception>
#include "GameEngine.h"
#include "Tetromino.h"

using namespace std;

int int_input;

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
				// assign points for hard-dropping
				uint8_t score_mult = static_cast<uint8_t>((floor(state->level / 2.0))) + 1;
				state->score += active_piece.fall_dist * SCORE_DEF[0] * score_mult;

				// generate a new piece if needed
				active_piece.next_piece(rng);
				ghost_piece.set_piece_id(active_piece.get_piece_id());

				// also, since the last piece has just been merged...

				bool is_full;
				vector<uint8_t> cleared;

				// for each line in the board
				// in reverse order so elements in the cleared vector are in the right order
				for (int8_t row = BOARD_H - 1; row >= 0; row--) {
					is_full = true;

					// check line
					for (uint8_t col = 0; col < BOARD_W; col++) {
						if (SAMPLE_BOARD(board, col, row) == '.') {
							is_full = false;
						}
					}

					//clear line
					if (is_full) {
						cleared.push_back(row);
						for (uint8_t i = 0; i < BOARD_W; i++) {
							SAMPLE_BOARD(board, i, row) = '.';
						}
					}
				}

				if (cleared.size() != 0) {
					// assign points for clearing
					state->score += SCORE_DEF[cleared.size()] * score_mult;
					state->lines += (uint16_t)cleared.size();

					renderer->blink();
					
					renderer->renderFrame();
					this_thread::sleep_for(chrono::milliseconds(500));

					// fill in the blanks by shifting down the upper portion of the board
					// starting from the cleared line
					int8_t move_count{ 0 };
					for (auto start_idx : cleared) {
						for (int8_t row = start_idx + move_count; row > 0; row--) {
							for (uint8_t col = 0; col < BOARD_W; col++) {
								SAMPLE_BOARD(board, col, row) = SAMPLE_BOARD(board, col, row - 1); // move down
							}
						}
						move_count++;
					}
				}
			}

			// refresh the ghost piece
			ghost_piece.y_pos = 0;
			ghost_piece.set_xpos(active_piece.x_pos);
			ghost_piece.set_rotation(active_piece.get_rotation());
			ghost_piece.ghost_drop(board);

			// check for game over

			// the ghost piece is overlapping with the start position
			if (ghost_piece.y_pos < TETROMINO_W) {
				char active_tile;
				char ghost_tile;

				// for each overlapping tile pair
				for (int8_t y = 0; y < TETROMINO_W - ghost_piece.y_pos; y++) {
					for (uint8_t x = 0; x < 4; x++) {
						ghost_tile = ghost_piece.realize_piece(x, y);
						active_tile = active_piece.realize_piece(x, y + ghost_piece.y_pos);

						if (active_tile != '.' && ghost_tile != '.') {
							// game over
							throw std::runtime_error("GAME OVER");
						}
					}
				}
			}

			// the top row has something in it
			for (uint8_t col = 0; col < BOARD_W; col++) {
				if (SAMPLE_BOARD(board, col, 0) != '.') {
					// game over
					throw std::runtime_error("GAME OVER");
				}
			}
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