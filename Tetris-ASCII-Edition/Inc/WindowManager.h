#pragma once
#include <stdexcept>
#include <curses.h>

using namespace std;

#define GLOBAL		0
#define MAIN_MENU	1
#define GAME_WIN	2
#define INPUT_WIN	3
#define ERR_WIN		4
#define TITLE_WIN	5
#define STATS_WIN	6

class WindowManager {
private:
	WINDOW* menu_win;
	WINDOW* input_win;
	WINDOW* err_win;
	WINDOW* game_win;
	WINDOW* title_win;
	WINDOW* stats_win;

	void initTerm();
	void deinitTerm();

	WINDOW* makeTitleWindow();
	WINDOW* makeMenuWindow();
	WINDOW* makeInputWindow();
	WINDOW* makeErrorWindow();
	WINDOW* makeGameWindow();
	WINDOW* makeStatsWindow();

	WINDOW* createNewWindow(const int& height, const int& width, const int& starty, const int& startx);

public:
	WindowManager() {
		initTerm();
		menu_win = makeMenuWindow();
		game_win = makeGameWindow();
		input_win = makeInputWindow();
		err_win = makeErrorWindow();
		title_win = makeTitleWindow();
		stats_win = makeStatsWindow();

		if (!menu_win || !game_win || !input_win || !err_win || !title_win || !stats_win) {
			throw std::runtime_error("<WindowManager> Failed to initialise windows");
		}
	}
	~WindowManager() {
		delwin(menu_win);
		delwin(game_win);
		delwin(input_win);
		delwin(err_win);
		delwin(title_win);
		delwin(stats_win);
		menu_win = game_win = input_win = err_win = title_win = stats_win = NULL;

		deinitTerm();
	}

	void showBorder(const int& win_id);
	void clearBorder(const int& win_id);
	void clearContents(const int& win_id);
	void clearWindow(const int& win_id);
	WINDOW* getWindow(const int& win_id);
};