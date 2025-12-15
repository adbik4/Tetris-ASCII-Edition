#pragma once
#include <stdexcept>
#include <random>
#include "Constants.h"

class Tetromino {
private:
	int8_t curr_piece;
	int8_t curr_rotation;

	bool isInvalidPosition(const std::string& board);
	void merge_piece();

public: 
	int8_t x_pos;
	int8_t y_pos;

	Tetromino() : curr_piece(NULL), curr_rotation(NULL), x_pos((int8_t)BOARD_W/2 - 2), y_pos(0) {};

	void next_piece(std::mt19937& rng);
	void rotateR(const std::string& board);
	void rotateL(const std::string& board);
	void moveR(const std::string& board);
	void moveL(const std::string& board);
	void soft_drop(const std::string& board);
	void hard_drop(const std::string& board);
 
	const uint8_t get_piece_id() const { return curr_piece; }
	const uint8_t get_rotation() const { return curr_rotation; }
	const char transform_piece(const int8_t& x, const int8_t& y);
};