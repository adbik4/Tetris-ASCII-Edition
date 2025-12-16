#include "GameRenderer.h"
#include "Tetromino.h"
#include <stdexcept>
#include <string>
#include "Constants.h"

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
	wclear(game_win); // clear the screen

	// 1st pass - board
	for (uint8_t y = 0; y < BOARD_H; y++) {
		for (uint8_t x = 0; x < BOARD_W; x++) {
			render_tile(SAMPLE_BOARD(engine->getState().board, x, y));
		}
	}

	// 2nd pass - active tetromino
	for (uint8_t y = 0; y < 4; y++) {
		for (uint8_t x = 0; x < 4; x++) {
			render_piece(x, y);
		}
	}

	wrefresh(game_win);
}

void GameRenderer::render_tile(const char tile) {
	if (engine->getState().ascii_mode) {
		block_buff[0] = tile;
		block_buff[1] = tile;
		wprintw(game_win, block_buff.data());
	}
	else {
		chtype color = 1;
		switch (tile) {
		case '.':
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
		case 'E':
			// error
			color = COLOR_PAIR(4);
			break;
		}

		wattron(game_win, color);
		wprintw(game_win, "  ");
		wattroff(game_win, color);
	}
}

void GameRenderer::render_piece(const uint8_t x, const uint8_t y) {
	char tile = engine->getState().active_piece.realize_piece(x, y);
	if (tile == '.') {
		return;
	}

	if (engine->getState().ascii_mode) {
		block_buff[0] = tile;
		block_buff[1] = tile;

		mvwprintw(
			game_win,
			engine->getState().active_piece.y_pos + y,
			(engine->getState().active_piece.x_pos + x) * 2,
			block_buff.data()
		);
	}
	else {
		chtype color = 1;
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
		case 'E':
			// error
			color = COLOR_PAIR(4);
			break;
		}

		wattron(game_win, color);
		mvwprintw(
			game_win,
			engine->getState().active_piece.y_pos + y,
			(engine->getState().active_piece.x_pos + x) * 2,
			"  "	// two spaces = one block
		);
		wattroff(game_win, color);
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

	WINDOW* title_win = win_mgr->getWindow(TITLE_WIN);
	wclear(title_win);
	wrefresh(title_win);

	win_mgr->showBorder(GAME_WIN);
}

void GameRenderer::initSettingsUI() {
	win_mgr->clearContents(MAIN_MENU);
	win_mgr->clearContents(INPUT_WIN);
}

void GameRenderer::showEndScreen() {
	windowPrint(MAIN_MENU, "Exiting the game...\n");;
}