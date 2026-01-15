#pragma once
#include "nlohmann/json.hpp"
#include <iostream>
#include "Constants.h"

using json = nlohmann::json;

class GameSettings {
public:
    uint64_t hi_score;      // current high score
    uint8_t start_level;    // the level at which the game will start
    bool ascii_mode;        // controls the rendering mode
    bool flash_on_clear;	// decides if the screen should flash after a line clear
    bool pure_randomness;	// decides how the pieces are randomized
    bool show_future;		// decides if future pieces are shown

    // defaults
    GameSettings()
        : hi_score(0),
        start_level(1),
        ascii_mode(false),
        flash_on_clear(true),
        pure_randomness(false),
        show_future(true)
    {
        if (hi_score > UINT64_MAX) {
            hi_score = 0;
        }
        if (start_level > MAX_LEVEL) {
            start_level = MAX_LEVEL;
        }
    };

    GameSettings& operator=(const json& data) {
        hi_score = data["hi_score"];
        start_level = data["start_level"];
        ascii_mode = data["ascii_mode"];
        flash_on_clear = data["flash_on_clear"];
        pure_randomness = data["pure_randomness"];
        show_future = data["show_future"];
        return *this;
    };
};