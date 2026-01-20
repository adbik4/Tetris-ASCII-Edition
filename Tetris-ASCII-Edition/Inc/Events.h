#pragma once
#include <cstdint>
#include <tuple>

using namespace std;

// Can be used to pass notifications from publishers to the mediator
typedef struct {
	EventId id;
} Event;

// Defines the possible event types
enum class EventId : int8_t {
	CLK = 0,
};