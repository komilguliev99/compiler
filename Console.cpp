// ФУ Консоль
#include "stdafx.h"
#include "Console.h"
#include <iostream>
#include <string>
#include <fstream>

void Console::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0: break;// Reset
	case 1: // Out 
	case 2: // OutLn
	case 3: // LnOut
	case 4: // LnOutLn
		cout << prefix;
		if (MK == 3 || MK == 4) cout << endl;
		Load.print();
		if (MK == 2 || MK == 4) cout << endl;
		break;
	case 10: // LN Перевод строки
		cout << endl;
		break;
	case 20: // PrefixSet Установить префикс перед строкой
		if ((Load.Type) >> 1 == Dstring)
			prefix = *(string*)Load.Point;
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}

