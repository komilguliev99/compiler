#include "stdafx.h"
#include "AluGeneral.h"

void AluGeneral::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0: break; // Reset

	case 1: // Set
		cout << "ALU Set"<<endl;
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}
