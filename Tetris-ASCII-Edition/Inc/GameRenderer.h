#pragma once
#include <string>

class GameRenderer
{
private:
	void initTerm();
	void deinitTerm();

public:
	GameRenderer() { initTerm(); }
	~GameRenderer() { deinitTerm(); }

	void print(const string& str);
	void showMenu();
	void showEndScreen();
};