#include "InputManager.h"
#include "GameEngine.h"
#include "WindowManager.h"
#include "CursesWinCompat.h"
#include "Events.h"

using namespace std;

// Non blocking keyboard input.
// Args: none (reads from INPUT_WIN)
// Returns: int key code or -1 if window missing
int InputManager::getKeyboardInput() {
    auto win_mgr = wm.lock();
    if (!win_mgr) {
        return -1;
    }

    WINDOW* input_win = win_mgr->getWindow(INPUT_WIN);
    return wgetch(input_win);
}

// Blocking keyboard input.
// Args: none (reads from ERR_WIN)
// Returns: int key code or -1 if window missing
int InputManager::waitForAnyKey() {
    auto win_mgr = wm.lock();
    if (!win_mgr) {
        return -1;
    }

    WINDOW* local_win = win_mgr->getWindow(INPUT_WIN);
    
    nodelay(local_win, FALSE); // Disable non-blocking input
    int input = wgetch(local_win);
    nodelay(local_win, TRUE);
    return input;
}