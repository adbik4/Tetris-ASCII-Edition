#include "ProjectHeaders.h"
#include "GameEngine.h"
#include "TimeManager.h"

int main()
{
    shared_ptr<GameEngine> engine = make_shared<GameEngine>();
    shared_ptr<TimeManager> clock = make_shared<TimeManager>(engine);

    std::cout << "MAIN MENU\n";

    engine->setTimeManager(clock);
    engine->startGame();

    while (true) {};
}
