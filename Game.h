#ifndef SHIPS_GAME
#define SHIPS_GAME
#include <fstream>
#include <iostream>

// ��������� ��� �������� ��������
struct cordinates {
	int x = -1;
	int y = -1;
};


// ��������� ��� �������� ����� (� ���������� �������� ������� ����)
struct field {
	// ���� 
	int Field[10][10];

	// ���������� ����� ��������
	int shipTipesCount = 4;

	// ���������� �������� ������� ���� 
	int ships[4] = { 4, 3, 2, 1 }; //(1-������, 2-������, 3-������, 4-������)

	// �������� �������� {x,y} (�����, �����, ����, ������)
	int moves[4][2] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };
};

//����� ����
void print(field Field, bool shipVizible);

class Game {
	// ���� ������������
	field userField;

	// ���� ����
	field botField;
	// ���� ��� �������� ���� (��, ��� ��� ����� ���� ������)
	field testBotsField;

	// ������� ����������, ���� �� ������ ������ ������� 
	bool isFree(field& Field, cordinates cord);

	// ������� ����������, ���� �� ������ ������ "�������" ������
	bool isKilled(field Field, cordinates xy);

	// ������� ������� ������� �������
	int doKilling(field& Field, cordinates xy, int count);

	// ������� �������� ��� �� ������������� �������� �� ����
	cordinates checkWoundedShips(field Field);

	// ��������� ���������� �� ���� (�� ���� �� �������� ������ �� ���������� > 0)
	bool gameEnd(field Field);

	// ������� ��� ��������� ��������� 
	bool doShot(int playerNum, cordinates cord); // playerNum => 0 - ������� �� ������, 1 - ������� �� ���� 

	// ��������� �� ���� ��� ����������� �������
	bool setAllShips(field& Field, int settingType = 1);

	// ��������� �� ���� ��� ����������� ������� (����������� ������� �� ���� ����)
	bool setAllShipsForGoodStart(field& Field);

	// ���������� ��������� ������� ������� ������� �� ������ ��������� 
	int setShip(field& Field, cordinates cord, const int size, int alreadySet = 0, int settingType = 1);

	// ������� ����� ������������ �������
	void removeShip(field& Field, cordinates cord);
	
	// �������������� 2 � ��������� ������������ ������� �� settingType
	void confirmShip(field& Field, cordinates cord, int settingType);

	// ���� ���������, ����� ������ �������� ������� ��������� �������� �������
	cordinates setWoundedShip(field& Field, cordinates startCord);

	// ������� ������� ������� ��������
	cordinates ChooseBestShot(field Field);

	// ��������� ����������� �� ����
	field calculateChances(field Field);

public:
	// �������� ���� (������� � ��������� ���� ��� �������)
	Game();

	// ������ ����
	void gameStart();
};

#endif

