#pragma once

#include <iostream>

using namespace std;

class GameRenderer
{
public:
	GameRenderer() = default;
	~GameRenderer() = default;

	void showMenu();
	void showEndScreen();
};