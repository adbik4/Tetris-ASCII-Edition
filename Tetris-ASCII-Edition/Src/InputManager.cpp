#include "InputManager.h"
#include "GameEngine.h"
#include "WindowManager.h"

#include <curses.h>
#include "Events.h"

using namespace std;

// Non blocking keyboard input.
// returns an integer key_id
int InputManager::getKeyboardInput() {
    auto win_mgr = wm.lock();
    if (!win_mgr) {
        return -1;
    }

    WINDOW* input_win = win_mgr->getWindow(INPUT_WIN);
    return wgetch(input_win);
}

// Blocking keyboard input.
// returns an integer key_id
int InputManager::waitForAnyKey() {
    auto win_mgr = wm.lock();
    if (!win_mgr) {
        return -1;
    }

    WINDOW* local_win = win_mgr->getWindow(ERR_WIN);
    
    keypad(local_win, TRUE); // Enable arrow keys
    nodelay(local_win, FALSE); // Disable non-blocking input
    int input = wgetch(local_win);
    nodelay(local_win, TRUE);
    keypad(local_win, FALSE);
    return input;
}