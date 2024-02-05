#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <time.h>
#include "Game.h" 


using namespace std;
// функция ищет размер корабля и удаляет его с поля
int Game::shipSize(field& Field, coordinates startCord) {
    int size = 0;

    if (Field.Field[startCord.x][startCord.y] == 1) {
        Field.Field[startCord.x][startCord.y] = 0;
        size++;
    }
    else {
        return 0;
    }

    coordinates newCord;
    // пробуем идти в 3 другие стороны
    for (int move = 0; move < 4; move++) {

        newCord.x = startCord.x + Field.moves[move][0];
        newCord.y = startCord.y + Field.moves[move][1];

        // проверяем не вышли ли мы за пределы поля
        if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
            size += shipSize(Field, newCord);
        }
    }

    return size;
}



// поставить на поле все необходимые корабли (расставляем корабли на поле бота)
bool Game::setAllShipsForGoodStart(field& Field) {
    // расставляем корабли в случайных местах, начиная с самых больших кораблей
    for (int type = Field.shipTipesCount - 1; type >= 0; type--) {
        // ставим на поле все корабли нужного типа 
        for (int i = 0; i < Field.ships[type]; i++) {

            // счётчик, сколько палуб было поставленно
            int togle = 0;

            // счётчик количества итераций для отлова ошибки 
            int iteration = 0;

            // пытаемся подставлять, пока не найдём позицию для корабля
            while (togle < type + 1) {
                // если прошло слишком много итераций, а мы так и не смогли поставить корабль, возвращаем false
                iteration++;
                if (iteration >= Iteration_Limit) {
                    return false;
                }


                coordinates cord;
                // берём случайную кординату у края для больших кораблей
                if (type >= 1) {
                    bool XorY = rand() % 2;
                    if (XorY) {
                        cord.x = rand() % 2 * (X - 1);
                        cord.y = rand() % Y;
                    }
                    else {
                        cord.x = rand() % X;
                        cord.y = rand() % 2 * (Y - 1);
                    }
                }
                else {
                    // берём случайную кординату ближе к центру для маленьких кораблей
                    if (cord.x == -1) {
                        cord.x = rand() % (X - 2) + 1;
                        cord.y = rand() % (Y - 2) + 1;
                    }
                    else {
                        cord.x++;
                        if (cord.x > X - 1) {
                            cord.x = 0;
                            cord.y++;
                            if (cord.y > Y - 1) {
                                cord.y = 0;
                            }
                        }
                    }
                }

                //пытаемся поставить корабль на кординаты
                togle = setShip(Field, cord, type + 1, 0, 1);


                // если не получилось поставить полный корабль
                if (togle < type + 1) {
                    removeShip(Field, cord);
                }
                else {
                    confirmShip(Field, cord, 1);
                    cord.x = -1;
                }

            }
        }
    }
    return true;
}


// поставить на поле все необходимые корабли
bool Game::setAllShips(field& Field, int settingType) {
    // расставляем корабли в случайных местах, начиная с самых больших кораблей
    for (int type = Field.shipTipesCount - 1; type >= 0; type--) {
        // ставим на поле все корабли нужного типа 
        for (int i = 0; i < Field.ships[type]; i++) {

            // счётчик, сколько палуб было поставленно
            int togle = 0;

            // счётчик количества итераций для отлова ошибки 
            int iteration = 0;

            // пытаемся подставлять, пока не найдём позицию для корабля
            while (togle < type + 1) {
                // если прошло слишком много итераций, а мы так и не смогли поставить корабль, возвращаем false
                iteration++;
                if (iteration >= Iteration_Limit) {
                    return false;
                }


                // берём случайную кординату
                coordinates cord;

                if (cord.x == -1) {
                    cord.x = rand() % X;
                    cord.y = rand() % Y;
                }
                else {
                    cord.x++;
                    if (cord.x > X - 1) {
                        cord.x = 0;
                        cord.y++;
                        if (cord.y > Y - 1) {
                            cord.y = 0;
                        }
                    }
                }

                //пытаемся поставить корабль на кординаты
                togle = setShip(Field, cord, type + 1, 0, settingType);


                // если не получилось поставить полный корабль
                if (togle < type + 1) {
                    removeShip(Field, cord);
                }
                else {
                    confirmShip(Field, cord, settingType);
                    cord.x = -1;
                }

            }
        }
    }
    return true;
}



