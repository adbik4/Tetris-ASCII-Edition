#include "Tetromino.h"
#include "Constants.h"

const char tetrominoLUT[TETROMINO_SIZE*TETROMINO_COUNT + 1] = {
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


void Tetromino::next_piece(std::mt19937& rng) {
	std::uniform_int_distribution<uint16_t> piece_distr(1, TETROMINO_COUNT);
	curr_piece = static_cast<uint8_t>(piece_distr(rng));

	x_pos = (uint8_t)BOARD_W / 2 - 2; // center
	y_pos = 0;
}

void Tetromino::rotateL() {
	if (curr_rotation == 0) {
		curr_rotation = 3;
	}
	else {
		curr_rotation = (curr_rotation - 1) % 4;
	}
}

void Tetromino::rotateR() {
	curr_rotation = (curr_rotation + 1) % 4;
}

const char Tetromino::lookup_piece(const uint8_t& x, const uint8_t& y) {
	uint8_t idx;
	switch (curr_rotation) {
	case 0: // 0deg
		idx = TETROMINO_SIZE * (curr_piece - 1) + x + TETROMINO_W * y;
		return tetrominoLUT[idx];
	case 1: // 90deg
		idx = TETROMINO_SIZE * (curr_piece - 1) - TETROMINO_W * x + y + 12;
		return tetrominoLUT[idx];
	case 2: // 180deg
		idx = TETROMINO_SIZE * (curr_piece - 1) - x - TETROMINO_W * y + 15;
		return tetrominoLUT[idx];
	case 3: // 270deg
		idx = TETROMINO_SIZE * (curr_piece - 1) + TETROMINO_W * x - y + 3;
		return tetrominoLUT[idx];
	default:
		return 'E';	// for debug purposes
	}
}