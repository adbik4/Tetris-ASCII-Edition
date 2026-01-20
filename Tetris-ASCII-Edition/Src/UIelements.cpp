#include "GameRenderer.h"
#include "SaveSystem.h"

// ==== VISUALS ====

// Refreshes the GameUI window with current info from the GameEngine
void GameRenderer::refreshGameUI() {
	auto engine = eng.lock();
	auto win_mgr = wm.lock();
	if (!engine || !win_mgr) {
		return;
	}

	WINDOW* stats_win = win_mgr->getWindow(STATS_WIN);

	uint64_t hi_score = engine->getState().cfg.hi_score;
	uint64_t score = engine->getState().score;
	uint16_t lines = engine->getState().lines;
	uint8_t level = engine->getState().level;

	wclear(stats_win);
	mvwprintw(stats_win, 0, 0, "high score: %lld", hi_score);
	mvwprintw(stats_win, 1, 0, "score: %lld", score);
	mvwprintw(stats_win, 2, 0, "lines: %ld", lines);
	mvwprintw(stats_win, 4, 12, "level: %d", level);
	wrefresh(stats_win);
}

// Refreshes the NextPieceUI window with current info from the GameEngine
void GameRenderer::refreshNextPieceUI() {
	auto engine = eng.lock();
	auto win_mgr = wm.lock();
	if (!engine || !win_mgr) {
		return;
	}

	WINDOW* next_win = win_mgr->getWindow(NEXT_WIN);
	Tetromino next_piece = engine->getState().next_piece;

	wclear(next_win);
	windowPrint(NEXT_WIN, "NEXT PIECE");
	if (engine->getState().next_piece.get_piece_id() != NULL) {
		for (uint8_t y = 0; y < TETROMINO_W; y++) {
			for (uint8_t x = 0; x < TETROMINO_W; x++) {
				render_piece(next_piece, x, y, NEXT_WIN);
			}
		}
	}
	wrefresh(next_win);
}

// Refreshes the MenuUI window with current info from the GameEngine
void GameRenderer::refreshMenuUI() {
	auto engine = eng.lock();
	auto win_mgr = wm.lock();
	if (!engine || !win_mgr) {
		return;
	}

	int h, w;
	WINDOW* menu_win = win_mgr->getWindow(MENU_WIN);
	getmaxyx(menu_win, h, w);

	for (int i = 0; i < menu_labels.size(); i++) {
		const char* label = menu_labels.at(i).c_str();
		uint8_t center = static_cast<uint8_t>((w - strlen(label)) / 2);

		if (i == engine->getState().active_label) {
			wattron(menu_win, A_REVERSE);
		}
		mvwprintw(menu_win, i, center, label);
		if (i == engine->getState().active_label) {
			wattroff(menu_win, A_REVERSE);
		}
	}

	wrefresh(menu_win);
}

// Refreshes the SettingsUI window with current info from the GameEngine
void GameRenderer::refreshSettingsUI() {
	auto engine = eng.lock();
	auto win_mgr = wm.lock();
	if (!engine || !win_mgr) {
		return;
	}

	int h, w;
	WINDOW* menu_win = win_mgr->getWindow(MENU_WIN);
	getmaxyx(menu_win, h, w);

	wclear(menu_win);

	for (int i = 0; i < settings_labels.size(); i++) {
		string label = settings_labels.at(i);
		if (label == "ASCII mode") {
			label += " : ";
			label += engine->getState().cfg.ascii_mode ? "on" : "off";
		}
		else if (label == "Flashing effects") {
			label += " : ";
			label += engine->getState().cfg.flash_on_clear ? "on" : "off";
		}
		else if (label == "Randomness") {
			label += " : ";
			label += engine->getState().cfg.pure_randomness ? "pure" : "TGM3";
		}
		else if (label == "Show future piece") {
			label += " : ";
			label += engine->getState().cfg.show_future ? "on" : "off";
		}
		else if (label == "Start level") {
			label += " : ";
			label += to_string(engine->getState().cfg.start_level);
		}

		uint8_t center = static_cast<uint8_t>((w - strlen(label.c_str())) / 2);

		if (i == engine->getState().active_label) {
			wattron(menu_win, A_REVERSE);
		}
		mvwprintw(menu_win, i, center, label.c_str());
		if (i == engine->getState().active_label) {
			wattroff(menu_win, A_REVERSE);
		}
	}

	wrefresh(menu_win);
}


