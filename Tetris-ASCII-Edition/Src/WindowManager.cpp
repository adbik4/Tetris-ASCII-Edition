#include "WindowManager.h"
#include <thread>
#include <chrono>

void WindowManager::initTerm() {
	initscr();            // Start curses mode
	noecho();             // Don’t echo pressed keys
	cbreak();             // Disable line buffering
	keypad(stdscr, TRUE); // Enable arrow keys
	nodelay(stdscr, TRUE);// Non-blocking input
}

void WindowManager::deinitTerm() {
	this_thread::sleep_for(chrono::seconds(1));
	endwin();
}

WINDOW* WindowManager::makeMenuWindow() {
	int width, height, starty, startx, lines, cols;
	getmaxyx(stdscr, lines, cols);

	width = 20;
	height = 10;
	starty = (lines - height) / 2;	/* Calculating for a center placement */
	startx = (cols - width) / 2;	/* of the window		*/

	WINDOW* window = newwin(width, height, starty, startx);
	keypad(window, TRUE); // Enable arrow keys
	nodelay(window, TRUE);// Non-blocking input
	return window;
}

WINDOW* WindowManager::getWindow(const int& win_id) {
	switch (win_id) {
	case MAIN_MENU:
		return menu_win;
		break;
	default:
		return nullptr;
		break;
	}
}