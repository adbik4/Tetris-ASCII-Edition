#pragma once

#include <memory>
#include "GameEngine.h"
#include "TimeManager.h"
#include "Settings.h"

class Application {
    unique_ptr<GameEngine> engine;
    bool running = false;

    public:
        Application(const struct GameSettings& settings) { create(settings); }
        ~Application() { stop(); }

        void create(const struct GameSettings& settings);
        void run();
        void stop();
};