#include "Tetromino.h"
#include "Constants.h"

using namespace std;

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
	is_falling = false;

	x_pos = (int8_t)BOARD_W / 2 - 2; // center
	y_pos = 0;
}

void Tetromino::rotateL(span<const char> board) {
	int8_t prev_rotation = curr_rotation;

	curr_rotation = curr_rotation == 0 ? 3 : (--curr_rotation % 4);
	
	if (isInvalidPosition(board)) {
		curr_rotation = prev_rotation;
	}
}

void Tetromino::rotateR(span<const char> board) {
	int8_t prev_rotation = curr_rotation;

	curr_rotation = ++curr_rotation % 4;
	if (isInvalidPosition(board)) {
		curr_rotation = prev_rotation;
	}
}

void Tetromino::moveR(span<const char> board) {
	x_pos++;
	if (isInvalidPosition(board)) {
		x_pos--;
	}
}

void Tetromino::moveL(span<const char> board) {
	x_pos--;
	if (isInvalidPosition(board)) {
		x_pos++;
	}
}

void Tetromino::soft_drop(span<char> board) {
	y_pos++;
	if (isInvalidPosition(board)) {
		y_pos--;
		merge_piece(board);
	}
}

void Tetromino::hard_drop(span<char> board) {
	is_falling = true;
	soft_drop(board);
}

// merges the tetromino with the board
void Tetromino::merge_piece(span<char> board) {
	if (isInvalidPosition(board)) {
		return;
	}

	for (int8_t y = 0; y < TETROMINO_W; y++) {
		for (int8_t x = 0; x < TETROMINO_W; x++) {
			char tile = realize_piece(x, y);
			if (tile != '.') {
				SAMPLE_BOARD(board, x_pos + x, y_pos + y) = tile;
			}
		}
	}

	is_falling = false;
	curr_piece = NULL;
}

bool Tetromino::isInvalidPosition(span<const char> board) {
	char tile;

	for (int8_t y = 0; y < TETROMINO_W; y++) {
		for (int8_t x = 0; x < TETROMINO_W; x++) {

			tile = realize_piece(x, y);

			if (tile != '.') {
				if ((x_pos + x) < 0 || (x_pos + x) >= BOARD_W) {
					// horizontal out of bounds
					return true;
				}
				else if ((y_pos + y) < 0 || (y_pos + y) >= BOARD_H) {
					// vertical out of bounds
					return true;
				}
				else if (SAMPLE_BOARD(board, x_pos + x, y_pos + y) != '.') {
					// there's piece overlap
					return true;
				}
			}
		}
	}

	return false;
}

const char Tetromino::realize_piece(const int8_t x, const int8_t y) {
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