// попытаться поставить корабль нужного размера на нужные кординаты 
int Game::setShip(field& Field, coordinates startCord, const int size, int alreadySet, int settingType) {
    // если все палубы поставлены 
    if (size == alreadySet) {
        return alreadySet;
    }

    // проверяем начальную кординату 
    if (Field.Field[startCord.x][startCord.y] == 0 && isFree(Field, startCord)) {

        // ставим палубу на поле
        Field.Field[startCord.x][startCord.y] = 2;
        alreadySet++;


        // выбираем случайный шаг (вверх, влево, вниз, вправо)
        int move;
        move = rand() % 4;
        //cout << startCord.x << " - " << startCord.y << " " << move << "\n";

        coordinates newCord;
        newCord.x = startCord.x + Field.moves[move][0];
        newCord.y = startCord.y + Field.moves[move][1];

        // проверяем не вышли ли мы за пределы поля
        if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
            alreadySet = setShip(Field, newCord, size, alreadySet, settingType);
        }

        // если все палубы поставлены 
        if (size == alreadySet) {
            return alreadySet;
        }


        // пробуем идти в 3 другие стороны
        for (int x = 0; x < 3; x++) {
            // берём другой возможный шаг
            move = (move + 1) % 4;

            newCord.x = startCord.x + Field.moves[move][0];
            newCord.y = startCord.y + Field.moves[move][1];

            // проверяем не вышли ли мы за пределы поля
            if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
                alreadySet = setShip(Field, newCord, size, alreadySet, settingType);
            }

            // если все палубы поставлены 
            if (size == alreadySet) {
                return alreadySet;
            }
        }
        return alreadySet;
    }
    else {
        return alreadySet;
    }
}



// ищем кординату, чтобы добить подбитый корабль достроить подбитый корабль
coordinates Game::setWoundedShip(field& Field, coordinates startCord) {
    coordinates emptyCoers;

    // проверяем начальную кординату 
    if (Field.Field[startCord.x][startCord.y] == -2) {
        // присваемаем ячейке 2, чтобы не создавать бесконечный цикл
        Field.Field[startCord.x][startCord.y] = 2;


        // выбираем случайный шаг (вверх, влево, вниз, вправо)
        int move;
        move = rand() % 4;

        // кордината для шага
        coordinates newCord;
        newCord.x = startCord.x + Field.moves[move][0];
        newCord.y = startCord.y + Field.moves[move][1];

        // проверяем не вышли ли мы за пределы поля
        if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
            // идём в соседнюю кординату
            coordinates testCoed = setWoundedShip(Field, newCord);

            // проверяем, какая пришла кордината (если не пустая, это ответ, который мы возвращаем)
            if (testCoed.x != -1) {
                return testCoed;
            }
        }

        // выбираем другие 3 шага (вверх, вниз, влево, вправо)
        for (int x = 0; x < 3; x++) {
            // берём другой возможный шаг
            move = (move + 1) % 4;

            newCord.x = startCord.x + Field.moves[move][0];
            newCord.y = startCord.y + Field.moves[move][1];

            // проверяем не вышли ли мы за пределы поля
            if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
                // идём в соседнюю кординату
                coordinates testCoed = setWoundedShip(Field, newCord);

                // проверяем, какая пришла кордината (если не пустая, это ответ, который мы возвращаем)
                if (testCoed.x != -1) {
                    return testCoed;
                }
            }
        }
    }
    // если клетак свободна, возвращаем её кординаты
    else if (Field.Field[startCord.x][startCord.y] == 0 && isFree(Field, startCord)) {
        return startCord;
    }

    return emptyCoers;
}



// удаляем плохо поставленный корабль
void Game::removeShip(field& Field, coordinates cord) {
    // если клетка содержит часть правильно поставленного корабля
    if (Field.Field[cord.x][cord.y] == 2) {
        Field.Field[cord.x][cord.y] = 0;

        // выбираем шаг (вверх, вниз, влево, вправо)
        for (int move = 0; move < 4; move++) {

            coordinates newCord;
            newCord.x = cord.x + Field.moves[move][0];
            newCord.y = cord.y + Field.moves[move][1];

            // проверяем не вышли ли мы за пределы поля
            if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
                removeShip(Field, newCord);
            }

        }

    }
}



// перезаписываем 2 в правильно поставленном корабле на settingType
void Game::confirmShip(field& Field, coordinates cord, int settingType) {
    // если клетка содержит часть правильно поставленного корабля
    if (Field.Field[cord.x][cord.y] == 2) {
        Field.Field[cord.x][cord.y] = settingType;

        // выбираем шаг (вверх, влево, вниз, вправо)
        for (int move = 0; move < 4; move++) {

            coordinates newCord;
            newCord.x = cord.x + Field.moves[move][0];
            newCord.y = cord.y + Field.moves[move][1];

            // проверяем не вышли ли мы за пределы поля
            if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
                confirmShip(Field, newCord, settingType);
            }

        }

    }
}




// функция расчёта энтропии на поле
float Game::calculateEntropy(field Field) {
    // энтропия для данного поля
    float testEntropy = 0;

    // ищем общую сумму для нахождения процента каждой ячейки
    int sumTestField = 0;
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            if (Field.Field[x][y] != 0) {
                sumTestField += Field.Field[x][y];
            }
        }
    }

    // ищем энтропию
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            if (Field.Field[x][y] != 0) {
                // процент данной ячейки
                float percentTest = (1.0 * Field.Field[x][y] / sumTestField);
                testEntropy += (-1.0 * percentTest * log2(percentTest));
            }
        }
    }

    return testEntropy;
}



