#ifndef SHIPS_GAME
#define SHIPS_GAME
#include <fstream>
#include <iostream>

// структера дл€ хранени€ кординат
struct coordinates {
	int x = -1;
	int y = -1;
};


// структера дл€ хранени€ полей (и количества кораблей каждого типа)
struct field {
	// поле 
	int Field[10][10];

	// количество типов кораблей
	int shipTipesCount = 4;

	// количество кораблей каждого типа 
	int ships[4] = { 4, 3, 2, 1 }; //(1-палуба, 2-палубы, 3-палубы, 4-палубы)

	// варианты движени€ {x,y} (вверх, влево, вниз, вправо)
	int moves[4][2] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };
};

//¬ывод пол€
void print(field Field, bool shipVizible);

class Game {
	// поле пользовател€
	field userField;

	// поле бота
	field botField;
	// поле дл€ расчЄтов бота (то, как бот видит поле игрока)
	field testBotsField;

	// функци€ определ€ет, есть ли вокруг €чейки корабли 
	bool isFree(field& Field, coordinates cord);

	// функци€ определ€ет, есть ли вокруг клетки "раненые" клетки
	bool isKilled(field Field, coordinates xy);

	// функци€ котора€ убивает корабль
	int doKilling(field& Field, coordinates xy, int count);

	// функци€ проверки нет ли подстреленных кораблей на поле
	coordinates checkWoundedShips(field Field);

	// провер€ем заончилась ли игра (на поле не осталось клеток со значени€ми > 0)
	bool gameEnd(field Field);

	// функци€ дл€ вызстрела соперника 
	bool doShot(int playerNum, coordinates cord); // playerNum => 0 - вастрел по игроку, 1 - выстрел по боту 

	// поставить на поле все необходимые корабли
	bool setAllShips(field& Field, int settingType = 1);

	// поставить на поле все необходимые корабли (расставл€ем корабли на поле бота)
	bool setAllShipsForGoodStart(field& Field);

	// попытатьс€ поставить корабль нужного размера на нужные кординаты 
	int setShip(field& Field, coordinates cord, const int size, int alreadySet = 0, int settingType = 1);

	// удал€ем плохо поставленный корабль
	void removeShip(field& Field, coordinates cord);
	
	// перезаписываем 2 в правильно поставленном корабле на settingType
	void confirmShip(field& Field, coordinates cord, int settingType);

	// ищем кординату, чтобы добить подбитый корабль достроить подбитый корабль
	coordinates setWoundedShip(field& Field, coordinates startCord);

	// функци€ расчета лучшего выстрела
	coordinates ChooseBestShot(field Field);

	// посчитать веро€тности на поле
	field calculateChances(field Field);

public:
	// создание игры (создаем и заполн€ем пол€ дл€ игроков)
	Game();

	// запуск игры
	void gameStart();
};

#endif

