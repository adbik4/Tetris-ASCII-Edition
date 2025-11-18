#include "GameRenderer.h"

#include <curses.h>
#include <thread>
#include <chrono>

using namespace std;

void GameRenderer::initTerm() {
	initscr();            // Start curses mode
	noecho();             // Don’t echo pressed keys
	cbreak();             // Disable line buffering
	keypad(stdscr, TRUE); // Enable arrow keys
	nodelay(stdscr, TRUE);// Non-blocking input
}

void GameRenderer::deinitTerm() {
	this_thread::sleep_for(chrono::seconds(1));
	endwin();
}

void GameRenderer::print(const string& str) {
	printw(str.c_str());
	refresh();
}

void GameRenderer::showMenu() {
	printw("MAIN MENU:\n");
	printw("1) Start Game\n");
	printw("2) Settings\n");
	printw("3) Exit\n");
	refresh();
}

void GameRenderer::showEndScreen() {
	print("Exiting the game...\n");
}