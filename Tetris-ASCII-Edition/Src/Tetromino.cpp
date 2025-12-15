#include "Tetromino.h"
#include "Constants.h"

using namespace std;

#define SAMPLE2D(array, x, y) ((array)[(x) + (BOARD_W) * (y)])

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


void Tetromino::next_piece(mt19937& rng) {
	uniform_int_distribution<int16_t> piece_distr(1, TETROMINO_COUNT);
	curr_piece = static_cast<int8_t>(piece_distr(rng));

	x_pos = (int8_t)BOARD_W / 2 - 2; // center
	y_pos = 0;
}

void Tetromino::rotateL(const string& board) {
	int8_t prev_rotation = curr_rotation;

	curr_rotation = curr_rotation == 0 ? 3 : (--curr_rotation % 4);
	
	if (isInvalidPosition(board)) {
		curr_rotation = prev_rotation;
	}
}

void Tetromino::rotateR(const string& board) {
	int8_t prev_rotation = curr_rotation;

	curr_rotation = ++curr_rotation % 4;
	if (isInvalidPosition(board)) {
		curr_rotation = prev_rotation;
	}
}

void Tetromino::moveR(const string& board) {
	x_pos++;
	if (isInvalidPosition(board)) {
		x_pos--;
	}
}

void Tetromino::moveL(const string& board) {
	x_pos--;
	if (isInvalidPosition(board)) {
		x_pos++;
	}
}

void Tetromino::soft_drop(const string& board) {
	y_pos++;
	if (isInvalidPosition(board)) {
		y_pos--;
		merge_piece();
	}
}

bool Tetromino::isInvalidPosition(const string& board) {
	char tile;

	for (int8_t y = 0; y < TETROMINO_W; y++) {
		for (int8_t x = 0; x < TETROMINO_W; x++) {
			tile = transform_piece(x, y);
			if (tile != '.') {
				if ((x_pos + x) < 0 || (x_pos + x) > (BOARD_W - 1)) {
					// horizontal out of bounds
					return true;
				}
				else if ((y_pos + y) < 0 || (y_pos + y) > (BOARD_H - 1)) {
					// vertical out of bounds
					return true;
				}
				else if (SAMPLE2D(board, x_pos + x, y_pos + y) != '.') {
					// there's piece overlap
					return true;
				}
			}
		}
	}

	return false;
}

const char Tetromino::transform_piece(const int8_t& x, const int8_t& y) {
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

void Tetromino::merge_piece() {};