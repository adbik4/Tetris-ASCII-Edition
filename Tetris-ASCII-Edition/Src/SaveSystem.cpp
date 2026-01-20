#include "SaveSystem.h"
#include "nlohmann/json.hpp"
#include <fstream>

// Updates the save file
void saveState(const GameState& state) {
    // write to the save file
    json data;
    ofstream of("save_state.json");
    if (of.is_open()) {
        data["hi_score"] = state.cfg.hi_score;
        data["start_level"] = state.cfg.start_level;
        data["ascii_mode"] = state.cfg.ascii_mode;
        data["flash_on_clear"] = state.cfg.flash_on_clear;
        data["pure_randomness"] = state.cfg.pure_randomness;
        data["show_future"] = state.cfg.show_future;

        of << setw(4) << data << endl;
    }
    else {
        throw std::runtime_error("Couldn't open save file");
    }

    of.close();
}

// Loads the save file
GameSettings loadState() {
    GameSettings cfg;

    ifstream f("save_state.json");
    if (f.is_open()) {
        try {
            json data = json::parse(f);
            cfg = data;
        }
        catch (const json::parse_error& err) {
            string details = err.what();
            throw std::runtime_error("The save file couldn't be parsed - loading defaults:\n" + details);
        }
        catch (const json::type_error& err) {
            string details = err.what();
            throw std::runtime_error("The save file contains invalid data" + details);
        }
    }

    f.close();

    return cfg;
}