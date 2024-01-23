#include <iostream>
#include <string>
#include "Game.h"
using namespace std;

void main() {
	setlocale(LC_ALL, "Rus");
	srand(time(NULL)); //Чтобы рандом был рандомным

	cout << "Programm Start" << "\n";

	Game a = Game();
	a.gameStart();


	cout << "Programm End" << "\n";
	

}

