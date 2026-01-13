#pragma once

#include <memory>
#include <random>
#include <queue>
#include "Events.h"
#include "GameState.h"
#include "GameRenderer.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "Constants.h"

// TODO: create a parent class for ncurses io classes
// contains pointers to each window and getters

using namespace std;

class TimeManager;
class InputManager;
class GameRenderer;

class Mediator {
	public:
		virtual void notify(const Event& event) = 0;
		virtual ~Mediator() = default;
};

// The GameEngine is a Mediator class, which keeps track of everything that is happening in the game.
// It notifies everyone about certain events who needs to react to them and decides on the order of operations.
class GameEngine : public Mediator {
private:
	unique_ptr<GameRenderer> renderer;
	unique_ptr<TimeManager> time_mgr;
	unique_ptr<InputManager> input_mgr;

	unique_ptr<GameState> state;

	mt19937 rng;
	array<uint8_t, 35> piece_bag;
	deque<uint8_t> history;
	deque<uint8_t> drought_history;

public:
	GameEngine(const GameSettings& cfg) { 
		state = make_unique<GameState>(cfg);

		// initialise the randomness source
		rng = mt19937((uint8_t)chrono::system_clock::now().time_since_epoch().count());

		// intitialise the history
		history = { 5, 6, 5 };
		history.push_front(static_cast<uint8_t>(uniform_int_distribution<int16_t>(1, 7)(rng)));

		// initialise the piece pool
		uint8_t start_idx;
		for (auto p = 1;p <= 7; p++) {
			start_idx = (p - 1) * 5;
			fill(piece_bag.begin() + start_idx, piece_bag.begin() + start_idx + 5, p);
		}
	};

	void setTimeManager(unique_ptr<TimeManager> ptr) { time_mgr = move(ptr); }
	void setGameRenderer(unique_ptr<GameRenderer> ptr) { renderer = move(ptr); }
	void setInputManager(unique_ptr<InputManager> ptr) { input_mgr = move(ptr); }
	void notify(const Event& event) override;

	void startEngine();
	void update();
	void stopEngine();

	void gameLogic(const int& k_input);
	void menuLogic(const int& k_input);
	void settingsLogic(const int& k_input);

	void inputHandling();
	void gameOver();
	void restartGame();
	
	uint8_t TGM3_randomizer();
	uint8_t pure_randomizer();

	GameState getState() const { return *(state.get()); }
};