// ==== LOGIC ====

// Responsible for the behavior of the main menu
void GameEngine::menuLogic(const int& k_input) {
	switch (k_input) {
	case 'w':
	case KEY_UP:
		--(state->active_label);
		if (state->active_label < 0) {
			state->active_label = static_cast<int8_t>(menu_labels.size() - 1);
		}
		break;

	case 's':
	case KEY_DOWN:
		state->active_label = static_cast<int8_t>(++(state->active_label) % menu_labels.size());
		break;

	case '\r':
	case '\n':
	case KEY_ENTER:
		if (menu_labels.at(state->active_label) == "Start Game") {
			renderer->initGameUI();
			if (state->game_over) {
				restartGame();
			}
			state->active_window = GAME;
		}
		else if (menu_labels.at(state->active_label) == "Settings") {
			state->active_label = 0;
			renderer->initSettingsUI();
			state->active_window = SETTINGS;
		}
		else if (menu_labels.at(state->active_label) == "Exit") {
			state->stop_flag = true;
		}
		break;
	case 27: // ESC
		state->stop_flag = true;
		break;
	}
}

// Responsible for the behavior of the settings menu
void GameEngine::settingsLogic(const int& k_input) {
	switch (k_input) {
	case 'w':
	case KEY_UP:
		--(state->active_label);
		if (state->active_label < 0) {
			state->active_label = static_cast<int8_t>(settings_labels.size() - 1);
		}
		break;

	case 's':
	case KEY_DOWN:
		state->active_label = static_cast<int8_t>(++(state->active_label) % settings_labels.size());
		break;

	case '\r':
	case '\n':
	case KEY_ENTER:
		if (settings_labels.at(state->active_label) == "Back") {
			saveState(getState());
			state->active_label = 0;
			renderer->initMenuUI();
			state->active_window = MENU;
			break;
		}

	case 'd':
	case KEY_RIGHT:
	case 'a':
	case KEY_LEFT:
		if ((k_input == 'd' || k_input == KEY_RIGHT) && settings_labels.at(state->active_label) == "Start level") {
			++(state->cfg.start_level);
			if (state->cfg.start_level > MAX_LEVEL) {
				state->cfg.start_level = MAX_LEVEL;
			}
			state->level = state->cfg.start_level;
		}
		else if ((k_input == 'a' || k_input == KEY_LEFT) && settings_labels.at(state->active_label) == "Start level") {
			if (state->cfg.start_level != 1) {
				--(state->cfg.start_level);
			}
			state->level = state->cfg.start_level;
		}
		else if (settings_labels.at(state->active_label) == "ASCII mode") {
			state->cfg.ascii_mode ^= true;	// toggle
		}
		else if (settings_labels.at(state->active_label) == "Flashing effects") {
			state->cfg.flash_on_clear ^= true;	// toggle
		}
		else if (settings_labels.at(state->active_label) == "Randomness") {
			state->cfg.pure_randomness ^= true;	// toggle
		}
		else if (settings_labels.at(state->active_label) == "Show future piece") {
			state->cfg.show_future ^= true;	// toggle
		}
		break;

	case 27: // ESC
		saveState(getState());
		state->active_label = 0;
		renderer->initMenuUI();
		state->active_window = MENU;
		break;
	}
}


// ==== INITIALISATION ====

// initialises the gameUI window
void GameRenderer::initGameUI() {
	auto win_mgr = wm.lock();
	if (!win_mgr) {
		return;
	}

	win_mgr->clearWindow(MENU_WIN);

	WINDOW* title_win = win_mgr->getWindow(TITLE_WIN);
	wclear(title_win);
	wrefresh(title_win);

	win_mgr->showBorder(GAME_WIN);
	win_mgr->showBorder(STATS_WIN);
}

// initialises the settingsUI window
void GameRenderer::initSettingsUI() {
	auto win_mgr = wm.lock();
	auto engine = eng.lock();
	if (!win_mgr || !engine) {
		return;
	}

	win_mgr->clearWindow(MENU_WIN);
}

