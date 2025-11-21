#include "App.h"

#include <memory>
#include "GameRenderer.h"
#include "TimeManager.h"
#include "Settings.h"

// Inititialises everything safely and in the right order
// the App holds a pointer to the GameEngine which owns the TimeManager
void Application::create(const struct GameSettings& cfg) {
    engine = make_shared<GameEngine>(cfg);
    win_mgr = make_shared<WindowManager>();

    auto gr = make_unique<GameRenderer>(engine, win_mgr);
    auto im = make_unique<InputManager>(engine, win_mgr);
    auto tm = make_unique<TimeManager>(engine.get());

    engine->setGameRenderer(move(gr));
    engine->setInputManager(move(im));
    engine->setTimeManager(move(tm));
}

// Runs the application
void Application::run() {
    app_running_ = true;
    engine->startGame();

    // wait for stop flag
    while (!engine->getStatus()) {}
}

// Shuts down the application
void Application::shutdown() {
    if (!app_running_)
        return;
    app_running_ = false;
    engine->stopGame();
}