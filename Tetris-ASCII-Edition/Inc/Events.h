#pragma once
#include <cstdint>
#include <curses.h>
#include <tuple>

using namespace std;

#define CLK			0
#define INPUT_ERR	1
#define INT_INPUT	2

typedef struct {
	uint8_t id;
	tuple<int, int> args;
} Event;