#include "InputManager.h"

#include <curses.h>
#include <iostream>
#include "Events.h"

using namespace std;

// Returns user input between valid bounds from a to b
int InputManager::getIntInput(WINDOW* local_win, const tuple<int, int>& bounds = {INT_MIN, INT_MAX}) {
    char buf[16];
    echo();
    nodelay(local_win, FALSE);

    while (true) {
        wgetstr(local_win, buf);
        int value = atoi(buf);

        if (value >= get<0>(bounds) && value <= get<1>(bounds)) {
            noecho();
            nodelay(local_win, TRUE);
            return value;
        }

        engine->notify(Event(INPUT_ERR, {0, 0}));
	}
}