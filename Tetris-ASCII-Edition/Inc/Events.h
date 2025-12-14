#pragma once
#include <cstdint>
#include <tuple>

using namespace std;

#define INPUT_ERR	-2
#define GENERAL_ERR	-1
#define CLK			0
#define INT_INPUT	1


typedef struct {
	int8_t id;
	tuple<int, int> args;
} Event;

/*
class Event {
public:
	int id;
	int getId() { return id; }
};

class TickEvent : public Event {};

class InputEvent : public Event {
	tuple<int, int> args;
};
*/