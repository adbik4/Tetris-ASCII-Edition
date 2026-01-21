#pragma once

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <windows.h>

	#ifdef MOUSE_MOVED
		#undef MOUSE_MOVED
	#endif
#endif

#include <curses.h>