#pragma once
#include <GameState.h>
#include "Settings.h"

void saveState(const GameState& state);
GameSettings loadState();