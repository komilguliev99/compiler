// StrGen.cpp: определяет точку входа для консольного приложения.

#include "stdafx.h"
#include "Consts.h"
#include "Bus.h"
#include "Console.h"
#include "StrGen.h"
#include "Lex.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main()
{
	LoadPoint LP = LoadPoint();
	BusFU Bus;
	string STR;
//	STR = "LexTest.ind";
	STR = "ALUGeneral.ind";
	LP.Type = Tstring;
	LP.Point = &STR;
	Bus.ProgFU(10, LP);
	system("pause");
	return 0;
}
