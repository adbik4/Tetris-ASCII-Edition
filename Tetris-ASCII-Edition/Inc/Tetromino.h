#pragma once
#include <stdexcept>
#include <random>
#include "Constants.h"

class Tetromino {
private:
	uint8_t curr_piece;
	uint8_t curr_rotation;

public: 
	uint8_t x_pos;
	uint8_t y_pos;

	Tetromino() : curr_piece(NULL), curr_rotation(NULL), x_pos((uint8_t)BOARD_W/2 - 2), y_pos(0) {};

	void next_piece(std::mt19937& rng);
	void rotateR();
	void rotateL();
 
	const uint8_t get_piece_id() const { return curr_piece; }
	const uint8_t get_rotation() const { return curr_rotation; }
	const char lookup_piece(const uint8_t& x, const uint8_t& y);
};