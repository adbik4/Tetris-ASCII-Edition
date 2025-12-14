#include "GameRenderer.h"
#include "Tetromino.h"
#include <stdexcept>
#include <string>

using namespace std;

string title_art =
"oooooooooooooooooo ooooooooooooooooo oooooooooooooooooo ooooooooooooooo     oooooooo    oooooooooooooo\n"
"oooooooooooooooooo ooooooooooooooooo oooooooooooooooooo ooooooooooooooooo   oooooooo  ooooooooooooooo\n"
"oooooooooooooooooo ooooooooooooooooo oooooooooooooooooo oooooooooooooooooo  oooooooo ooooooooooooooo\n"
"oooooooooooooooooo ooooooooooooooooo oooooooooooooooooo oooooooooooooooooo  ooooooo ooooooooooooooo\n"
"     oooooooo      ooooooo                oooooooo      ooooooo  ooooooooo  oooooooo ooooooooo\n"
"     oooooooo      ooooooooooooooo        oooooooo      ooooooooooooooooo   oooooooo oooooooooo\n"
"     oooooooo      ooooooooooooooo        oooooooo      oooooooooooooooo    oooooooo   ooooooooo\n"
"     oooooooo      oooooooooooooo         oooooooo      oooooooooooooooo    oooooooo    oooooooooo\n"
"     oooooooo      ooooooooooooo          oooooooo      ooooooo ooooooooo   oooooooo     oooooooooo\n"
"     oooooooo      oooooooo               oooooooo      ooooooo  ooooooooo  oooooooo       ooooooooo\n"
"     oooooooo      oooooooo               oooooooo      oooooooo   oooooooo oooooooo        oooooooooo\n"
"     oooooooo      oooooooooooooooooo     oooooooo      oooooooo    ooooooo oooooooo ooooooooooooooooo\n"
"     oooooooo      oooooooooooooooooooo   oooooooo      oooooooo     oooooo oooooooo ooooooooooooooooo\n"
"     oooooooo      ooooooooooooooooooooo  oooooooo      oooooooo      ooooo oooooooo ooooooooooooooooo\n"
"     oooooooo        oooooooooooooooooooo oooooooo      oooooooo       oooo oooooooo ooooooooooooooo";



void GameRenderer::renderFrame() {
	wclear(game_win_); // reset the screen

	string board = engine->getState().board;

	// 1st pass - board
	for (char tile : board) {
		render_tile(tile);
	}

	// 2nd pass - active tetromino
	for (uint8_t y = 0; y < 4; y++) {
		for (uint8_t x = 0; x < 4; x++) {
			render_piece(x, y);
		}
	}

	wrefresh(game_win_);
}

void GameRenderer::render_tile(const char& tile) {
	if (tile == '.') {
		return;
	}

	if (engine->getState().ascii_mode) {
		memset(block_buf_, tile, 2);
		block_buf_[2] = '\0';
		windowPrint(GAME_WIN, block_buf_);
	}
	else {
		chtype color = 6;
		switch (tile) {
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
		}

		wattron(game_win_, color);
		windowPrint(GAME_WIN, "  ");
		wattroff(game_win_, color);
	}
}

void GameRenderer::render_piece(const uint8_t& x, const uint8_t& y) {
	char tile = engine->getState().active_piece.lookup_piece(x, y);
	if (tile == '.') {
		return;
	}

	if (engine->getState().ascii_mode) {
		memset(block_buf_, tile, 2);
		block_buf_[2] = '\0';

		mvwprintw(
			game_win_,
			engine->getState().active_piece.y_pos + y,
			(engine->getState().active_piece.x_pos + x) * 2,
			block_buf_
		);
	}
	else {
		chtype color = 6;
		switch (tile) {
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
		}

		wattron(game_win_, color);
		mvwprintw(
			game_win_,
			engine->getState().active_piece.y_pos + y,
			(engine->getState().active_piece.x_pos + x) * 2,
			"  "	// two spaces = one block
		);
		wattroff(game_win_, color);
	}
}

void GameRenderer::blink() {
	flash();
}

// UTILITY ----
void GameRenderer::windowPrint(const int& win_id, const string& str) {
	WINDOW* local_win = win_mgr->getWindow(win_id);

	wprintw(local_win, str.c_str());
	wrefresh(local_win);
}

void GameRenderer::errPrint(const string& str) {
	WINDOW* err_win = win_mgr->getWindow(ERR_WIN);
	win_mgr->clearContents(ERR_WIN);

	wprintw(err_win, str.c_str());
	wrefresh(err_win);
}

void GameRenderer::showMenu() {
	WINDOW* title_win = win_mgr->getWindow(TITLE_WIN);
	waddnstr(title_win, title_art.c_str(), -1);
	wrefresh(title_win);

	WINDOW* menu_win = win_mgr->getWindow(MAIN_MENU);

	mvwprintw(menu_win, 1, 0, "1) Start Game\n");
	mvwprintw(menu_win, 2, 0, "2) Settings\n");
	mvwprintw(menu_win, 3, 0, "3) Exit\n");

	win_mgr->showBorder(MAIN_MENU);
	mvwprintw(menu_win, 0, 6, "MAIN MENU");
	wrefresh(menu_win);

	engine->notify(Event(INT_INPUT, { 1, 3 }));
}

void GameRenderer::initGameUI() {
	win_mgr->clearWindow(MAIN_MENU);
	win_mgr->clearWindow(INPUT_WIN);
	win_mgr->clearWindow(TITLE_WIN);
	win_mgr->showBorder(GAME_WIN);
}

void GameRenderer::initSettingsUI() {
	win_mgr->clearContents(MAIN_MENU);
	win_mgr->clearContents(INPUT_WIN);
}

void GameRenderer::showEndScreen() {
	windowPrint(MAIN_MENU, "Exiting the game...\n");;
}