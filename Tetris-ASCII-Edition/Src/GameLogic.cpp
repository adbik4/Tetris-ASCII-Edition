#include <algorithm>
#include "GameEngine.h"
#include <SaveSystem.h>

// Implements the TETRIS game logic.
// Decides what happens after a given k_input press
void GameEngine::gameLogic(const int& k_input) {
	if (state->active_piece.is_falling) {
		state->active_piece.soft_drop(state->board);
		return;
	}

	// keyboard input
	switch (k_input) {
	case (int)'q':
	case (int)'j':
		state->active_piece.rotateL(state->board);
		break;

	case (int)'e':
	case (int)'k':
		state->active_piece.rotateR(state->board);
		break;

	case (int)'w':
	case KEY_UP:
		state->active_piece.hard_drop(state->board);
		break;

	case (int)'a':
	case KEY_LEFT:
		state->active_piece.moveL(state->board);
		break;

	case (int)'s':
	case KEY_DOWN:
		state->active_piece.soft_drop(state->board);
		break;

	case (int)'d':
	case KEY_RIGHT:
		state->active_piece.moveR(state->board);
		break;

	case 27: // ESC
		if (!(state->game_over)) {
			gameOver();
		}
	}

	// create refs for easier access
	array<char, BOARD_W* BOARD_H>& board = state->board;
	Tetromino& active_piece = state->active_piece;
	Tetromino& ghost_piece = state->ghost_piece;
	Tetromino& next_piece = state->next_piece;

	// increment level
	if ((state->lines >= state->line_goal) && (state->level != MAX_LEVEL)) {
		state->level++;
		state->line_goal += 10;
	}

	// gravity
	uint16_t drop_interval = static_cast<uint16_t>(BASE_SPEED - ((state->level - 1) * LEVEL_DECR));
	if (!drop_interval) {
		throw std::invalid_argument("<engine::update> Level got too high");
	}
	if (state->tick % drop_interval == 0) {
		active_piece.soft_drop(board);
	}

	if (active_piece.get_piece_id() == NULL) {
		// assign points for dropping
		uint8_t score_mult = static_cast<uint8_t>((floor(state->level / 2.0))) + 1;
		state->score += active_piece.fall_dist * SCORE_DEF[0] * score_mult;

		// draw the next tetromino piece
		uint8_t random_id;
		while (active_piece.get_piece_id() == NULL) {
			if (state->cfg.pure_randomness) {
				random_id = pure_randomizer();
			}
			else {
				random_id = TGM3_randomizer();
			}
			active_piece = next_piece;
			next_piece.reset(random_id);
		}
		
		ghost_piece.set_piece_id(active_piece.get_piece_id());

		renderer->refreshNextPieceUI();

		// also, since the last piece has just been merged...

		bool is_full = true;
		vector<uint8_t> lines_cleared;

		// for each line in the board
		// in reverse order so elements in the lines_cleared vector are in the right order
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
				lines_cleared.push_back(row);
				for (uint8_t i = 0; i < BOARD_W; i++) {
					SAMPLE_BOARD(board, i, row) = '.';
				}
			}
		}

		// check for perfect clear
		bool is_perfect_clear = (find_if_not(board.begin(), board.end(), [](char i) {return i == '.';}) == board.end()) ? true : false;

		if (lines_cleared.size() != 0) {
			// assign points for clearing
			uint16_t score = SCORE_DEF[lines_cleared.size()] * score_mult;

			if (!is_perfect_clear) {
				state->score += score;
				state->lines += (uint16_t)lines_cleared.size();

				renderer->lineClearEffect(lines_cleared, score);
				this_thread::sleep_for(chrono::milliseconds(500));
			}
			else {
				renderer->windowPrintAtPos(GAME_WIN, 3, BOARD_H/2, "PERFECT CLEAR");
				score *= 10;	// 10x multiplier
				state->score += score;
				for (auto i = 0; i < 5; i++) {
					renderer->lineClearEffect(lines_cleared, score);
					this_thread::sleep_for(chrono::milliseconds(100));
				}
			}

			// fill in the blanks by shifting down the upper portion of the board
			// starting from the lines_cleared line
			int8_t move_count{ 0 };
			for (auto start_idx : lines_cleared) {
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

	// vertical out of bounds
	if (active_piece.y_pos < 0) {
		gameOver();
	}
	else if (ghost_piece.y_pos < TETROMINO_W) {
		/// check if the ghost piece is overlapping with the start position
		char active_tile;
		char ghost_tile;
		bool game_over = false;

		// for each overlapping tile pair
		for (int8_t y = 0; y < TETROMINO_W - ghost_piece.y_pos; y++) {
			for (uint8_t x = 0; x < 4; x++) {
				ghost_tile = ghost_piece.realize_piece(x, y);
				active_tile = active_piece.realize_piece(x, y + ghost_piece.y_pos);

				if (active_tile != '.' && ghost_tile != '.') {
					gameOver();
					game_over = true;
					break;
				}
			}

			if (game_over) {
				break;
			}
		}
	}
	else {
		// check if the top row has something in it
		for (uint8_t col = 0; col < BOARD_W; col++) {
			if (SAMPLE_BOARD(board, col, 0) != '.') {
				gameOver();
			}
		}
	}
}

// Resets the game state and UI, preparing it for a new game
void GameEngine::restartGame() {
	state->reset();
	state->active_piece = Tetromino();
	renderer->initGameUI();
	renderer->windowReset(GAME_WIN);
}

// Ends the game, checks for hi-score and updates the UI
void GameEngine::gameOver() {
	if (state->stop_flag) {
		return;
	}

	state->game_over = true;

	if (state->score > state->cfg.hi_score) {
		state->cfg.hi_score = state->score;
	}

	saveState(getState());
	renderer->renderFrame();

	for (auto i = 0; i < 3; i++) {
		this_thread::sleep_for(chrono::milliseconds(100));
		renderer->flashEffect();
	}

	renderer->showEndScreen(getState());
	int k_input = input_mgr->waitForAnyKey();

	switch (k_input) {
	case 27: // ESC
		state->active_label = 0;
		renderer->initMenuUI();
		state->active_window = MENU;
		return;
	}

	restartGame();
}

// Returns a random piece index using the TETRIS TGM3 algorithm.
// the game keeps track of a piece_bag with 35 pieces, initialised with 5 counts of each variant.
// to mitigate floods, it also tracks the last four pieces that were played.
// it will take a piece from the piece_bag at random, and check (up to six times) if it was recently played
// to prevent droughts, the missing spot in the piece_bag will get filled by the most needed piece
uint8_t GameEngine::TGM3_randomizer() {
	uint8_t piece, tmp;
	uint16_t random_idx;

	// roll six times
	for (uint8_t _ = 0; _ < 6; _++) {
		random_idx = static_cast<uint8_t>(uniform_int_distribution<int16_t>(0, 34)(rng));
		piece = piece_bag[random_idx];
		if (find(history.begin(), history.end(), piece) == history.end()) {
			break;
		}
	}

	// manage the drought history
	auto idx = find(drought_history.begin(), drought_history.end(), piece);
	if (idx == drought_history.end()) {
		drought_history.push_front(piece);
	}
	else {
		// update drought history
		tmp = *idx;
		drought_history.erase(idx);
		drought_history.push_front(tmp);
	}

	// replace the gap with the least used piece
	if (random_idx < 35) {
		piece_bag[random_idx] = drought_history.back();
	}

	// update history
	history.push_front(piece);
	history.pop_back();

	return piece;
}

// Returns a purely random piece without messing around
uint8_t GameEngine::pure_randomizer() {
	return static_cast<uint8_t>(uniform_int_distribution<int16_t>(1, 7)(rng));
}