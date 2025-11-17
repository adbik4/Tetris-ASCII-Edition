#pragma once

#include <memory>
#include "GameEngine.h"
#include "GameRenderer.h"
#include "TimeManager.h"
#include "Settings.h"

class Application {
    unique_ptr<GameEngine> engine;
    bool app_running_ = false;  // used to check if the app is running

    public:
        Application(const struct GameSettings& settings) { create(settings); }
        ~Application() { shutdown(); }

        void create(const struct GameSettings& settings);
        void run();
        void shutdown();
};