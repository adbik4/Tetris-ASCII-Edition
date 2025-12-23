#pragma once
#include "nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;

class GameSettings {
public:
    uint64_t hi_score;
    uint8_t start_level;
    bool ascii_mode;
    bool flash_on_clear;
    bool pure_randomness;

    // defaults
    GameSettings()
        : hi_score(0),
        start_level(1),
        ascii_mode(false),
        flash_on_clear(true),
        pure_randomness(false)
    {};

    GameSettings& operator=(const json& data) {
        hi_score = data["hi_score"];
        start_level = data["start_level"];
        ascii_mode = data["ascii_mode"];
        flash_on_clear = data["flash_on_clear"];
        pure_randomness = data["pure_randomness"];
        return *this;
    };
};