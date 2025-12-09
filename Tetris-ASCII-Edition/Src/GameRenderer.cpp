#include "GameRenderer.h"

using namespace std;

void GameRenderer::windowPrint(const int& win_id, const string& str) {
	WINDOW* local_win = win_mgr->getWindow(win_id);

	wprintw(local_win, str.c_str());
	wrefresh(local_win);
}

void GameRenderer::showMenu() {
	WINDOW* menu_win = win_mgr->getWindow(MAIN_MENU);

	mvwprintw(menu_win, 1, 2, "1) Start Game\n");
	mvwprintw(menu_win, 2, 2, "2) Settings\n");
	mvwprintw(menu_win, 3, 2, "3) Exit\n");
	mvwprintw(menu_win, 4, 2, "input: \n");

	box(menu_win, 0, 0);
	mvwprintw(menu_win, 0, 1, "MAIN MENU");
	wrefresh(menu_win);

	engine->notify(Event(INT_INPUT, { 1, 3 }));
}

void GameRenderer::showEndScreen() {
	windowPrint(MAIN_MENU, "Exiting the game...\n");;
}

void GameRenderer::clearWindow(const int& win_id) {
	WINDOW* local_win = win_mgr->getWindow(win_id);
	wclear(local_win);
	box(local_win, 0, 0);
}