#include "stdafx.h"
#include "consts.h"
#include "Bus.h"
#include "FUini.h"
#include <string>
#include <map>
#include <algorithm>
#include "IndFileConvertOld.h"

using namespace std;

void BusFU::FUTypesIni()
{
	FUs.push_back(nullptr);
	FUs.push_back(nullptr);
	FUTypeCorrect = 0;
}

void BusFU::ProgFU(int MK, LoadPoint Load)
{
	ICVect* ipVect; // Указатель на вектор ИК
	if (MK >= FUMkRange*2)
	{
		int FU_num = MK / FUMkRange;
		if(FU_num<FUs.size())
			FUs[FU_num]->ProgFU(MK%FUMkRange, Load);
	}
	else
		switch (MK%FUMkRange)
		{
		case 0: // Сброс
			break;

		case 1: // MakeFU Создать ФУ
			FUs.push_back(FUTypes[*(int *)Load.Point - FUTypeCorrect](this, FUTempl));
			break;
		case 5: // ProgExec Выполнить программу из ИК
			ProgExec((vector<ip>*)Load.Point, this);
			break;
		case 10: // FileOldProgExec Выполнить программу из файла
			ipVect = ConvIndOld(*(string*)Load.Point);
			if(ipVect->size()>0) ProgExec( (*ipVect)[0], this);
			break;
		case 155:// FUTypeCorrectSet Установить коррекцию номера типа ФУ (для переноса ОА-программы на другую ОА-платформу)
			FUTypeCorrect = *(int *)Load.Point;
			break;
		default:
			CommonMk(MK, Load);
			break;
		}
}