#pragma once

#include "GameEngine.h"

class Application {
    unique_ptr<GameEngine> engine;
    bool app_running_ = false;  // used to check if the app is running

    void create(const struct GameSettings& settings);
    void shutdown();

    public:
        Application(const struct GameSettings& settings) { create(settings); }
        ~Application() { shutdown(); }

        void run();
};