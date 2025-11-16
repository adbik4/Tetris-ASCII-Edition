#include "App.h"

// Inititialises everything safely and in the right order
// the App holds a pointer to the GameEngine which owns the TimeManager
void Application::create(const struct GameSettings& cfg) {
    engine = make_unique<GameEngine>(cfg);
    auto tm = make_unique<TimeManager>(engine.get());

    engine->setTimeManager(move(tm));
}

void Application::run() {
    engine->startGame();
    running = true;
    cout << "MAIN MENU\n";
    cout << "Press ENTER to quit...\n";
    cin.get();

    stop();
}

void Application::stop() {
    if (!running)
        return;
    running = false;
    engine->stopGame();
}