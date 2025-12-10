#include "WindowManager.h"
#include <thread>
#include <chrono>

void WindowManager::initTerm() {
	setlocale(LC_ALL, "");

	initscr();            // Start curses mode
	noecho();             // Don’t echo pressed keys
	cbreak();             // Disable line buffering

	start_color();
	use_default_colors(); // enable color usage

	// Define color pairs
	init_pair(1, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(2, COLOR_BLACK, COLOR_CYAN);
	
	// example
	// attron(COLOR_PAIR(0));
	// mvaddstr(y, x * 2, "  ");  // two spaces = one "pixel"
	// attroff(COLOR_PAIR(0));
}

void WindowManager::deinitTerm() {
	this_thread::sleep_for(chrono::seconds(1));
	endwin();
}

WINDOW* WindowManager::makeMenuWindow() {
	int width, height, starty, startx, lines, cols;
	getmaxyx(stdscr, lines, cols);

	width = 22;
	height = 5;
	starty = (lines - height) / 2;	/* Calculating for a center placement */
	startx = (cols - width) / 2;	/* of the window		*/

	WINDOW* window = createNewWindow(height, width, starty, startx);
	keypad(window, TRUE); // Enable arrow keys
	nodelay(window, TRUE);// Non-blocking input
	return window;
}

WINDOW* WindowManager::makeGameWindow() {
	int width, height, starty, startx, lines, cols, voffset;
	getmaxyx(stdscr, lines, cols);

	width = 10;
	height = 20;
	voffset = 0;
	starty = voffset + (lines - height) / 2;
	startx = (cols - width) / 2;

	WINDOW* window = createNewWindow(height, width, starty, startx);

	keypad(window, TRUE); // Enable arrow keys
	nodelay(window, TRUE);// Non-blocking input
	return window;
}

WINDOW* WindowManager::makeErrorWindow() {
	int width, height, starty, startx, lines, cols;
	getmaxyx(stdscr, lines, cols);

	width = 40;
	height = 5;
	starty = 0;
	startx = 0;

	WINDOW* window = createNewWindow(height, width, starty, startx);
	return window;
}

WINDOW* WindowManager::makeInputWindow() {
	int width, height, starty, startx, lines, cols, voffset;
	getmaxyx(stdscr, lines, cols);

	width = 20;
	height = 2;
	voffset = 4;
	starty = voffset + (lines - height) / 2;
	startx = (cols - width) / 2;

	WINDOW* window = createNewWindow(height, width, starty, startx);

	keypad(window, TRUE); // Enable arrow keys
	nodelay(window, TRUE);// Non-blocking input
	return window;
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
	clearContents(win_id);
	clearBorder(win_id);
}

WINDOW* WindowManager::createNewWindow(const int& height, const int& width, const int& starty, const int& startx) {
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);

	// Validate coordinates and size
	if (height <= 0 || width <= 0) {
		throw std::invalid_argument("Window height and width must be positive");
	}
	if (starty < 0 || startx < 0) {
		throw std::invalid_argument("Window coordinates must be non-negative");
	}
	if (starty + height > max_y || startx + width > max_x) {
		throw std::out_of_range("Window does not fit within the terminal");
	}

	WINDOW* window;
	if ((window = newwin(height, width, starty, startx)) == nullptr) {
		throw std::runtime_error("Failed to create a window");
	}
	return window;
}

WINDOW* WindowManager::getWindow(const int& win_id) {
	switch (win_id) {
	case MAIN_MENU:
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
	case GLOBAL:
		return stdscr;
		break;
	default:
		return nullptr;
		showBorder(ERR_WIN);
		wprintw(err_win, "[DEBUG] undefined window ID");
		wrefresh(err_win);
		break;
	}
}