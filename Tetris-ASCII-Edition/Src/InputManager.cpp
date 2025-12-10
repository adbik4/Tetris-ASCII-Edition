#include "InputManager.h"

#include <curses.h>
#include <iostream>
#include "Events.h"

using namespace std;

// Returns user input between valid bounds from a to b
int InputManager::getIntInput(const tuple<int, int>& bounds = {INT_MIN, INT_MAX}) {
    char buf[16];
    Event err(INPUT_ERR, { 0, 0 });

    win_mgr->clearContents(INPUT_WIN);
    win_mgr->showBorder(INPUT_WIN);
    WINDOW* input_win = win_mgr->getWindow(INPUT_WIN);

    echo();
    nodelay(input_win, FALSE);

    while (true) {
        mvwprintw(input_win, 0, 0, "input: ");
        wgetstr(input_win, buf);
        int value = atoi(buf);

        if (value >= get<0>(bounds) && value <= get<1>(bounds)) {
            noecho();
            nodelay(input_win, TRUE);
            win_mgr->clearContents(INPUT_WIN);
            win_mgr->clearBorder(INPUT_WIN);
            return value;
        }

        engine->notify(err);
        wgetch(input_win);
        win_mgr->clearContents(INPUT_WIN);
	}
}