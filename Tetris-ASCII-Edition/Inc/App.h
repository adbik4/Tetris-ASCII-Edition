#pragma once

#include "GameEngine.h"
#include "WindowManager.h"

class Application {
    shared_ptr<GameEngine> engine;
    shared_ptr<WindowManager> win_mgr;

    bool app_running_ = false;  // used to check if the app is running

    void create(const struct GameSettings& settings);
    void shutdown();

    public:
        Application(const struct GameSettings& settings) { create(settings); }
        ~Application() { shutdown(); }

        void run();
};