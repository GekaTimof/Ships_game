#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <time.h>
#include "Game.h" 

using namespace std;

Game::Game() {
    //Задаём поле игрока из файла
    ifstream file;
    file.open("user.txt");

    //Проверяем что файл открылся
    if (!file) {
        cout << "Не удалось открыть файл пользователя\n";
    }
    else {
        string s;
        cout << "Файл считан.\n";

        for (int i = 0; i < 10; i++) {
            getline(file, s);
            for (int j = 0; j < 10; j++) {
                if (!isspace(s[j])) {
                    //Записываем в массив если не пробельный символ
                    this->userField.Field[j][i] = s[j] - '0';
                }
            }
        }
    }
    file.close(); //Закрываем файл


    //Задаём пустое поле бота
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->botField.Field[i][j] = 0;
        }
    }

    // расставляем корабли на поле 
    setAllShipsForGoodStart(this->botField);


    // создаем пустое тестовое поле для расчётов бота
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->testBotsField.Field[i][j] = 0;
        }
    }

    this->gameStart();
}



void Game::gameStart() {
    //Чей чейчас ход (1-юзера, 0 - бота)
    bool isFirst = 1;

    //Пока не конец игры
    while (!gameEnd(this->botField) && !gameEnd(this->userField)) {
        while (isFirst) {
            system("cls");

            cout << "---ВАШЕ ПОЛЕ---\n";
            print(this->userField, 0);
            cout << "---ПОЛЕ БОТА---\n"; //НАЧАТЬ ВЫВОДИТЬ ПО-ДРУГОМУ
            print(this->botField, 1);

            cout << "Ваш выстрел(NN): ";

            //Преобразуем координаты пользователя
            coordinates xy;
            string s_xy;
            cin >> s_xy;
            while ((size(s_xy) != 2) || s_xy[1] < 48 || s_xy[1]>57 || s_xy[0] < 65 || s_xy[0] >74) {
                cout << "Неверный формат ввода, введите координаты как в примере: А2\n";
                cin >> s_xy;
            }
            xy.y = int(s_xy[1]) - 48;
            if (s_xy[0] == 'A')
                xy.x = 0;
            else if (s_xy[0] == 'B')
                xy.x = 1;
            else if (s_xy[0] == 'C')
                xy.x = 2;
            else if (s_xy[0] == 'D')
                xy.x = 3;
            else if (s_xy[0] == 'E')
                xy.x = 4;
            else if (s_xy[0] == 'F')
                xy.x = 5;
            else if (s_xy[0] == 'G')
                xy.x = 6;
            else if (s_xy[0] == 'H')
                xy.x = 7;
            else if (s_xy[0] == 'I')
                xy.x = 8;
            else if (s_xy[0] == 'J')
                xy.x = 9;

            isFirst = doShot(isFirst, xy);
        }
        if (gameEnd(this->botField))
            break;
        while (!isFirst) {
            cout << "\nВыстрел бота (Подождите несколько секунд): \n";

            coordinates BestShot = ChooseBestShot(this->testBotsField);
            isFirst = doShot(isFirst, BestShot);
        }
    }
    system("cls");
    if (gameEnd(this->botField))
    {
        cout << "Вы проиграли :(";
    }
    else
    {
        cout << "Вы выиграли! :)";
    }
}



bool Game::doShot(int player, coordinates xy) {
    if (player == 1) {
        //Проверка, не занята ли клетка
        if (this->botField.Field[xy.x][xy.y] < 0) {
            cout << "\nВы уже стреляли в эту клетку. Выберете другую.\n";
            return 1;
        }

        if (this->botField.Field[xy.x][xy.y] == 0) {
            cout << "\nМимо\n";
            botField.Field[xy.x][xy.y] = -1;
            return 0;
        }
        else if (this->botField.Field[xy.x][xy.y] > 0) {
            if (isKilled(botField, xy)) {
                cout << "\nУбит!\n";
                int a = doKilling(botField, xy, 0);
                this->botField.ships[a - 1]--;
                return 1;
            }
            else {
                cout << "\nРанен\n";
                botField.Field[xy.x][xy.y] = -2;
                return 1;
            }
        }
    }
    if (player == 0)
    {
        char a = 'Z';
        if (xy.x == 0)
            a = 'A';
        else if (xy.x == 1)
            a = 'B';
        else if (xy.x == 2)
            a = 'C';
        else if (xy.x == 3)
            a = 'D';
        else if (xy.x == 4)
            a = 'E';
        else if (xy.x == 5)
            a = 'F';
        else if (xy.x == 6)
            a = 'G';
        else if (xy.x == 7)
            a = 'H';
        else if (xy.x == 8)
            a = 'I';
        else if (xy.x == 9)
            a = 'J';


        if (a == 'Z') {
            cout << "\n\n" << xy.x << " " << "\n\n";
        }
        cout << "\n" << a << xy.y;

        if (this->userField.Field[xy.x][xy.y] == 0) {
            cout << "\nМимо\n";
            testBotsField.Field[xy.x][xy.y] = -1;
            userField.Field[xy.x][xy.y] = -1;
            print(testBotsField, 1);
            system("pause");
            return 1;
        }
        else if (this->userField.Field[xy.x][xy.y] > 0) {
            if (isKilled(userField, xy)) {
                cout << "\nУбит!\n";
                int a = doKilling(userField, xy, 0);
                a = doKilling(testBotsField, xy, 0);
                this->userField.ships[a - 1]--;
                this->testBotsField.ships[a - 1]--;
                print(testBotsField, 1);
                system("pause");
                return 0;
            }
            else {
                cout << "\nРанен\n";
                userField.Field[xy.x][xy.y] = -2;
                testBotsField.Field[xy.x][xy.y] = -2;
                print(testBotsField, 1);
                system("pause");
                return 0;
            }
        }
    }

}




