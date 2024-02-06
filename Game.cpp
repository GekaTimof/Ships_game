#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <time.h>
#include "Game.h" 

using namespace std;

Game::Game() {
    //����� ������ ���� 1 ����
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->guestField.Field[i][j] = 0;
        }
    }

    // ����������� ������� �� ���� 1
    setAllShips_HalfTactic(this->guestField, 0);

    // ������� ������ �������� ���� ��� �������� 1 ����
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->testGuestField.Field[i][j] = 0;
        }
    }




    //����� ������ ���� 2 ����
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->botField.Field[i][j] = 0;
        }
    }

    // ����������� ������� �� ���� 2
    setAllShips_HalfTactic(this->botField, 3);

    // ������� ������ �������� ���� ��� �������� 2 ����
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->testBotsField.Field[i][j] = 0;
        }
    }



    cout << "---���� �����---\n";
    print(this->guestField, 0, 0);
    cout << "---���� ����---\n";
    print(this->botField, 0, 0);
    system("pause");
}



bool Game::gameStart() {
    //��� ������ ��� (1-�����, 0 - ����)
    bool isFirst = 0;

    //���� �� ����� ����
    while (!gameEnd(this->botField) && !gameEnd(this->guestField))
    {
        while (isFirst)
        {
            //������� � ���� ����, ���� ������ � �������
            if (fileout)
            {
                //fout << "---���� �����---\n";
                print(this->guestField, 0, 1);
                //fout << "---���� ����---\n";
                print(this->botField, 1, 1);
                
            }
            else
            {
                cout << "---���� �����---\n";
                print(this->guestField, 0, 0);
                cout << "---���� ����---\n";
                print(this->botField, 0, 0);
                if (withStops)
                    system("pause");
            }

            if (gameEnd(this->botField))
            {
                cout << "\n����� �������\n";
                //if (fileout)
                    //fout << "\n����� �������\n";
                return 1;
            }

            cout << "������� �����: ";
            //if (fileout)
                //fout << "������� �����: ";

            coordinates guestBestShot = ChooseBestShot_Random(this->testGuestField);
            isFirst = doShot(isFirst, guestBestShot);

            /*
            ���� ��� �����
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

            isFirst = doShot(isFirst, xy);*/

        }
        while (!isFirst)
        {
            if (gameEnd(this->guestField))
            {
                cout << "\n��� �������\n";
                //if (fileout)
                    //fout << "\n��� �������\n";
                return 1;
            }

            cout << "\n������� ���� (��������� ��������� ������): \n";
            //if (fileout)
                //fout << "\n������� ���� (��������� ��������� ������): \n";

            coordinates BestShot = ChooseBestShot_Entropy(this->testBotsField);
            isFirst = doShot(isFirst, BestShot);
        }

    }
    //if (withStops)
        //system("cls");

    if (gameEnd(this->botField))
    {
        cout << "�� ��������� :(";
        //if (fileout)
            //fout << "�� ��������� :(";
    }
    else
    {
        cout << "�� ��������! :)";
        //if (fileout)
            //fout << "�� ��������! :)";
    }
}



bool Game::doShot(int player, coordinates xy)
{
    if (player == 1) //������� �� ����
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

        cout << "\n" << a << xy.y;
        //if (fileout)
            //fout << "\n" << a << xy.y;

        if (this->botField.Field[xy.x][xy.y] == 0)
        {
            cout << "\n����\n";
            //if (fileout)
                //fout << "\n����\n";

            testGuestField.Field[xy.x][xy.y] = -1;
            botField.Field[xy.x][xy.y] = -1;


            if (withStops)
                system("pause");

            cout << "��� ��� �������� �������� ���� �����";
            print(testGuestField, 0, 0);
            return 0;
        }
        else if (this->botField.Field[xy.x][xy.y] > 0) {
            if (isKilled(botField, xy)) {
                cout << "\n����!\n";
                //if (fileout)
                    //fout << "\n����!\n";
                int a = doKilling(botField, xy, 0);
                a = doKilling(testGuestField, xy, 0);
                this->botField.ships[a - 1]--;
                this->testGuestField.ships[a - 1]--;

                if (withStops)
                    system("pause");

                return 1;
            }
            else {
                cout << "\n�����\n";
                botField.Field[xy.x][xy.y] = -2;
                testGuestField.Field[xy.x][xy.y] = -2;

                if (withStops)
                    system("pause");
                cout << "��� ��� �������� �������� ���� �����";
                print(testGuestField, 0, 0);

                return 1;
            }
        }
    }
    if (player == 0) //������� �� �����
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

        cout << "\n" << a << xy.y;
        //if (fileout)
            //fout << "\n" << a << xy.y;

        if (this->guestField.Field[xy.x][xy.y] == 0)
        {
            cout << "\n����\n";
            //if (fileout)
                //fout << "\n����\n";

            testBotsField.Field[xy.x][xy.y] = -1;
            guestField.Field[xy.x][xy.y] = -1;


            if (withStops)
                system("pause");

            return 1;
        }
        else if (this->guestField.Field[xy.x][xy.y] > 0) {
            if (isKilled(guestField, xy)) {
                cout << "\n����!\n";
                //if (fileout)
                    //fout << "\n����!\n";
                int a = doKilling(guestField, xy, 0);
                a = doKilling(testBotsField, xy, 0);
                this->guestField.ships[a - 1]--;
                this->testBotsField.ships[a - 1]--;


                if (withStops)
                    system("pause");

                return 0;
            }
            else {
                cout << "\n�����\n";
                guestField.Field[xy.x][xy.y] = -2;
                testBotsField.Field[xy.x][xy.y] = -2;

                if (withStops)
                    system("pause");

                cout << "��� ��� �������� �������� ���� ����";
                print(testBotsField, 0, 0);

                return 0;
            }
        }
    }

}




void Game::print(field Field, bool shipVizible, bool toFile) {
    cout << "���������� �������: ";
    //if (fileout)
        //fout << "���������� �������: ";

    for (int i = 0; i < Field.shipTipesCount; i++) {
        cout << i + 1 << "-��������(" << Field.ships[i] << ") ";
        //if (fileout)
           // fout << i + 1 << "-��������(" << Field.ships[i] << ") ";
    }
    cout << "\n";
    cout << "     A B C D E F G H I J\n";
    //if (fileout)
    //{
        //fout << "\n";
        //fout << "     A B C D E F G H I J\n";
    //}
    for (int j = 0; j < 10; j++) {
        cout << "(" << j << ")  ";
        //if (fileout)
            //fout << "(" << j << ")  ";

        for (int i = 0; i < 10; i++) {
            if (Field.Field[i][j] == 0) {
                //if (fileout)
                    //fout << '-' << ' ';
                cout << '-' << ' ';
            }
            else if (shipVizible == 0 && Field.Field[i][j] == 1) {
                //if (fileout)
                    //fout << 1 << ' ';
                cout << 1 << ' ';
            }
            else if (shipVizible == 1 && Field.Field[i][j] == 1) {
                //if (fileout)
                    //fout << '-' << ' ';
                cout << '-' << ' ';
            }
            else if (Field.Field[i][j] == -1) {
                //if (fileout)
                    //fout << 0 << ' ';
                cout << 0 << ' ';
            }
            else if (Field.Field[i][j] == -2) {
                //if (fileout)
                    //fout << 'x' << ' ';
                cout << 'x' << ' ';
            }
            else if (Field.Field[i][j] == -3) {
                //if (fileout)
                    //fout << 'X' << ' ';
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