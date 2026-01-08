#include "GameRenderer.h"
#include "Tetromino.h"
#include <stdexcept>
#include <string>
#include "Constants.h"

using namespace std;

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
				render_piece(i, j, '/');
			}
		}

		// 3rd pass - active tetromino
		for (uint8_t y = 0; y < TETROMINO_W; y++) {
			for (uint8_t x = 0; x < TETROMINO_W; x++) {
				render_piece(x, y);
			}
		}
	}

	wrefresh(game_win);
}

void GameRenderer::render_tile(const char tile) {
	auto engine = eng.lock();
	if (!engine) {
		return;
	}

	char block[3] = { ' ', ' ', '\0' };

	if (engine->getState().ascii_mode) {
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

void GameRenderer::render_piece(const uint8_t x, const uint8_t y, const char force_tile) {
	auto engine = eng.lock();
	if (!engine) {
		return;
	}

	char tile = engine->getState().active_piece.realize_piece(x, y);
	int8_t x_pos, y_pos;
	char block[3] = {' ', ' ', '\0'};

	if (tile == '.') {
		return;
	}
	else if (force_tile) {
		tile = force_tile;
	}

	if (engine->getState().ascii_mode) {
		if (tile == '/') {
			x_pos = engine->getState().ghost_piece.x_pos;
			y_pos = engine->getState().ghost_piece.y_pos;
			block[0] = '.';
			block[1] = '.';
		}
		else {
			x_pos = engine->getState().active_piece.x_pos;
			y_pos = engine->getState().active_piece.y_pos;
			block[0] = tile;
			block[1] = tile;
		}

		mvwprintw(game_win, y_pos + y, (x_pos + x) * 2, block);
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

		if (tile == '/') {
			x_pos = engine->getState().ghost_piece.x_pos;
			y_pos = engine->getState().ghost_piece.y_pos;
			block[0] = GHOST_SYM;
			block[1] = GHOST_SYM;

			mvwprintw(game_win, y_pos + y, (x_pos + x) * 2, block);
		}
		else {
			x_pos = engine->getState().active_piece.x_pos;
			y_pos = engine->getState().active_piece.y_pos;

			wattron(game_win, color);
			mvwprintw(game_win, y_pos + y, (x_pos + x) * 2, "  ");
			wattroff(game_win, color);
		}


	}
}

void GameRenderer::refreshGameUI() {
	auto engine = eng.lock();
	auto win_mgr = wm.lock();
	if (!engine || !win_mgr) {
		return;
	}
	
	WINDOW* stats_win = win_mgr->getWindow(STATS_WIN);
	uint64_t hi_score = engine->getState().hi_score;
	uint64_t score = engine->getState().score;
	uint16_t lines = engine->getState().lines;
	uint8_t level = engine->getState().level;

	wclear(stats_win);
	mvwprintw(stats_win, 0, 0, "high score: %lld", hi_score);
	mvwprintw(stats_win, 1, 0, "score: %lld", score);
	mvwprintw(stats_win, 2, 0, "lines: %ld", lines);
	mvwprintw(stats_win, 4, 12, "level: %d", level);
	wrefresh(stats_win);
}

void GameRenderer::flashEffect() {
	auto engine = eng.lock();
	if (!engine) {
		return;
	}

	if (engine->getState().flash_on_clear) {
		flash();
	}
}

void GameRenderer::clearEffect(vector<uint8_t> lines, uint16_t score) {
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
void GameRenderer::windowPrint(const int& win_id, const string& str) {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	WINDOW* local_win = win_mgr->getWindow(win_id);

	waddstr(local_win, str.c_str());
	wrefresh(local_win);
}

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

void GameRenderer::windowReset(const int& win_id) {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	win_mgr->clearContents(win_id);
}

void GameRenderer::showMenu() {
	auto engine = eng.lock();
	auto win_mgr = wm.lock();
	if (!engine || !win_mgr) {
		return;
	}

	WINDOW* title_win = win_mgr->getWindow(TITLE_WIN);
	waddnstr(title_win, title_art.data(), -1);
	wrefresh(title_win);

	WINDOW* menu_win = win_mgr->getWindow(MAIN_MENU);

	mvwprintw(menu_win, 1, 0, "1) Start Game\n");
	mvwprintw(menu_win, 2, 0, "2) Settings\n");
	mvwprintw(menu_win, 3, 0, "3) Exit\n");

	win_mgr->showBorder(MAIN_MENU);
	mvwprintw(menu_win, 0, 6, "MAIN MENU");
	wrefresh(menu_win);

	engine->notify(Event(EventId::INT_INPUT, { 1, 3 }));
}

void GameRenderer::initGameUI() {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	win_mgr->clearWindow(MAIN_MENU);
	win_mgr->clearWindow(INPUT_WIN);

	WINDOW* title_win = win_mgr->getWindow(TITLE_WIN);
	wclear(title_win);
	wrefresh(title_win);

	win_mgr->showBorder(GAME_WIN);
	win_mgr->showBorder(STATS_WIN);
}

void GameRenderer::initSettingsUI() {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	win_mgr->clearContents(MAIN_MENU);
	win_mgr->clearContents(INPUT_WIN);
}

void GameRenderer::showEndScreen(const GameState& state) {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	win_mgr->clearContents(GAME_WIN);
	if (state.score == state.hi_score) {
		windowPrint(GAME_WIN, "!!!NEW HIGH SCORE!!!\n");
	}
	else {
		windowPrint(GAME_WIN, "GAME OVER.\n");
	}

	windowPrint(GAME_WIN, "score: " + to_string(state.score) + "\n");
	windowPrint(GAME_WIN, "lines cleared: " + to_string(state.lines) + "\n");
	windowPrint(GAME_WIN, "level reached: " + to_string(state.level) + "\n");
	windowPrint(GAME_WIN, "\nPress [any key]\nto start new game\n");
	windowPrint(GAME_WIN, "\nor [ESC] to quit\n");

	this_thread::sleep_for(chrono::milliseconds(1500));
}