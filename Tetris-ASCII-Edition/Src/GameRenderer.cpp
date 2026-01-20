#include "GameRenderer.h"
#include "Tetromino.h"
#include "Constants.h"

using namespace std;

vector<string> menu_labels = { "Start Game", "Settings", "Exit" }; 	// defines the label contents for the main menu
vector<string> settings_labels = { "ASCII mode", "Flashing effects", "Randomness", "Show future piece", "Start level", "Back"}; // defines the label contents for the settings menu


// Renders the contents of the GAME_WIN based on the current GameState
void GameRenderer::renderFrame() {
	auto engine = eng.lock();
	if (!engine) {
		return;
	}

	wclear(game_win); // clear the screen

	// 1st pass - board
	for (uint8_t y = 0; y < BOARD_H; y++) {
		for (uint8_t x = 0; x < BOARD_W; x++) {
			render_tile(SAMPLE_BOARD(engine->getState().board, x, y));
		}
	}

	if (engine->getState().active_piece.get_piece_id() != NULL) {
		// 2nd pass - ghost piece
		for (uint8_t j = 0; j < TETROMINO_W; j++) {
			for (uint8_t i = 0; i < TETROMINO_W; i++) {
				render_piece(engine->getState().ghost_piece, i, j);
			}
		}

		// 3rd pass - active tetromino
		for (uint8_t y = 0; y < TETROMINO_W; y++) {
			for (uint8_t x = 0; x < TETROMINO_W; x++) {
				render_piece(engine->getState().active_piece, x, y);
			}
		}
	}

	wrefresh(game_win);
}

// Flashes the screen if flashing effects are enabled
void GameRenderer::flashEffect() {
	auto engine = eng.lock();
	if (!engine) {
		return;
	}

	if (engine->getState().cfg.flash_on_clear) {
		flash();
	}
}

// Displays an effect for line clears
void GameRenderer::lineClearEffect(vector<uint8_t> lines, uint16_t score) {
	auto engine = eng.lock();
	if (!engine) {
		return;
	}

	// generate padding for centering
	array<char, BOARD_W - 1> pad;

	fill(begin(pad), end(pad), ' ');
	pad[BOARD_W - 2] = '\0';
	
	for (auto idx : lines) {
		mvwprintw(game_win, idx, 0, "%s+%u%s", pad.data(), score, pad.data());
	}

	wrefresh(game_win);

	flashEffect();
}

// UTILITY ----
// Renders a background tile
void GameRenderer::render_tile(const char tile) {
	auto engine = eng.lock();
	if (!engine) {
		return;
	}

	char block[3] = { ' ', ' ', '\0' };

	if (engine->getState().cfg.ascii_mode) {
		if (tile == '.') {
			block[0] = ' ';
			block[1] = ' ';
		}
		else {
			block[0] = tile;
			block[1] = tile;
		}
		waddstr(game_win, block);
	}
	else {
		chtype color = 1;
		switch (tile) {
		case '.':	// empty space
			color = COLOR_PAIR(0);
			break;
		case '@':
			color = COLOR_PAIR(1);
			break;
		case '#':
			color = COLOR_PAIR(2);
			break;
		case 'o':
			color = COLOR_PAIR(3);
			break;
		case '$':
			color = COLOR_PAIR(4);
			break;
		case '*':
			color = COLOR_PAIR(5);
			break;
		case '%':
			color = COLOR_PAIR(6);
			break;
		case '&':
			color = COLOR_PAIR(7);
			break;
		case 'E':	// error
			color = COLOR_PAIR(4);
			break;
		}

		wattron(game_win, color);
		waddstr(game_win, "  ");
		wattroff(game_win, color);
	}
}

