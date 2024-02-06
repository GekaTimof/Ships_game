#ifndef SHIPS_GAME
#define SHIPS_GAME
#include <fstream>
#include <string>
#include <iostream>

// ������ ����
const int X = 10;
const int Y = 10;

// ����������� �� ����� �������� ��� ������� ��������� ���� ������� ������� (���-�� �� ������� � ����)
const int Iteration_Limit = 100;

// ���������� �������� ��������� ��� ������� ��������
const int Iteration_Test = 500;

//����� � ����(1) ��� � �������(0)
const bool fileout = 0;

//�������� ���������� ��� � �����������
const bool withStops = 0;

// ��������� ��� �������� ��������
struct coordinates {
	int x = -1;
	int y = -1;
};


// ��������� ��� �������� ����� (� ���������� �������� ������� ����)
struct field {
	// ���� 
	int Field[X][Y];

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
	// ���� ��������� ����
	field guestField;

	// ���� ��� ��������� ��������� ���� (��, ��� �������� ��� ����� ���� ����������� ����)
	field testGuestField;

	// ���� ����������� ����
	field botField;

	// ���� ��� �������� ����������� ���� (��, ��� ���������� ��� ����� ���� ������)
	field testBotsField;



	// ������� ����������, ���� �� ������ ������ ������� 
	bool isFree(field& Field, coordinates cord);

	// ������� ����������, ���� �� ������ ������ "�������" ������
	bool isKilled(field Field, coordinates xy);

	// ������� ������� ������� �������
	int doKilling(field& Field, coordinates xy, int count);

	// ������� �������� ��� �� ������������� �������� �� ����
	coordinates checkWoundedShips(field Field);

	// ��������� ���������� �� ���� (�� ���� �� �������� ������ �� ���������� > 0)
	bool gameEnd(field Field);

	// ������� ��� �������� 
	bool doShot(int playerNum, coordinates cord); // playerNum => 0 - ������� �� �����, 1 - ������� �� ���� 

	// ��������� �� ���� ��� ����������� �������
	bool setAllShips(field& Field, int settingType = 1);

	// ��������� �� ���� ��� ����������� ������� (����������� ������� �� ���� ����)
	bool setAllShipsForGoodStart(field& Field);

	// ���������� ��������� ������� ������� ������� �� ������ ��������� 
	int setShip(field& Field, coordinates cord, const int size, int alreadySet = 0, int settingType = 1);

	// ������� ����� ������������ �������
	void removeShip(field& Field, coordinates cord);

	// �������������� 2 � ��������� ������������ ������� �� settingType
	void confirmShip(field& Field, coordinates cord, int settingType);

	// ���� ���������, ����� ������ �������� ������� ��������� �������� �������
	coordinates setWoundedShip(field& Field, coordinates startCord);

	// ������� ������ ��������� ������ ���������
	coordinates randomCoordinates(field Field);

	// ������� ������� �������� �� ����
	float calculateEntropy(field Field);

	// ��������� ����������� �� ����
	field calculateChances(field Field);

	// �������� ���������� ���� �� ����� (���� �� ���� � ��������� �� �� ��������)
	bool readGuestField(field& Field, const char* link);

	// ������� ���� ������ ������� � ������� ��� � ����
	int shipSize(field& Field, coordinates startCord);

	// ������� ������� ������� �������� ����� �����������
	coordinates ChooseBestShot_Random(field Field);

	// ������� ������� ������� �������� ����� ������� ����������� �������� �� �����
	coordinates ChooseBestShot_BorderTactic(field Field);

	// ������� ������� ������� �������� ����� ��������
	coordinates ChooseBestShot_Entropy(field Field);

	//����� ����
	void print(field Field, bool shipVizible, bool toFile);

public:
	// �������� ���� (������� � ��������� ���� ��� �������)
	Game();

	// ������ ����
	bool gameStart();
};

#endif

