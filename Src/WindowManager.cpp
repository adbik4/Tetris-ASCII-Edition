#include "WindowManager.h"
#include "locale"

// Initialises the terminal.
// Runs when the WindowManager is created
// Args: none
// Returns: void
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

// Deinitialises the terminal.
// Runs when the WindowManager is deleted
// Args: none
// Returns: void
void WindowManager::deinitTerm() {
	endwin();
}

// Shows the given window border without clearing its contents
// Args: win_id - const int& window identifier
// Returns: void
void WindowManager::showBorder(const int& win_id) {
	WINDOW* local_win = getWindow(win_id);
	WINDOW* tmp = createNewWindow(local_win->_maxy + 2, local_win->_maxx + 2, local_win->_begy - 1, local_win->_begx - 1);
	box(tmp, 0, 0);
	wrefresh(tmp);
	delwin(tmp);
}

// Clears the given window border without clearing its contents
// Args: win_id - const int& window identifier
// Returns: void
void WindowManager::clearBorder(const int& win_id) {
	WINDOW* local_win = getWindow(win_id);
	WINDOW* tmp = createNewWindow(local_win->_maxy + 2, local_win->_maxx + 2, local_win->_begy - 1, local_win->_begx - 1);
	wborder(tmp, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(tmp);
	delwin(tmp);
}

// Clears the given window contents without clearing its border
// Args: win_id - const int& window identifier
// Returns: void
void WindowManager::clearContents(const int& win_id) {
	WINDOW* local_win = getWindow(win_id);
	wclear(local_win);
	showBorder(win_id);
	wrefresh(local_win);
}

// Clears the given window border and contents
// Args: win_id - const int& window identifier
// Returns: void
void WindowManager::clearWindow(const int& win_id) {
	WINDOW* local_win = getWindow(win_id);
	wclear(local_win);
	clearBorder(win_id);
}

// Properly creates and validates a new WINDOW object
// Args: height - const int& rows; width - const int& cols; starty - const int& y origin; startx - const int& x origin
// Returns: WINDOW* newly created window (exits on failure)
WINDOW* WindowManager::createNewWindow(const int& height, const int& width, const int& starty, const int& startx) {
	int max_y, max_x;

	getmaxyx(stdscr, max_y, max_x);

	// Validate coordinates and size
	if (height <= 0 || width <= 0) {
		exit(-1);	// invalid size
	}
	if (starty < 0 || startx < 0) {
		exit(-2);	// start coordinate too small
	}
	if (max_x <= startx || max_y <= starty) {
		exit(-3);	// start coordinate too large
	}

	WINDOW* window;
	if ((window = newwin(height, width, starty, startx)) == nullptr) {
		exit(-4);	// error during window creation
	}
	return window;
}

// Returns a pointer to a given WINDOW
// Args: win_id - const int& window identifier
// Returns: WINDOW* corresponding to requested id (exits on invalid id)
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