#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <time.h>
#include "Game.h" 

using namespace std;
// проверка поля пользователя (есть ли файл и правильно ли он заполнен)
bool Game::readUserField(field& Field, const char* link) {
    //Задаём поле игрока из файла
    ifstream file;
    file.open(link);

    //Проверяем что файл открылся
    if (!file) {
        cout << "Не удалось открыть файл пользователя\n";
        return false;
    }
    else {
        cout << "Файл считан.\n";
        string s;

        // читаем файл построчно 
        for (int i = 0; i < 10; i++) {
            getline(file, s);

            // символы в строке
            int RightSymbols = 0;

            // идём до конца строки
            for (int j = 0; j < s.length(); j++) {
                // проверяем что символ 1 или 0 (счилаем их количество)
                if ((s[j] - '0' == 0 or s[j] - '0' == 1) and RightSymbols < 10 and i < 10) {
                    // записываем символ в поле
                    Field.Field[RightSymbols][i] = s[j] - '0';
                    RightSymbols++;
                }
            }

            if (RightSymbols == 0) {
                RightSymbols++;
            }

            // если нам не хватило символов доставим 0
            if (RightSymbols < 10 and i < 10) {
                for (int j = RightSymbols - 1; j < 10; j++) {
                    Field.Field[j][i] = 0;
                }
            }
        }

    }
    file.close(); //Закрываем файл

    // дублируем поле для проверки
    field testField = Field;

    // считаем количество кораблей на поле
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            // если нашли корабль, ищем его размер (после он будет удален)
            if (Field.Field[j][i] == 1) {
                coordinates startCord;
                startCord.x = j;
                startCord.y = i;

                int size = shipSize(testField, startCord);
                

                if (size <= 4) {
                    // уменьшаем количество найденого типа корабля
                    testField.ships[size-1]--;
                    
                }
                else {
                    cout << "слишком большой корабль \n";
                    return false;
                }
            }
        }
    }

    // проверяем количество кораблей на поле
    for (int i = 0; i < 4; i++) {
        if (testField.ships[i] != 0) {
            cout << "слишком много корабль \n";
            return false;
        }
    }

    return true;
}

Game::Game() {
    //Задаём поле игрока из файла
    // проверяем что всё работает корректно 
    if (!(readUserField(this->userField, "user.txt"))) {
        cout << " в вашем файле ошибка, попробуйте заново или воспользуйтесь полем созданным автоматически ";
        // расставляем корабли на поле 
        setAllShipsForGoodStart(this->userField);
    }
    print(this->userField, 0);
    system("pause");

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

    //this->gameStart();
}



bool Game::gameStart() {
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

            if (gameEnd(this->botField))
            {
                cout << "\nИгрок выиграл\n";
                return 1;
            }


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
        while (!isFirst) {
            if (gameEnd(this->userField))
            {
                cout << "\nБот выиграл\n";
                return 1;
            }

            cout << "\nВыстрел бота (Подождите несколько секунд): \n";

            coordinates BestShot = ChooseBestShot_Random(this->testBotsField);
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