#include "InputManager.h"

// Returns user input betwqeen valid bounds from a to b
int InputManager::getIntInput(const int& a = INT_MIN, const int& b = INT_MAX) {
	int input{ 0 };
	cout << "input: ";
	while (!(cin >> input) || input < a || input > b) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Wrong input, please try again\n";
		cout << "input: ";
	}
	return input;
}