#pragma once
#include <stdexcept>
#include <random>
#include <span>
#include <array>
#include "Constants.h"

using namespace std;

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

	void next_piece(mt19937& rng);
	void rotateR(span<const char> board);
	void rotateL(span<const char> board);
	void moveR(span<const char> board);
	void moveL(span<const char> board);
	void soft_drop(span<char> board);
	void hard_drop(span<char> board);
 
	const uint8_t get_piece_id() const { return curr_piece; }
	const uint8_t get_rotation() const { return curr_rotation; }
	const char realize_piece(const int8_t x, const int8_t y);
	void ghost_drop (span<const char> board);

	void set_xpos(const int8_t& pos) { x_pos = pos; };
	void set_ypos(const int8_t& pos) { y_pos = pos; };
};

inline constexpr array<char, TETROMINO_SIZE * TETROMINO_COUNT + 1> tetrominoLUT {
	// id: 1
		"..@."
		"..@."
		"..@."
		"..@."
	// id: 2
		"...."
		"...#"
		".###"
		"...."
	// id: 3
		"...."
		"o..."
		"ooo."
		"...."
	// id: 4
		"...."
		"..$."
		".$$$"
		"...."
	// id: 5
		"...."
		"..**"
		".**."
		"...."
	// id: 6
		"...."
		"%%.."
		".%%."
		"...."
	// id: 7
		"...."
		".&&."
		".&&."
		"...."
};