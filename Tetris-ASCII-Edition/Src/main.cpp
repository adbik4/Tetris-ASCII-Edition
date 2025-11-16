#include "Settings.h"
#include "App.h"

int main()
{
	struct GameSettings settings;
	settings.start_level = 0;

	Application app(settings);
	app.run();
}
