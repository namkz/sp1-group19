#ifndef  _INVENTORY_H
#define _INVENTORY_H

#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>
#include <conio.h>

using namespace std;

struct SItem
{
	string sStringName;
	int sType; //o - Hat , 1 - Necklace , 2 - Robe, 3 - Main Hand , 4 - Glove, 5 - Shoes
	int sHealth;
	int sMana;
	int sAttack;
	int sDefense;

};

void background()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
}

void red()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void Green()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

#endif //_INVENTORY_H
