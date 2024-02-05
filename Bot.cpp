#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <time.h>
#include "Game.h" 


using namespace std;
// ������� ���� ������ ������� � ������� ��� � ����
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
    // ������� ���� � 3 ������ �������
    for (int move = 0; move < 4; move++) {

        newCord.x = startCord.x + Field.moves[move][0];
        newCord.y = startCord.y + Field.moves[move][1];

        // ��������� �� ����� �� �� �� ������� ����
        if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
            size += shipSize(Field, newCord);
        }
    }

    return size;
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
                if (iteration >= Iteration_Limit) {
                    return false;
                }


                coordinates cord;
                // ���� ��������� ��������� � ���� ��� ������� ��������
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
                    // ���� ��������� ��������� ����� � ������ ��� ��������� ��������
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
                if (iteration >= Iteration_Limit) {
                    return false;
                }


                // ���� ��������� ���������
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
        if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
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
            if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
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
        if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
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
            if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
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
            if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
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
            if (newCord.x >= 0 && newCord.x <= X - 1 && newCord.y >= 0 && newCord.y <= Y - 1) {
                confirmShip(Field, newCord, settingType);
            }

        }

    }
}




// ������� ������� �������� �� ����
float Game::calculateEntropy(field Field) {
    // �������� ��� ������� ����
    float testEntropy = 0;

    // ���� ����� ����� ��� ���������� �������� ������ ������
    int sumTestField = 0;
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            if (Field.Field[x][y] != 0) {
                sumTestField += Field.Field[x][y];
            }
        }
    }

    // ���� ��������
    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            if (Field.Field[x][y] != 0) {
                // ������� ������ ������
                float percentTest = (1.0 * Field.Field[x][y] / sumTestField);
                testEntropy += (-1.0 * percentTest * log2(percentTest));
            }
        }
    }

    return testEntropy;
}



// ��������� ����������� �� ����
field Game::calculateChances(field Field) {
    // ���� ��� �������� ������ 
    field chancesField;


    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            chancesField.Field[x][y] = 0;
        }
    }

    // X ��� ����������� ������� 
    for (int test = 0; test < Iteration_Test; test++) {
        // ������� ����� ���� Field
        field testField = Field;

        // ����������� ������� � ��������� ������, ������� � ����� ������� ��������
        bool setingTest = setAllShips(testField, 3);

        // ���� �������� ������ ��� ����������� ��������, ��������� ��� � ����� ���� �� ����� -4
        if (!(setingTest)) {
            for (int x = 0; x < X; x++) {
                for (int y = 0; y < Y; y++) {
                    chancesField.Field[x][y] = -4;
                }
            }
            return chancesField;
        }

        //print(testField);

        //  ������� ����� (��������� ������������ ������� � ������ ������)
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



// ������� ������ ��������� ������ ���������
coordinates Game::randomCoordinates(field Field) {

    // ���� ���������
    coordinates cord;

    // ���������, ���� �� ������ ������ �� ����
    bool test = true;

    for (int x = 0; x < X; x++) {
        for (int y = 0; y < Y; y++) {
            // ���� ����� ����-�� ���� ������ �����, ��������� �������
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

        // ���������, ������ �� ���������
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




// ������� ������� ������� �������� ����� �����������
coordinates Game::ChooseBestShot_Random(field Field) {
    // ��������� ��� ������������ ��������
    coordinates BestShot;

    // ���������, ���� �� ������� �������, ���� ����, �������� ��� ������
    if (checkWoundedShips(Field).x != -1) {
        coordinates WoundedShipsCord = checkWoundedShips(Field);

        BestShot = setWoundedShip(Field, WoundedShipsCord);

    }
    else {
        // ���� ��������� ������ �����
        BestShot = randomCoordinates(Field);
        return BestShot;
    }
}



// ������� ������� ������� �������� ����� ��������
coordinates Game::ChooseBestShot_Entropy(field Field) {
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
        while (chancesField.Field[0][0] == -4) {
            // ������ ���� ���� � �������
            chancesField = calculateChances(Field);
        }

        // �������� ��� ������� ����
        float Entropy = 0;

        // ������� � ��������
        float bestTestEntropy = calculateEntropy(chancesField);


        // ������ ������� �������� ��� ������ �������� ��������
        for (int x = 0; x < X; x++) {
            for (int y = 0; y < Y; y++) {
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
                    float testEntropy = calculateEntropy(chancesTestField);


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
        return BestShot;
    }
}

// ������� ������� ������� �������� ����� ������� ����������� �������� �� ������
coordinates ChooseBestShot_BorderTactic(field Field) {


}