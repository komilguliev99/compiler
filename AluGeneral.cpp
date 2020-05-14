#include "stdafx.h"
#include "AluGeneral.h"

void AluGeneral::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0: break; // Reset
		TreadStack.clear();
		break;
	case 1: // prog
//		if(Load.Type>>2!=DIC) // �������� ��� ������ �� ��
//		{break;/* ��������� �� ������ ���� */}
		TreadStack.push_back({ {}, (ip*)Load.Point,nullptr });
		TreadStack.back().AluStack.push_back({ { Tint ,(void*) new int }, nullptr });
		ProgExec(Load.Point, Bus);
		TreadStack.pop_back();
		break;
	case 10: // DeepOut
		*(int*)Load.Point= TreadStack.size();
		break;
	case 11: // DeepOutMk
		Bus->MkExec(*(int*)Load.Point, {Cint,(int)TreadStack.size() });
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}

void	AluGeneral::error_msg(int error_code)
{
	switch (error_code)
	{
		case 1:
			cout << "Not registered operation!";
		case 2:
			cout << "Not allowed operation!";
		case 3:
			cout << "Divided by zero!";
	}
}

void	AluGeneral::calc(int MK, LoadPoint *load)
{
	switch (MK)
	{
		case 2:
			this.add(load);
			break;
		case 3:
			this.sub(load);
			break;
		case 4:
			this.mult(load);
			break;
		case 5:
			this.div(load);
			break;
		default:
			std::cout << "Not registered operation!";
			break;
	}
}

void	AluGeneral::add(LoadPoint *load)
{

}
