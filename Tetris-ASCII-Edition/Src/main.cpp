#include "Settings.h"
#include "App.h"

// TODO: create a wrapper for a WINDOW* (make it a shared_ptr)
// TODO: define some sort of struct for the game-state

int main()
{
	struct GameSettings settings;
	settings.start_level = 0;

	Application app(settings);
	app.run();
}