// initialises the MenuUI window
void GameRenderer::initMenuUI() {
	auto win_mgr = wm.lock();
	auto engine = eng.lock();
	if (!win_mgr || !engine) {
		return;
	}

	win_mgr->clearWindow(MENU_WIN);
	if (engine->getState().game_over) {
		win_mgr->clearWindow(GAME_WIN);
		win_mgr->clearWindow(STATS_WIN);
	}
	showTitleScreen();
}


// ==== DEFINITIONS ====
WINDOW* WindowManager::makeTitleWindow() {
	int width, height, starty, startx, lines, cols, voffset;
	getmaxyx(stdscr, lines, cols);

	width = 105; //105;
	height = 15;
	voffset = -3;

	starty = voffset + (lines - height) / 2;	/* Calculating for a center placement */
	startx = (cols - width) / 2;	/* of the window		*/

	WINDOW* window = createNewWindow(height, width, starty, startx);
	keypad(window, TRUE); // Enable arrow keys
	nodelay(window, TRUE);// Non-blocking input
	return window;
}

WINDOW* WindowManager::makeMenuWindow() {
	int width, height, starty, startx, lines, cols, voffset;
	getmaxyx(stdscr, lines, cols);

	width = UI_UNIT_W * 3;
	height = 6;
	voffset = 8;

	starty = voffset + (lines - height) / 2;	/* Calculating for a center placement */
	startx = (cols - width) / 2;	/* of the window		*/

	WINDOW* window = createNewWindow(height, width, starty, startx);
	keypad(window, TRUE); // Enable arrow keys
	nodelay(window, TRUE);// Non-blocking input
	return window;
}

WINDOW* WindowManager::makeInputWindow() {
	int width, height, starty, startx, lines, cols, voffset;
	getmaxyx(stdscr, lines, cols);

	width = 1;
	height = 1;
	voffset = 13;

	starty = voffset + (lines - height) / 2;
	startx = (cols - width) / 2;

	WINDOW* window = createNewWindow(height, width, starty, startx);

	keypad(window, TRUE); // Enable arrow keys
	nodelay(window, TRUE);// Non-blocking input
	return window;
}

WINDOW* WindowManager::makeGameWindow() {
	int width, height, starty, startx, lines, cols, voffset;
	getmaxyx(stdscr, lines, cols);

	width = BOARD_W * 2;	// scale according to block size
	height = BOARD_H;
	voffset = 0;

	starty = voffset + (lines - height) / 2;
	startx = (cols - width) / 2;

	WINDOW* window = createNewWindow(height, width, starty, startx);

	keypad(window, TRUE);	// Enable arrow keys
	nodelay(window, TRUE);	// Non-blocking input

	return window;
}

WINDOW* WindowManager::makeErrorWindow() {
	int width, height, starty, startx, lines, cols;
	getmaxyx(stdscr, lines, cols);

	width = cols - 2;
	height = 1;
	starty = 1;
	startx = 1;

	WINDOW* window = createNewWindow(height, width, starty, startx);
	return window;
}

WINDOW* WindowManager::makeStatsWindow() {
	int width, height, starty, startx, lines, cols, voffset, hoffset;
	getmaxyx(stdscr, lines, cols);

	width = UI_UNIT_W * 2 + 2;
	height = 5;
	voffset = -5;
	hoffset = 26;

	starty = voffset + (lines - height) / 2;	/* Calculating for a center placement */
	startx = hoffset + (cols - width) / 2;				/* of the window		*/

	WINDOW* window = createNewWindow(height, width, starty, startx);
	return window;
}

WINDOW* WindowManager::makeNextPieceWindow() {
	int width, height, starty, startx, lines, cols, voffset, hoffset;
	getmaxyx(stdscr, lines, cols);

	width = TETROMINO_W * 2 + 2;
	height = TETROMINO_W + 1;
	voffset = 3;
	hoffset = 26;
	//voffset = -5;
	//hoffset = -20;

	starty = voffset + (lines - height) / 2;	/* Calculating for a center placement */
	startx = hoffset + (cols - width) / 2;				/* of the window		*/

	WINDOW* window = createNewWindow(height, width, starty, startx);
	return window;
}