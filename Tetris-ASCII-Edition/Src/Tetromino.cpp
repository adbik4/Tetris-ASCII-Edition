#include "Tetromino.h"

const char tetrominoLUT[81] = {
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
		"..$."
		".$$$"
		"...."
	// id: 4
		"...."
		"..**"
		".**."
		"...."
	// id: 5
		"...."
		".&&."
		".&&."
		"...."
};

void Tetromino::next_piece() {
	uint8_t random_id = 5;
	uint8_t random_rotation = 0;

	curr_piece = random_id;
	curr_rotation = random_rotation;
	x_pos = (uint8_t)BOARD_W / 2 - 2; // center
	y_pos = 0;
}

void Tetromino::rotateL() {
	curr_rotation = (curr_rotation - 1) % 4;
}

void Tetromino::rotateR() {
	curr_rotation = (curr_rotation + 1) % 4;
}

const char Tetromino::lookup_piece(const uint8_t& x, const uint8_t& y) {
	uint8_t idx;
	switch (curr_rotation) {
	case 0:
		idx = (curr_piece - 1) * 16 + x + y * 4;
		return tetrominoLUT[idx];
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 42:
		return 'E';	// for debug purposes
	}
}