#include "GameRenderer.h"
#include "Tetromino.h"
#include <stdexcept>
#include <string>

using namespace std;

void GameRenderer::renderFrame() {
	WINDOW* game_win = win_mgr->getWindow(GAME_WIN);
	string board = engine->getState().board;
	char block[3];

	// 1st pass - board
	for (char tile : board) {
		if (true || tile != '.') {
			memset(block, tile, 2);
			block[2] = '\0';
			windowPrint(GAME_WIN, block);
		}
	}

	// 2nd pass - active tetromino
	Tetromino piece = engine->getState().active_piece;
	uint8_t x, y;
	char tile;
	for (y = 0; y < 4; y++) {
		for (x = 0; x < 4; x++) {
			tile = piece.lookup_piece(x, y);
			memset(block, tile, 2);
			block[2] = '\0';
			mvwprintw(game_win, piece.y_pos+y, 2*(piece.x_pos+x), block);
		}
	}
}

void GameRenderer::windowPrint(const int& win_id, const string& str) {
	WINDOW* local_win = win_mgr->getWindow(win_id);

	wprintw(local_win, str.c_str());
	wrefresh(local_win);
}

void GameRenderer::errPrint(const string& str) {
	WINDOW* err_win = win_mgr->getWindow(ERR_WIN);
	win_mgr->showBorder(ERR_WIN);

	wprintw(err_win, str.c_str());
	wrefresh(err_win);
}

void GameRenderer::showMenu() {
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
	win_mgr->showBorder(GAME_WIN);
	//windowPrint(GAME_WIN, string(BOARD_W * BOARD_H * 2, '.'));
}

void GameRenderer::showEndScreen() {
	windowPrint(MAIN_MENU, "Exiting the game...\n");;
}