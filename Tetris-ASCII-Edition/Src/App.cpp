#include "App.h"

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

    while (!engine->getStatus()) {}; // wait for stop flag
}

void Application::shutdown() {
    if (!app_running_)
        return;
    app_running_ = false;
    engine->stopGame();
}