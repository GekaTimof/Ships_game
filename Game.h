#ifndef SHIPS_GAME
#define SHIPS_GAME
#include <fstream>
#include <string>
#include <iostream>

// размер поля
const int X = 10;
const int Y = 10;

// ограничения на число итераций для попыток поставить один корабль корабля (что-бы не попасть в цикл)
const int Iteration_Limit = 100;

// количество тестовых раскладов при расчёте энтропии
const int Iteration_Test = 500;

//вывод в файл(1) или в консоль(0)
const bool fileout = 0;

//выводить непрерывно или с остановками
const bool withStops = 0;

// структера для хранения кординат
struct coordinates {
	int x = -1;
	int y = -1;
};


// структера для хранения полей (и количества кораблей каждого типа)
struct field {
	// поле 
	int Field[X][Y];

	// количество типов кораблей
	int shipTipesCount = 4;

	// количество кораблей каждого типа 
	int ships[4] = { 4, 3, 2, 1 }; //(1-палуба, 2-палубы, 3-палубы, 4-палубы)

	// варианты движения {x,y} (вверх, влево, вниз, вправо)
	int moves[4][2] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };
};

//Вывод поля
void print(field Field, bool shipVizible);

class Game {
	// поле пользователя
	// поле гостевого бота
	field guestField;

	// поле для рассчетов гостевого бота (то, как гостевой бот видит поле встроенного бота)
	field testGuestField;

	// поле встроенного бота
	field botField;

	// поле для расчётов встроенного бота (то, как встроенный бот видит поле игрока)
	field testBotsField;



	// функция определяет, есть ли вокруг ячейки корабли 
	bool isFree(field& Field, coordinates cord);

	// функция определяет, есть ли вокруг клетки "раненые" клетки
	bool isKilled(field Field, coordinates xy);

	// функция которая убивает корабль
	int doKilling(field& Field, coordinates xy, int count);

	// функция проверки нет ли подстреленных кораблей на поле
	coordinates checkWoundedShips(field Field);

	// проверяем заончилась ли игра (на поле не осталось клеток со значениями > 0)
	bool gameEnd(field Field);

	// функция для выстрела 
	bool doShot(int playerNum, coordinates cord); // playerNum => 0 - выстрел по гостю, 1 - выстрел по боту 

	// поставить на поле все необходимые корабли
	bool setAllShips(field& Field, int settingType = 1);

	// поставить на поле все необходимые корабли (расставляем корабли на поле бота)
	bool setAllShipsForGoodStart(field& Field);

	// попытаться поставить корабль нужного размера на нужные кординаты 
	int setShip(field& Field, coordinates cord, const int size, int alreadySet = 0, int settingType = 1);

	// удаляем плохо поставленный корабль
	void removeShip(field& Field, coordinates cord);

	// перезаписываем 2 в правильно поставленном корабле на settingType
	void confirmShip(field& Field, coordinates cord, int settingType);

	// ищем кординату, чтобы добить подбитый корабль достроить подбитый корабль
	coordinates setWoundedShip(field& Field, coordinates startCord);

	// функция поиска случайной пустой кординаты
	coordinates randomCoordinates(field Field);

	// функция расчёта энтропии на поле
	float calculateEntropy(field Field);

	// посчитать вероятности на поле
	field calculateChances(field Field);

	// проверка введенного поля из файла (есть ли файл и правильно ли он заполнен)
	bool readGuestField(field& Field, const char* link);

	// функция ищет размер корабля и удаляет его с поля
	int shipSize(field& Field, coordinates startCord);

	// функция расчета лучшего выстрела через случайность
	coordinates ChooseBestShot_Random(field Field);

	// функция расчета лучшего выстрела через тактику расстановки кораблей по краям
	coordinates ChooseBestShot_BorderTactic(field Field);

	// функция расчета лучшего выстрела через энтропию
	coordinates ChooseBestShot_Entropy(field Field);

	//Вывод поля
	void print(field Field, bool shipVizible, bool toFile);

public:
	// создание игры (создаем и заполняем поля для игроков)
	Game();

	// запуск игры
	bool gameStart();
};

#endif

