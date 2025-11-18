#include "InputManager.h"

#include <curses.h>
#include <iostream>

using namespace std;

// Returns user input between valid bounds from a to b
int InputManager::getIntInput(const int& min_val = INT_MIN, const int& max_val = INT_MAX) {
	int input{ 0 };
	echo();					// input echoing
	nodelay(stdscr, FALSE);	// blocking input

	while (true) {
		printw("input: ");
		refresh();

		int result = scanw("%d", &input);

		if (result == 1 && input >= min_val && input <= max_val)
			return input;

		printw("Wrong input, try again.\n");
	}

	noecho();				// redisable input echoing
	nodelay(stdscr, TRUE);	// reenable blocking input
	return input;
}