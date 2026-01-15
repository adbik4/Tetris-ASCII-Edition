#include "WindowManager.h"
#include "locale"

void WindowManager::initTerm() {
	setlocale(LC_ALL, "");

	initscr();            // Start curses mode
	noecho();             // Don't echo pressed keys
	cbreak();             // Disable line buffering
	curs_set(0);		  // invisible cursor

	start_color();
	use_default_colors(); // enable color usage

	// Define color pairs (the indexes correspond to tetromino id's)
	init_pair(0, COLOR_BLACK, COLOR_BLACK);
	init_pair(1, COLOR_BLACK, COLOR_CYAN);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_BLACK, COLOR_BLUE);
	init_pair(4, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(5, COLOR_BLACK, COLOR_GREEN);
	init_pair(6, COLOR_BLACK, COLOR_RED);
	init_pair(7, COLOR_BLACK, COLOR_YELLOW);
}

void WindowManager::deinitTerm() {
	endwin();
}

void WindowManager::showBorder(const int& win_id) {
	WINDOW* local_win = getWindow(win_id);
	WINDOW* tmp = createNewWindow(local_win->_maxy + 2, local_win->_maxx + 2, local_win->_begy - 1, local_win->_begx - 1);
	box(tmp, 0, 0);
	wrefresh(tmp);
	delwin(tmp);
}

void WindowManager::clearBorder(const int& win_id) {
	WINDOW* local_win = getWindow(win_id);
	WINDOW* tmp = createNewWindow(local_win->_maxy + 2, local_win->_maxx + 2, local_win->_begy - 1, local_win->_begx - 1);
	wborder(tmp, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(tmp);
	delwin(tmp);
}

void WindowManager::clearContents(const int& win_id) {
	WINDOW* local_win = getWindow(win_id);
	wclear(local_win);
	showBorder(win_id);
	wrefresh(local_win);
}

void WindowManager::clearWindow(const int& win_id) {
	WINDOW* local_win = getWindow(win_id);
	wclear(local_win);
	clearBorder(win_id);
}

WINDOW* WindowManager::createNewWindow(const int& height, const int& width, const int& starty, const int& startx) {
	int max_y, max_x;

	getmaxyx(stdscr, max_y, max_x);

	// Validate coordinates and size
	if (height <= 0 || width <= 0) {
		exit(-1);
	}
	if (starty < 0 || startx < 0) {
		exit(-2);
	}
	if (max_x <= startx || max_y <= starty) {
		exit(-3);
	}

	WINDOW* window;
	if ((window = newwin(height, width, starty, startx)) == nullptr) {
		exit(-4);
	}
	return window;
}

WINDOW* WindowManager::getWindow(const int& win_id) {
	switch (win_id) {
	case MENU_WIN:
		return menu_win;
		break;
	case GAME_WIN:
		return game_win;
		break;
	case INPUT_WIN:
		return input_win;
		break;
	case ERR_WIN:
		return err_win;
		break;
	case TITLE_WIN:
		return title_win;
		break;
	case STATS_WIN:
		return stats_win;
		break;
	case NEXT_WIN:
		return next_win;
		break;
	case GLOBAL:
		return stdscr;
		break;
	default:
		exit(-5); // undefined window id 
		break;
	}
}