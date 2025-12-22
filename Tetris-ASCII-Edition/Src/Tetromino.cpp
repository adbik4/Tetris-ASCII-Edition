#include "Tetromino.h"
#include "Constants.h"

using namespace std;

void Tetromino::next_piece(mt19937& rng) {
	uniform_int_distribution<int16_t> piece_distr(1, TETROMINO_COUNT);
	piece_id = static_cast<int8_t>(piece_distr(rng));
	is_falling = false;
	fall_dist = 0;

	x_pos = (int8_t)BOARD_W / 2 - 2; // center
	y_pos = 0;
}

void Tetromino::rotateL(span<const char> board) {
	if (piece_id == NULL) {
		return;
	}

	int8_t prev_rotation = curr_rotation;

	curr_rotation = curr_rotation == 0 ? 3 : (--curr_rotation % 4);
	
	if (isInvalidPosition(board)) {
		curr_rotation = prev_rotation;
	}
}

void Tetromino::rotateR(span<const char> board) {
	if (piece_id == NULL) {
		return;
	}

	int8_t prev_rotation = curr_rotation;

	curr_rotation = ++curr_rotation % 4;
	if (isInvalidPosition(board)) {
		curr_rotation = prev_rotation;
	}
}

void Tetromino::moveR(span<const char> board) {
	if (piece_id == NULL) {
		return;
	}

	x_pos++;
	if (isInvalidPosition(board)) {
		x_pos--;
	}
}

void Tetromino::moveL(span<const char> board) {
	if (piece_id == NULL) {
		return;
	}

	x_pos--;
	if (isInvalidPosition(board)) {
		x_pos++;
	}
}

void Tetromino::ghost_drop(span<const char> board) {
	if (piece_id == NULL) {
		return;
	}

	while (!isInvalidPosition(board)) {
		y_pos++;
	}
	y_pos--;
}

void Tetromino::soft_drop(span<char> board) {
	if (piece_id == NULL) {
		return;
	}
	else if (is_falling) {
		fall_dist++;
	}

	y_pos++;
	if (isInvalidPosition(board)) {
		y_pos--;
		fall_dist -= (fall_dist > 0) ? 1 : 0;
		merge_piece(board);
	}
}

void Tetromino::hard_drop(span<char> board) {
	if (piece_id == NULL) {
		return;
	}

	is_falling = true;
}

// merges the tetromino with the board
void Tetromino::merge_piece(span<char> board) {
	if (isInvalidPosition(board)) {
		throw std::out_of_range("<merge_piece> Tetromino is out of bounds");
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
	piece_id = NULL;
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
	if (x < 0 || y < 0 || x >= TETROMINO_W || y >= TETROMINO_W) {
		throw std::out_of_range("<realize_piece> Tetromino tile index out of range");
	}
	if (piece_id == NULL) {
		throw std::runtime_error("<realize_piece> No tetromino is active");
	}

	uint8_t idx;
	switch (curr_rotation) {
	case 0: // 0deg
		idx = TETROMINO_SIZE * (piece_id - 1) + x + TETROMINO_W * y;
		return tetrominoLUT[idx];
	case 1: // 90deg
		idx = TETROMINO_SIZE * (piece_id - 1) - TETROMINO_W * x + y + 12;
		return tetrominoLUT[idx];
	case 2: // 180deg
		idx = TETROMINO_SIZE * (piece_id - 1) - x - TETROMINO_W * y + 15;
		return tetrominoLUT[idx];
	case 3: // 270deg
		idx = TETROMINO_SIZE * (piece_id - 1) + TETROMINO_W * x - y + 3;
		return tetrominoLUT[idx];
	default:
		return 'E';	// for debug purposes
	}
}