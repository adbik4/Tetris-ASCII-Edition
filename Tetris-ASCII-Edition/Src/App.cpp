#include "App.h"

#include <memory>
#include "GameRenderer.h"
#include "TimeManager.h"
#include "GameState.h"

// Inititialises everything safely and in the right order
// the App holds a pointer to the GameEngine which owns the TimeManager
void Application::create(const struct GameSettings& cfg) {
    engine = make_unique<GameEngine>(cfg);
    auto tm = make_unique<TimeManager>(engine.get());
    auto gr = make_unique<GameRenderer>();
    auto im = make_unique<InputManager>();

    engine->setGameRenderer(move(gr));
    engine->setTimeManager(move(tm));
    engine->setInputManager(move(im));
}

void Application::run() {
    app_running_ = true;
    engine->startGame();

    // wait for stop flag
    while (!engine->getState().stop_flag) {
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void Application::shutdown() {
    if (!app_running_)
        return;
    app_running_ = false;
    engine->stopGame();
}