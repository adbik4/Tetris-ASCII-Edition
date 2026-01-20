#include <memory>
#include <iostream>
#include <stdexcept>

#include "Settings.h"
#include "SaveSystem.h"
#include "GameEngine.h"
#include "WindowManager.h"


using namespace std;
using json = nlohmann::json;

int main()
{
    GameSettings cfg;

    // Load the GameState from a save file
    try {
        cfg = loadState();
    }
    catch (const std::exception& err) {
        cout << err.what();
        cout << "\n\npress [enter] to continue";
        cin.get();
    }

    // Inititialise everything safely and in the right order
    shared_ptr<GameEngine> engine = make_shared<GameEngine>(cfg);

    auto win_mgr = make_shared<WindowManager>();

    auto gr = make_unique<GameRenderer>(engine, win_mgr);
    auto im = make_unique<InputManager>(engine, win_mgr);
    auto tm = make_unique<TimeManager>(engine.get());

    engine->setGameRenderer(move(gr));
    engine->setInputManager(move(im));
    engine->setTimeManager(move(tm));

    // Start the GameEngine
    engine->startEngine();

    // Wait for the stop flag
    while (!engine->getState().stop_flag) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    // Stop the GameEngine
    engine->stopEngine();

    // Write the GameState to a save file
    try {
        saveState(engine->getState());
    }
    catch (const std::exception& err) {
        cout << err.what();
        cout << "\n\npress [enter] to continue";
        cin.get();
    }
}