void print(field Field, bool shipVizible) {
    cout << "Оставшиеся корабли: ";
    for (int i = 0; i < Field.shipTipesCount; i++) {
        cout << i + 1 << "-палубные(" << Field.ships[i] << ") ";
    }
    cout << "\n";
    cout << "     A B C D E F G H I J\n";
    for (int j = 0; j < 10; j++) {
        cout << "(" << j << ")  ";
        for (int i = 0; i < 10; i++) {
            if (Field.Field[i][j] == 0) {
                cout << '-' << ' ';
            }
            else if (shipVizible == 0 && Field.Field[i][j] == 1) {
                cout << 1 << ' ';
            }
            else if (shipVizible == 1 && Field.Field[i][j] == 1) {
                cout << '-' << ' ';
            }
            else if (Field.Field[i][j] == -1) {
                cout << 0 << ' ';
            }
            else if (Field.Field[i][j] == -2) {
                cout << 'x' << ' ';
            }
            else if (Field.Field[i][j] == -3) {
                cout << 'X' << ' ';
            }
        }
        cout << '\n';
    }
    cout << '\n';
}


bool Game::gameEnd(field Field) {
    for (int i = 0; i < Field.shipTipesCount; i++) {
        if (Field.ships[i] > 0) {
            return 0;
        }
    }
    return 1;
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
                if (iteration >= 100) {
                    return false;
                }


                coordinates cord;
                // берём случайную кординату у края для больших кораблей
                if (type >= 1) {
                    bool XorY = rand() % 2;
                    if (XorY) {
                        cord.x = rand() % 2 * 9;
                        cord.y = rand() % 10;
                    }
                    else {
                        cord.x = rand() % 10;
                        cord.y = rand() % 2 * 9;
                    }
                }
                else {
                    // берём случайную кординату ближе к центру для маленьких кораблей
                    if (cord.x == -1) {
                        cord.x = rand() % 8 + 1;
                        cord.y = rand() % 8 + 1;
                    }
                    else {
                        cord.x++;
                        if (cord.x > 9) {
                            cord.x = 0;
                            cord.y++;
                            if (cord.y > 9) {
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
                if (iteration >= 1000) {
                    return false;
                }


                // берём случайную кординату
                coordinates cord;

                if (cord.x == -1) {
                    cord.x = rand() % 10;
                    cord.y = rand() % 10;
                }
                else {
                    cord.x++;
                    if (cord.x > 9) {
                        cord.x = 0;
                        cord.y++;
                        if (cord.y > 9) {
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
        if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
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
            if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
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
        if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
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
            if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
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
            if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
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
            if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
                confirmShip(Field, newCord, settingType);
            }

        }

    }
}



// функция расчета лучшего выстрела
coordinates Game::ChooseBestShot(field Field) {
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
        while (chancesField.Field[0][0] == -4){
            // заново ищем поле с шансами
            chancesField = calculateChances(Field);
        }

        // энтропия для данного поля
        float Entropy = 0;

        // разница в энтропии
        float bestTestEntropy = 0;

        // ищем кол-во клеток для выстрела
        int sumField = 0;
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                if (chancesField.Field[x][y] != 0) {
                    sumField += chancesField.Field[x][y];
                }
            }
        }

        // ищем энтропию
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                if (chancesField.Field[x][y] != 0) {
                    // процент данной ячейки
                    float percent = (1.0 * chancesField.Field[x][y] / sumField);
                    Entropy += (-1.0 * percent * log2(percent));

                }
            }
        }

        //cout << Entropy << "\n";

        // теперь считаем энтропию для кадого варианта выстрела
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
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
                    float testEntropy = 0;

                    // ищем общую сумму для нахождения процента каждой ячейки
                    int sumTestField = 0;
                    for (int x = 0; x < 10; x++) {
                        for (int y = 0; y < 10; y++) {
                            if (chancesTestField.Field[x][y] != 0) {
                                sumTestField += chancesTestField.Field[x][y];
                            }
                        }
                    }

                    // ищем энтропию
                    for (int x = 0; x < 10; x++) {
                        for (int y = 0; y < 10; y++) {
                            if (chancesTestField.Field[x][y] != 0) {
                                // процент данной ячейки
                                float percentTest = (1.0 * chancesTestField.Field[x][y] / sumTestField);
                                testEntropy += (-1.0 * percentTest * log2(percentTest));
                            }
                        }
                    }

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
        //cout << Entropy << "     " << bestTestEntropy << "     " << BestShot.x << " - " << BestShot.y << "\n";
        return BestShot;
    }
}



