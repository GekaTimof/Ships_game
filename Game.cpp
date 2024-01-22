#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <time.h>
#include "Game.h" 

using namespace std;

Game::Game() {
    //����� ���� ������ �� �����
    ifstream file;
    file.open("user.txt");

    //��������� ��� ���� ��������
    if (!file) {
        cout << "�� ������� ������� ���� ������������\n";
    }
    else {
        string s;
        cout << "���� ������.\n";

        for (int i = 0; i < 10; i++) {
            getline(file, s);
            for (int j = 0; j < 10; j++) {
                if (!isspace(s[j])) {
                    //���������� � ������ ���� �� ���������� ������
                    this->userField.Field[j][i] = s[j] - '0';
                }
            }
        }
    }
    file.close(); //��������� ����


    //����� ������ ���� ����
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->botField.Field[i][j] = 0;
        }
    }

    // ����������� ������� �� ���� 
    setAllShipsForGoodStart(this->botField);


    // ������� ������ �������� ���� ��� �������� ����
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->testBotsField.Field[i][j] = 0;
        }
    }

    this->gameStart();
}



void Game::gameStart() {
    //��� ������ ��� (1-�����, 0 - ����)
    bool isFirst = 1;

    //���� �� ����� ����
    while (!gameEnd(this->botField) && !gameEnd(this->userField)) {
        while (isFirst) {
            system("cls");

            cout << "---���� ����---\n";
            print(this->userField, 0);
            cout << "---���� ����---\n"; //������ �������� ��-�������
            print(this->botField, 1);

            cout << "��� �������(NN): ";

            //����������� ���������� ������������
            coordinates xy;
            string s_xy;
            cin >> s_xy;
            while ((size(s_xy) != 2) || s_xy[1] < 48 || s_xy[1]>57 || s_xy[0] < 65 || s_xy[0] >74) {
                cout << "�������� ������ �����, ������� ���������� ��� � �������: �2\n";
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
            cout << "\n������� ���� (��������� ��������� ������): \n";

            coordinates BestShot = ChooseBestShot(this->testBotsField);
            isFirst = doShot(isFirst, BestShot);
        }
    }
    system("cls");
    if (gameEnd(this->botField))
    {
        cout << "�� ��������� :(";
    }
    else
    {
        cout << "�� ��������! :)";
    }
}



bool Game::doShot(int player, coordinates xy) {
    if (player == 1) {
        //��������, �� ������ �� ������
        if (this->botField.Field[xy.x][xy.y] < 0) {
            cout << "\n�� ��� �������� � ��� ������. �������� ������.\n";
            return 1;
        }

        if (this->botField.Field[xy.x][xy.y] == 0) {
            cout << "\n����\n";
            botField.Field[xy.x][xy.y] = -1;
            return 0;
        }
        else if (this->botField.Field[xy.x][xy.y] > 0) {
            if (isKilled(botField, xy)) {
                cout << "\n����!\n";
                int a = doKilling(botField, xy, 0);
                this->botField.ships[a - 1]--;
                return 1;
            }
            else {
                cout << "\n�����\n";
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
            cout << "\n����\n";
            testBotsField.Field[xy.x][xy.y] = -1;
            userField.Field[xy.x][xy.y] = -1;
            print(testBotsField, 1);
            system("pause");
            return 1;
        }
        else if (this->userField.Field[xy.x][xy.y] > 0) {
            if (isKilled(userField, xy)) {
                cout << "\n����!\n";
                int a = doKilling(userField, xy, 0);
                a = doKilling(testBotsField, xy, 0);
                this->userField.ships[a - 1]--;
                this->testBotsField.ships[a - 1]--;
                print(testBotsField, 1);
                system("pause");
                return 0;
            }
            else {
                cout << "\n�����\n";
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
    cout << "���������� �������: ";
    for (int i = 0; i < Field.shipTipesCount; i++) {
        cout << i + 1 << "-��������(" << Field.ships[i] << ") ";
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



// ��������� �� ���� ��� ����������� ������� (����������� ������� �� ���� ����)
bool Game::setAllShipsForGoodStart(field& Field) {
    // ����������� ������� � ��������� ������, ������� � ����� ������� ��������
    for (int type = Field.shipTipesCount - 1; type >= 0; type--) {
        // ������ �� ���� ��� ������� ������� ���� 
        for (int i = 0; i < Field.ships[type]; i++) {

            // �������, ������� ����� ���� �����������
            int togle = 0;

            // ������� ���������� �������� ��� ������ ������ 
            int iteration = 0;

            // �������� �����������, ���� �� ����� ������� ��� �������
            while (togle < type + 1) {
                // ���� ������ ������� ����� ��������, � �� ��� � �� ������ ��������� �������, ���������� false
                iteration++;
                if (iteration >= 100) {
                    return false;
                }


                coordinates cord;
                // ���� ��������� ��������� � ���� ��� ������� ��������
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
                    // ���� ��������� ��������� ����� � ������ ��� ��������� ��������
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

                //�������� ��������� ������� �� ���������
                togle = setShip(Field, cord, type + 1, 0, 1);


                // ���� �� ���������� ��������� ������ �������
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


// ��������� �� ���� ��� ����������� �������
bool Game::setAllShips(field& Field, int settingType) {
    // ����������� ������� � ��������� ������, ������� � ����� ������� ��������
    for (int type = Field.shipTipesCount - 1; type >= 0; type--) {
        // ������ �� ���� ��� ������� ������� ���� 
        for (int i = 0; i < Field.ships[type]; i++) {

            // �������, ������� ����� ���� �����������
            int togle = 0;

            // ������� ���������� �������� ��� ������ ������ 
            int iteration = 0;

            // �������� �����������, ���� �� ����� ������� ��� �������
            while (togle < type + 1) {
                // ���� ������ ������� ����� ��������, � �� ��� � �� ������ ��������� �������, ���������� false
                iteration++;
                if (iteration >= 1000) {
                    return false;
                }


                // ���� ��������� ���������
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

                //�������� ��������� ������� �� ���������
                togle = setShip(Field, cord, type + 1, 0, settingType);


                // ���� �� ���������� ��������� ������ �������
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



// ���������� ��������� ������� ������� ������� �� ������ ��������� 
int Game::setShip(field& Field, coordinates startCord, const int size, int alreadySet, int settingType) {
    // ���� ��� ������ ���������� 
    if (size == alreadySet) {
        return alreadySet;
    }

    // ��������� ��������� ��������� 
    if (Field.Field[startCord.x][startCord.y] == 0 && isFree(Field, startCord)) {

        // ������ ������ �� ����
        Field.Field[startCord.x][startCord.y] = 2;
        alreadySet++;


        // �������� ��������� ��� (�����, �����, ����, ������)
        int move;
        move = rand() % 4;
        //cout << startCord.x << " - " << startCord.y << " " << move << "\n";

        coordinates newCord;
        newCord.x = startCord.x + Field.moves[move][0];
        newCord.y = startCord.y + Field.moves[move][1];

        // ��������� �� ����� �� �� �� ������� ����
        if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
            alreadySet = setShip(Field, newCord, size, alreadySet, settingType);
        }

        // ���� ��� ������ ���������� 
        if (size == alreadySet) {
            return alreadySet;
        }


        // ������� ���� � 3 ������ �������
        for (int x = 0; x < 3; x++) {
            // ���� ������ ��������� ���
            move = (move + 1) % 4;

            newCord.x = startCord.x + Field.moves[move][0];
            newCord.y = startCord.y + Field.moves[move][1];

            // ��������� �� ����� �� �� �� ������� ����
            if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
                alreadySet = setShip(Field, newCord, size, alreadySet, settingType);
            }

            // ���� ��� ������ ���������� 
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



// ���� ���������, ����� ������ �������� ������� ��������� �������� �������
coordinates Game::setWoundedShip(field& Field, coordinates startCord) {
    coordinates emptyCoers;

    // ��������� ��������� ��������� 
    if (Field.Field[startCord.x][startCord.y] == -2) {
        // ����������� ������ 2, ����� �� ��������� ����������� ����
        Field.Field[startCord.x][startCord.y] = 2;


        // �������� ��������� ��� (�����, �����, ����, ������)
        int move;
        move = rand() % 4;

        // ��������� ��� ����
        coordinates newCord;
        newCord.x = startCord.x + Field.moves[move][0];
        newCord.y = startCord.y + Field.moves[move][1];

        // ��������� �� ����� �� �� �� ������� ����
        if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
            // ��� � �������� ���������
            coordinates testCoed = setWoundedShip(Field, newCord);

            // ���������, ����� ������ ��������� (���� �� ������, ��� �����, ������� �� ����������)
            if (testCoed.x != -1) {
                return testCoed;
            }
        }

        // �������� ������ 3 ���� (�����, ����, �����, ������)
        for (int x = 0; x < 3; x++) {
            // ���� ������ ��������� ���
            move = (move + 1) % 4;

            newCord.x = startCord.x + Field.moves[move][0];
            newCord.y = startCord.y + Field.moves[move][1];

            // ��������� �� ����� �� �� �� ������� ����
            if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
                // ��� � �������� ���������
                coordinates testCoed = setWoundedShip(Field, newCord);

                // ���������, ����� ������ ��������� (���� �� ������, ��� �����, ������� �� ����������)
                if (testCoed.x != -1) {
                    return testCoed;
                }
            }
        }
    }
    // ���� ������ ��������, ���������� � ���������
    else if (Field.Field[startCord.x][startCord.y] == 0 && isFree(Field, startCord)) {
        return startCord;
    }

    return emptyCoers;
}



// ������� ����� ������������ �������
void Game::removeShip(field& Field, coordinates cord) {
    // ���� ������ �������� ����� ��������� ������������� �������
    if (Field.Field[cord.x][cord.y] == 2) {
        Field.Field[cord.x][cord.y] = 0;

        // �������� ��� (�����, ����, �����, ������)
        for (int move = 0; move < 4; move++) {

            coordinates newCord;
            newCord.x = cord.x + Field.moves[move][0];
            newCord.y = cord.y + Field.moves[move][1];

            // ��������� �� ����� �� �� �� ������� ����
            if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
                removeShip(Field, newCord);
            }

        }

    }
}



// �������������� 2 � ��������� ������������ ������� �� settingType
void Game::confirmShip(field& Field, coordinates cord, int settingType) {
    // ���� ������ �������� ����� ��������� ������������� �������
    if (Field.Field[cord.x][cord.y] == 2) {
        Field.Field[cord.x][cord.y] = settingType;

        // �������� ��� (�����, �����, ����, ������)
        for (int move = 0; move < 4; move++) {

            coordinates newCord;
            newCord.x = cord.x + Field.moves[move][0];
            newCord.y = cord.y + Field.moves[move][1];

            // ��������� �� ����� �� �� �� ������� ����
            if (newCord.x >= 0 && newCord.x <= 9 && newCord.y >= 0 && newCord.y <= 9) {
                confirmShip(Field, newCord, settingType);
            }

        }

    }
}



// ������� ������� ������� ��������
coordinates Game::ChooseBestShot(field Field) {
    // ��������� ��� ������������ ��������
    coordinates BestShot;

    // ���������, ���� �� ������� �������, ���� ����, �������� ��� ������
    if (checkWoundedShips(Field).x != -1) {
        coordinates WoundedShipsCord = checkWoundedShips(Field);

        BestShot = setWoundedShip(Field, WoundedShipsCord);

    }
    else {
        // ���� � ���������� �������
        field chancesField = calculateChances(Field);

        // ���� �������� ������ � ����������� ���������, �������� �� ��������� ������
        while (chancesField.Field[0][0] == -4){
            // ������ ���� ���� � �������
            chancesField = calculateChances(Field);
        }

        // �������� ��� ������� ����
        float Entropy = 0;

        // ������� � ��������
        float bestTestEntropy = 0;

        // ���� ���-�� ������ ��� ��������
        int sumField = 0;
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                if (chancesField.Field[x][y] != 0) {
                    sumField += chancesField.Field[x][y];
                }
            }
        }

        // ���� ��������
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                if (chancesField.Field[x][y] != 0) {
                    // ������� ������ ������
                    float percent = (1.0 * chancesField.Field[x][y] / sumField);
                    Entropy += (-1.0 * percent * log2(percent));

                }
            }
        }

        //cout << Entropy << "\n";

        // ������ ������� �������� ��� ������ �������� ��������
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                // ������� ����� ���� Field
                field testField = Field;

                // ���� �� ����� ���������� � ��� ������
                if (testField.Field[x][y] == 0) {
                    // ������������, ��� �� ������� ������� � ������������
                    testField.Field[x][y] = -1;

                    // ������� ���� � �������
                    field chancesTestField;
                    chancesTestField = calculateChances(testField);

                    // ���� ��� ������ ������ ������ ���������� ��������, ������ ��� ������ ���� 100% � �������
                    if (chancesTestField.Field[0][0] == -4) {
                        // ���������� ����� �������, ��� ��������� �������
                        BestShot.x = x;
                        BestShot.y = y;
                        return BestShot;
                    }

                    // �������� ��� ������� ����
                    float testEntropy = 0;

                    // ���� ����� ����� ��� ���������� �������� ������ ������
                    int sumTestField = 0;
                    for (int x = 0; x < 10; x++) {
                        for (int y = 0; y < 10; y++) {
                            if (chancesTestField.Field[x][y] != 0) {
                                sumTestField += chancesTestField.Field[x][y];
                            }
                        }
                    }

                    // ���� ��������
                    for (int x = 0; x < 10; x++) {
                        for (int y = 0; y < 10; y++) {
                            if (chancesTestField.Field[x][y] != 0) {
                                // ������� ������ ������
                                float percentTest = (1.0 * chancesTestField.Field[x][y] / sumTestField);
                                testEntropy += (-1.0 * percentTest * log2(percentTest));
                            }
                        }
                    }

                    // ���� 1� ��������� �������
                    if (BestShot.x == -1) {
                        BestShot.x = x;
                        BestShot.y = y;

                        // ���������� ��������
                        bestTestEntropy = testEntropy;
                    }
                    else {
                        // ���� ���� �������, ������� ������� ��������� ��������, ����������� ���
                        if (bestTestEntropy > testEntropy) {
                            BestShot.x = x;
                            BestShot.y = y;

                            // ���������� ��������
                            bestTestEntropy = testEntropy;
                        }
                        // ���� ���� ������� � ������� ���������, �� ������� ������ ���������, ����������� ���
                        else if ((bestTestEntropy == testEntropy) && (chancesField.Field[x][y] > chancesField.Field[BestShot.x][BestShot.y])) {
                            BestShot.x = x;
                            BestShot.y = y;

                            // ���������� ��������
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



// ��������� ����������� �� ����
field Game::calculateChances(field Field) {
    // ���� ��� �������� ������ 
    field chancesField;


    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            chancesField.Field[x][y] = 0;
        }
    }

    // X ��� ����������� ������� 
    for (int test = 0; test < 500; test++) {
        // ������� ����� ���� Field
        field testField = Field;

        // ����������� ������� � ��������� ������, ������� � ����� ������� ��������
        bool setingTest = setAllShips(testField, 3);

        // ���� �������� ������ ��� ����������� ��������, ��������� ��� � ����� ���� �� ����� -4
        if (!(setingTest)) {
            for (int x = 0; x < 10; x++) {
                for (int y = 0; y < 10; y++) {
                    chancesField.Field[x][y] = -4;
                }
            }
            return chancesField;
        }

        //print(testField);

        //  ������� ����� (��������� ������������ ������� � ������ ������)
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


// ������� ����������, ���� �� ������ ������ ������� (�� ��������� ����������� �������)
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
        return 1; //���� ������ �������� ����� ���� ��� ��������� ��������
    }
    else {
        return 0;
    }
}



//�������, ���� �� ������ ��������� ������� (1 - ����, 0 - �����)
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



//������� ���� ������� ���������
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