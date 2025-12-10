#include <memory>
#include <iostream>
#include <stdexcept>

#include "GameState.h"
#include "GameEngine.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "GameRenderer.h"
#include "WindowManager.h"

using namespace std;

// TODO: create a wrapper for a WINDOW* (make it a shared_ptr)

int main()
{
    shared_ptr<WindowManager> win_mgr;

    struct GameSettings cfg;
	cfg.start_level = 0;

    // Inititialises everything safely and in the right order
    shared_ptr<GameEngine> engine = make_shared<GameEngine>(cfg);
    
    try {
        win_mgr = make_shared<WindowManager>();
    }
    catch (const std::exception& err) {
        cout << "[DEBUG]: " << err.what() << '\n';
    }

    auto gr = make_unique<GameRenderer>(engine, win_mgr);
    auto im = make_unique<InputManager>(engine, win_mgr);
    auto tm = make_unique<TimeManager>(engine.get());

    engine->setGameRenderer(move(gr));
    engine->setInputManager(move(im));
    engine->setTimeManager(move(tm));

    engine->startEngine();

    // wait for stop flag
    while (!engine->getState().stop_flag) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    engine->stopEngine();
}
