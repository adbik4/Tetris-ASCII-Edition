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

class WindowManager {
private:
	WINDOW* menu_win;
	WINDOW* input_win;
	WINDOW* err_win;
	WINDOW* game_win;
	WINDOW* title_win;

	void initTerm();
	void deinitTerm();

	WINDOW* makeMenuWindow();
	WINDOW* makeInputWindow();
	WINDOW* makeErrorWindow();
	WINDOW* makeGameWindow();
	WINDOW* makeTitleWindow();

	WINDOW* createNewWindow(const int& height, const int& width, const int& starty, const int& startx);

public:
	WindowManager() {
		initTerm();
		menu_win = makeMenuWindow();
		game_win = makeGameWindow();
		input_win = makeInputWindow();
		err_win = makeErrorWindow();
		title_win = makeTitleWindow();

		if (!menu_win || !game_win || !input_win || !err_win || !title_win) {
			throw std::runtime_error("<WindowManager> Failed to initialise windows");
		}
	}
	~WindowManager() { deinitTerm(); }

	void showBorder(const int& win_id);
	void clearBorder(const int& win_id);
	void clearContents(const int& win_id);
	void clearWindow(const int& win_id);
	WINDOW* getWindow(const int& win_id);
};