// посчитать вероятности на поле
field Game::calculateChances(field Field) {
    // поле для расчётов шансов 
    field chancesField;


    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            chancesField.Field[x][y] = 0;
        }
    }

    // X раз расставляем корабли 
    for (int test = 0; test < 500; test++) {
        // создаем копию поля Field
        field testField = Field;

        // расставляем корабли в случайных местах, начиная с самых больших кораблей
        bool setingTest = setAllShips(testField, 3);

        // еслт возникла ошибка при расстановек кораблей, завершаем код и выдаём поле из меток -4
        if (!(setingTest)) {
            for (int x = 0; x < 10; x++) {
                for (int y = 0; y < 10; y++) {
                    chancesField.Field[x][y] = -4;
                }
            }
            return chancesField;
        }

        //print(testField);

        //  считаем шансы (возможное расположение корабля в каждой ячейке)
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                if (testField.Field[x][y] == 3) {
                    chancesField.Field[x][y] += 1;
                }
            }
        }
    }

    return chancesField;
}


// функция определяет, есть ли вокруг ячейки корабли (не учитывает повреждённые корабли)
bool Game::isFree(field& Field, coordinates xy) {
    if (((xy.y == 0) || (Field.Field[xy.x][xy.y - 1] == 0) || (Field.Field[xy.x][xy.y - 1] == 2) || (Field.Field[xy.x][xy.y - 1] == -1) || (Field.Field[xy.x][xy.y - 1] == -2)) &&
        ((xy.y == 9) || (Field.Field[xy.x][xy.y + 1] == 0) || (Field.Field[xy.x][xy.y + 1] == 2) || (Field.Field[xy.x][xy.y + 1] == -1) || (Field.Field[xy.x][xy.y + 1] == -2)) &&
        ((xy.x == 0) || (Field.Field[xy.x - 1][xy.y] == 0) || (Field.Field[xy.x - 1][xy.y] == 2) || (Field.Field[xy.x - 1][xy.y] == -1) || (Field.Field[xy.x - 1][xy.y] == -2)) &&
        ((xy.x == 9) || (Field.Field[xy.x + 1][xy.y] == 0) || (Field.Field[xy.x + 1][xy.y] == 2) || (Field.Field[xy.x + 1][xy.y] == -1) || (Field.Field[xy.x + 1][xy.y] == -2)) &&
        (((xy.x == 0) || (xy.y == 0)) || (Field.Field[xy.x - 1][xy.y - 1] == 0) || (Field.Field[xy.x - 1][xy.y - 1] == 2) || (Field.Field[xy.x - 1][xy.y - 1] == -1) || (Field.Field[xy.x - 1][xy.y - 1] == -2)) &&
        (((xy.x == 9) || (xy.y == 9)) || (Field.Field[xy.x + 1][xy.y + 1] == 0) || (Field.Field[xy.x + 1][xy.y + 1] == 2) || (Field.Field[xy.x + 1][xy.y + 1] == -1) || (Field.Field[xy.x + 1][xy.y + 1] == -2)) &&
        (((xy.x == 9) || (xy.y == 0)) || (Field.Field[xy.x + 1][xy.y - 1] == 0) || (Field.Field[xy.x + 1][xy.y - 1] == 2) || (Field.Field[xy.x + 1][xy.y - 1] == -1) || (Field.Field[xy.x + 1][xy.y - 1] == -2)) &&
        (((xy.x == 0) || (xy.y == 9)) || (Field.Field[xy.x - 1][xy.y + 1] == 0) || (Field.Field[xy.x - 1][xy.y + 1] == 2) || (Field.Field[xy.x - 1][xy.y + 1] == -1) || (Field.Field[xy.x - 1][xy.y + 1] == -2)))
    {
        return 1; //если вокруг клеточки рамки поля или свободные клеточки
    }
    else {
        return 0;
    }
}