// посчитать вероятности на поле
field Game::calculateChances(field Field) {
    // поле для расчётов шансов 
    field chancesField;


    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            chancesField.Field[x][y] = 0;
        }
    }

    // X раз расставляем корабли 
    for (int test = 0; test < Iteration_Test; test++) {
        // создаем копию поля Field
        field testField = Field;

        // расставляем корабли в случайных местах, начиная с самых больших кораблей
        bool setingTest = setAllShips(testField, 3);

        // еслт возникла ошибка при расстановек кораблей, завершаем код и выдаём поле из меток -4
        if (!(setingTest)) {
            for (int x = 0; x < X; x++) {
                for (int y = 0; y < Y; y++) {
                    chancesField.Field[x][y] = -4;
                }
            }
            return chancesField;
        }

        //print(testField);

        //  считаем шансы (возможное расположение корабля в каждой ячейке)
        for (int x = 0; x < X; x++) {
            for (int y = 0; y < Y; y++) {
                if (testField.Field[x][y] == 3) {
                    chancesField.Field[x][y] += 1;
                }
            }
        }
    }

    return chancesField;
}



// функция поиска случайной пустой кординаты
coordinates Game::randomCoordinates(field Field) {

    // берём кординату
    coordinates cord;

    // проверяем, есть ли пустые клетки на поле
    bool test = true;

    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            // если нашли хотя-бы одну пустую точку, завершаем провеку
            if (Field.Field[x][y] == 0) {
                test = true;
                y = Y;
                x = X;
            }
        }
    }

    if (test) {
        cord.x = rand() % X;
        cord.y = rand() % Y;

        // проверяем, пустая ли кордината
        while (Field.Field[cord.x][cord.y] != 0) {
            cord.x = rand() % X;
            cord.y = rand() % Y;
        }
        return cord;
    }
    else {
        return cord;
    }

}




// функция расчета лучшего выстрела через случайность
coordinates Game::ChooseBestShot_Random(field Field) {
    // кординаты для оптимального выстрела
    coordinates BestShot;

    // проверяем, есть ли раненые корабли, если есть, пытаемся его добить
    if (checkWoundedShips(Field).x != -1) {
        coordinates WoundedShipsCord = checkWoundedShips(Field);

        BestShot = setWoundedShip(Field, WoundedShipsCord);

    }
    else {
        // берём случайную пустую ячеёку
        BestShot = randomCoordinates(Field);
        return BestShot;
    }
}



// функция расчета лучшего выстрела через энтропию
coordinates Game::ChooseBestShot_Entropy(field Field) {
    // кординаты для оптимального выстрела
    coordinates BestShot;

    // проверяем, есть ли раненые корабли, если есть, пытаемся его добить
    if (checkWoundedShips(Field).x != -1) {
        coordinates WoundedShipsCord = checkWoundedShips(Field);

        BestShot = setWoundedShip(Field, WoundedShipsCord);

    }
    else {
        // поле с найденными шансами
        field chancesField = calculateChances(Field);

        // если возникла ошибка с растановеой корбаблей, тытаемся их поставить заново
        while (chancesField.Field[0][0] == -4) {
            // заново ищем поле с шансами
            chancesField = calculateChances(Field);
        }

        // энтропия для данного поля
        float Entropy = 0;

        // разница в энтропии
        float bestTestEntropy = calculateEntropy(chancesField);


        // теперь считаем энтропию для кадого варианта выстрела
        for (int x = 0; x < X; x++) {
            for (int y = 0; y < Y; y++) {
                // создаем копию поля Field
                field testField = Field;

                // если мы можем выстрелить в эту клетку
                if (testField.Field[x][y] == 0) {
                    // пердпологаем, что мы сделали выстрел и промахнулись
                    testField.Field[x][y] = -1;

                    // счетаем поле с шансами
                    field chancesTestField;
                    chancesTestField = calculateChances(testField);

                    // если наш промах вызвал ошибку растановки кораблей, значит эта ячейка была 100% с кораблём
                    if (chancesTestField.Field[0][0] == -4) {
                        // возвращаем точку промаха, как идеальный выстрел
                        BestShot.x = x;
                        BestShot.y = y;
                        return BestShot;
                    }

                    // энтропия для данного поля
                    float testEntropy = calculateEntropy(chancesTestField);


                    // берём 1й возможный выстрел
                    if (BestShot.x == -1) {
                        BestShot.x = x;
                        BestShot.y = y;

                        // запоминаем энтропию
                        bestTestEntropy = testEntropy;
                    }
                    else {
                        // если есть выстрел, который сильнее уменьшает энтропию, запоаминаем его
                        if (bestTestEntropy > testEntropy) {
                            BestShot.x = x;
                            BestShot.y = y;

                            // запоминаем энтропию
                            bestTestEntropy = testEntropy;
                        }
                        // если есть выстрел с такойже энтропией, но большим шансом попадания, запоаминаем его
                        else if ((bestTestEntropy == testEntropy) && (chancesField.Field[x][y] > chancesField.Field[BestShot.x][BestShot.y])) {
                            BestShot.x = x;
                            BestShot.y = y;

                            // запоминаем энтропию
                            bestTestEntropy = testEntropy;
                        }
                    }
                }
            }
        }
        return BestShot;
    }
}

// функция расчета лучшего выстрела через тактику расстановки кораблей по крайам
coordinates ChooseBestShot_BorderTactic(field Field) {


}