// Renders the entirety of a given tetromino piece in a given window
void GameRenderer::render_piece(const Tetromino& piece, const uint8_t x, const uint8_t y, const int& win_id) {
	auto engine = eng.lock();
	auto win_mgr = wm.lock();
	if (!engine || !win_mgr) {
		return;
	}

	WINDOW* local_win = win_mgr->getWindow(win_id);
	char tile = piece.realize_piece(x, y);
	char block[3] = { ' ', ' ', '\0' };

	const int draw_y = (win_id == GAME_WIN) ? piece.y_pos + y : 1 + y;
	const int draw_x = (win_id == GAME_WIN) ? (piece.x_pos + x) * 2 : x * 2;

	if (tile == '.') {
		return;
	}

	if (engine->getState().cfg.ascii_mode) {
		if (piece.is_ghost) {
			block[0] = GHOST_SYM;
			block[1] = GHOST_SYM;
		}
		else {
			block[0] = tile;
			block[1] = tile;
		}

		mvwprintw(local_win, draw_y, draw_x, block);
	}
	else {
		chtype color = 1;
		switch (tile) {
		case '/': // ghost piece
			color = COLOR_PAIR(2);
			break;
		case '@':
			color = COLOR_PAIR(1);
			break;
		case '#':
			color = COLOR_PAIR(2);
			break;
		case 'o':
			color = COLOR_PAIR(3);
			break;
		case '$':
			color = COLOR_PAIR(4);
			break;
		case '*':
			color = COLOR_PAIR(5);
			break;
		case '%':
			color = COLOR_PAIR(6);
			break;
		case '&':
			color = COLOR_PAIR(7);
			break;
		case 'E':
			// error
			color = COLOR_PAIR(4);
			break;
		}

		if (piece.is_ghost) {
			block[0] = GHOST_SYM;
			block[1] = GHOST_SYM;
			mvwprintw(local_win, draw_y, draw_x, block);
		}
		else {
			wattron(local_win, color);
			mvwprintw(local_win, draw_y, draw_x, "  ");
			wattroff(local_win, color);
		}
	}
}

// Renders the ASCII art title
void GameRenderer::showTitleScreen() {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	WINDOW* title_win = win_mgr->getWindow(TITLE_WIN);
	wclear(title_win);
	waddnstr(title_win, title_art.data(), -1);
	wrefresh(title_win);
}

// Renders the contents of the end screen based on the GameState
void GameRenderer::showEndScreen(const GameState& state) {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	win_mgr->clearContents(GAME_WIN);
	win_mgr->clearWindow(STATS_WIN);
	win_mgr->clearWindow(NEXT_WIN);

	if (state.score == state.cfg.hi_score) {
		windowPrint(GAME_WIN, "!!!NEW HIGH SCORE!!!\n");
	}
	else {
		windowPrint(GAME_WIN, "GAME OVER.\n");
	}

	windowPrint(GAME_WIN, "score: " + to_string(state.score) + "\n");
	windowPrint(GAME_WIN, "lines cleared: " + to_string(state.lines) + "\n");
	windowPrint(GAME_WIN, "level reached: " + to_string(state.level) + "\n");
	windowPrint(GAME_WIN, "\nPress [any key]\nto start new game\n");
	windowPrint(GAME_WIN, "\nor\n\n[ESC] to go to menu\n");

	this_thread::sleep_for(chrono::milliseconds(1000));	// delay to give the player time for reaction
}

// Appends a message to the end of the given window and displays it
void GameRenderer::windowPrint(const int& win_id, const string& str) {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	WINDOW* local_win = win_mgr->getWindow(win_id);

	waddstr(local_win, str.c_str());
	wrefresh(local_win);
}

// Displays a message at a given position in the given window
void GameRenderer::windowPrintAtPos(const int& win_id, const int& x, const int& y, const string& str) {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	WINDOW* local_win = win_mgr->getWindow(win_id);

	if (mvwaddstr(local_win, y, x, str.c_str()) < 0) {
		throw std::out_of_range("<windowPrintAtPos> out of bounds");
	}
	wrefresh(local_win);
}

// Prints an error in the ERR_WIN
void GameRenderer::errPrint(const string& str) {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	WINDOW* err_win = win_mgr->getWindow(ERR_WIN);
	win_mgr->clearContents(ERR_WIN);

	waddstr(err_win, str.c_str());
	wrefresh(err_win);
}

// Clears the contents of a given window
void GameRenderer::windowReset(const int& win_id) {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	win_mgr->clearContents(win_id);
}