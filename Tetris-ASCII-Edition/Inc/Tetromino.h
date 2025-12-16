#pragma once
#include <stdexcept>
#include <random>
#include <span>
#include "Constants.h"

class Tetromino {
private:
	int8_t curr_piece;
	int8_t curr_rotation;

	bool isInvalidPosition(std::span<const char> board);
	void merge_piece(std::span<char> board);

public: 
	bool is_falling;	// keeps track of the falling animation

	int8_t x_pos;
	int8_t y_pos;

	Tetromino() : is_falling(false), curr_piece(NULL), curr_rotation(NULL), x_pos((int8_t)BOARD_W/2 - 2), y_pos(0) {};

	void next_piece(std::mt19937& rng);
	void rotateR(std::span<const char> board);
	void rotateL(std::span<const char> board);
	void moveR(std::span<const char> board);
	void moveL(std::span<const char> board);
	void soft_drop(std::span<char> board);
	void hard_drop(std::span<char> board);
 
	const uint8_t get_piece_id() const { return curr_piece; }
	const uint8_t get_rotation() const { return curr_rotation; }
	const char realize_piece(const int8_t x, const int8_t y);
};