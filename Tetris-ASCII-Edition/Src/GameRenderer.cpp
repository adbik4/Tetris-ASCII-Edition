#include "GameRenderer.h"
#include <stdexcept>

using namespace std;

void GameRenderer::windowPrint(const int& win_id, const string& str) {
	WINDOW* local_win = win_mgr->getWindow(win_id);

	wprintw(local_win, str.c_str());
	wrefresh(local_win);
}

void GameRenderer::errPrint(const string& str) {
	WINDOW* err_win = win_mgr->getWindow(ERR_WIN);
	win_mgr->showBorder(ERR_WIN);

	mvwprintw(err_win, 0, 0, str.c_str());
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
	windowPrint(GAME_WIN, "########################################################################################################################################################################################################");
}

void GameRenderer::showEndScreen() {
	windowPrint(MAIN_MENU, "Exiting the game...\n");;
}