//Смотрит, убит ли данным выстрелом корабль (1 - убит, 0 - ранен)
bool Game::isKilled(field Field, coordinates xy) {
    coordinates XY;

    bool res = 1;

    Field.Field[xy.x][xy.y] = 2;


    if (xy.y != 0 && Field.Field[xy.x][xy.y - 1] == -2) {
        XY.x = xy.x;
        XY.y = xy.y - 1;
        res *= isKilled(Field, XY);
    }
    else if (xy.y != 0 && Field.Field[xy.x][xy.y - 1] == 1) {
        res *= 0;
    }


    if (xy.y != 9 && Field.Field[xy.x][xy.y + 1] == -2) {
        XY.x = xy.x;
        XY.y = xy.y + 1;
        res *= isKilled(Field, XY);
    }
    else if (xy.y != 9 && Field.Field[xy.x][xy.y + 1] == 1) {
        res *= 0;
    }


    if (xy.x != 0 && Field.Field[xy.x - 1][xy.y] == -2) {
        XY.x = xy.x - 1;
        XY.y = xy.y;
        res *= isKilled(Field, XY);
    }
    else if (xy.x != 0 && Field.Field[xy.x - 1][xy.y] == 1) {
        res *= 0;
    }

    if (xy.x != 9 && Field.Field[xy.x + 1][xy.y] == -2) {
        XY.x = xy.x + 1;
        XY.y = xy.y;
        res *= isKilled(Field, XY);
    }
    else if (xy.x != 9 && Field.Field[xy.x + 1][xy.y] == 1) {
        res *= 0;
    }

    return res;
}



//Убивает весь корабль выстрелом
int Game::doKilling(field& Field, coordinates xy, int count) {
    coordinates XY;
    Field.Field[xy.x][xy.y] = -3;
    count++;

    if (xy.y != 0 && Field.Field[xy.x][xy.y - 1] == -2) {
        XY.x = xy.x;
        XY.y = xy.y - 1;
        count = doKilling(Field, XY, count);
    }
    else if (xy.y != 0 && Field.Field[xy.x][xy.y - 1] == 0)
        Field.Field[xy.x][xy.y - 1] = -1;


    if (xy.y != 9 && Field.Field[xy.x][xy.y + 1] == -2) {
        XY.x = xy.x;
        XY.y = xy.y + 1;
        count = doKilling(Field, XY, count);
    }
    else if (xy.y != 9 && Field.Field[xy.x][xy.y + 1] == 0)
        Field.Field[xy.x][xy.y + 1] = -1;


    if (xy.x != 0 && Field.Field[xy.x - 1][xy.y] == -2) {
        XY.x = xy.x - 1;
        XY.y = xy.y;
        count = doKilling(Field, XY, count);
    }
    else if (xy.x != 0 && Field.Field[xy.x - 1][xy.y] == 0)
        Field.Field[xy.x - 1][xy.y] = -1;

    if (xy.x != 9 && Field.Field[xy.x + 1][xy.y] == -2) {
        XY.x = xy.x + 1;
        XY.y = xy.y;
        count = doKilling(Field, XY, count);
    }
    else if (xy.x != 9 && Field.Field[xy.x + 1][xy.y] == 0)
        Field.Field[xy.x + 1][xy.y] = -1;


    if ((xy.x != 0) && (xy.y != 0) && (Field.Field[xy.x - 1][xy.y - 1] == 0)){
        Field.Field[xy.x - 1][xy.y - 1] = -1;
    }

    if ((xy.x != 9) && (xy.y != 9) && (Field.Field[xy.x + 1][xy.y + 1] == 0)){
        Field.Field[xy.x + 1][xy.y + 1] = -1;
    }

    if ((xy.x != 9) && (xy.y != 0) && (Field.Field[xy.x + 1][xy.y - 1] == 0)){
        Field.Field[xy.x + 1][xy.y - 1] = -1;
    }

    if ((xy.x != 0) && (xy.y != 9) && (Field.Field[xy.x - 1][xy.y + 1] == 0)){
        Field.Field[xy.x - 1][xy.y + 1] = -1;
    }

    return count;
}



coordinates Game::checkWoundedShips(field Field) {
    coordinates XY;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (Field.Field[i][j] == -2) {
                XY.x = i;
                XY.y = j;
            }
        }
    }
    return XY;
}