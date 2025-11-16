#include "ProjectHeaders.h"
#include "GameEngine.h"
#include "TimeManager.h"

int main()
{
    unique_ptr<GameEngine> engine = make_unique<GameEngine>();
    unique_ptr<TimeManager> clock = make_unique<TimeManager>(engine.get());

    std::cout << "MAIN MENU\n";

    engine->setTimeManager(move(clock));
    engine->startGame();

    std::cout << "Press ENTER to stop...\n";
    std::cin.get();

    engine->stopGame();
}
