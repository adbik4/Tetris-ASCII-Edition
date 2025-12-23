#include <memory>
#include <iostream>
#include <stdexcept>
#include <fstream>

#include "nlohmann/json.hpp"
#include "Constants.h"
#include "GameEngine.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "GameRenderer.h"
#include "WindowManager.h"
#include <Settings.h>

using namespace std;
using json = nlohmann::json;

// TODO: create a wrapper for a WINDOW* (make it a shared_ptr)

int main()
{
    // load the save file
    GameSettings cfg;
    ifstream f("save_state.json");
    if (f.is_open()) {
        try {
            json data = json::parse(f);
            cfg = data;
        }
        catch (const json::parse_error& err) {
            cout << "The save file couldn't be parsed - loading defaults:\n\n";
            cout << err.what() << "\n\npress [enter] to continue";
            cin.get();
        }
        catch (const json::type_error& err) {
            cout << "The save file contains invalid data\n\n";
            cout << err.what() << "\n\npress [enter] to continue";
            cin.get();
        }
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

    engine->startEngine();

    // wait for stop flag
    while (!engine->getState().stop_flag) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    engine->stopEngine();
}
