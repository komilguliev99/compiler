#include "stdafx.h"
#include "Consts.h"
#include <iostream>
#include <string>
#include "LocationTable.h"

using namespace std;

void ICDel(void* Uk) // Удаление ИК
{
	for (auto i : *(IC_type)Uk)
		i.Load.Clear();
	delete (IC_type)Uk;
}

void* ICCopy(LoadPoint uk) // Копирование ИК
{
	IC_type CapsNew = new vector<ip>;
	if (uk.Type >> 1 == DIP) // Если передается ИП
	{
		CapsNew->push_back(*(*(ip*)uk.Point).сlone());
		return CapsNew;
	}
	IC_type Uk = (IC_type)uk.Point;
	CapsNew->resize(((IC_type)Uk)->size());
	for (auto i = ((IC_type)Uk)->begin(), j = CapsNew->begin(); j != CapsNew->end(); i++, j++)
	{
		j->atr = i->atr;
		j->Load.Copy(&i->Load);
	}
	return CapsNew;
}

void* GraphCopy(void* Uk, LocatTable* Table=nullptr) // Копирование ОА-графа
{
	return nullptr;
}

void GraphDel(void* Uk, LocatTable* Table = nullptr) // Удаление ОА-графа
{
	return;
}

LoadPoint LoadPoint::Clone()
{
	switch (Type)
	{
	case Cstring: return { Type, new string(*(string*)Point) };
	case Cint: return { Type, new int(*(int*)Point) };
	case Cfloat: return { Type, new float(*(float*)Point)};
	case Cdouble: return { Type,new double(*(double*)Point)};
	case Cchar: return { Type,new char(*(char*)Point) };
	case Cbool: return { Type,new bool(*(bool*)Point) };
	case CPPoint: return { Type,new (void*)(*(void**)Point) };
	case CIP: return { Type, (*(ip*)Point).сlone()};
	case CIC: return { Type, Point = ICCopy(*this) };
	default: return *this;
	}
}

void LoadPoint::Copy(LoadPoint *LP)
{
	Type = LP->Type;
	if (LP->Type % 2 == 0)
	{
		Point = LP->Point;
		return;
	}
	switch (Type)
	{
	case Cstring: Point = new string(*(string*)LP->Point); break;
	case Cint: Point = new int(*(int*)LP->Point); break;
	case Cfloat: Point = new float(*(float*)LP->Point); break;
	case Cdouble: Point = new double(*(double*)LP->Point); break;
	case Cchar: Point = new char(*(char*)LP->Point); break;
	case Cbool: Point = new bool(*(bool*)LP->Point); break;
	case CPPoint: Point = new (void*)(*(void**)LP->Point); break;
	case CIP: // ???
		break;
	case CIC:
		Point = ICCopy(*LP);
		break;
	}
}

void LoadPoint::Clear() // Сбросить нагрузку
{
	if (Type % 2 == 0) return;
	switch (Type)
	{
	case Cstring: delete (string*)Point; break;
	case Cint: delete (int*)Point; break;
	case Cfloat: delete (float*)Point; break;
	case Cdouble: delete (double*)Point; break;
	case Cchar: delete (char*)Point; break;
	case Cbool: delete (bool*)Point; break;
	case CPPoint: delete (void**)Point; break;
	case CIP: // ???
		break;
	case CIC:
		ICDel(Point);
		break;
	}
	Type = 0; Point = nullptr;

}

void LoadPoint::VarClear() // Сброс нагрузки ИП в том числе и с переменной (переменная стирается)
{
	Type -= Type % 2;
	Type += 1;
	Clear();
}

void LoadPoint::print(void* AtrMnemo, string offset)
{
	if (Point == nullptr)
	{
		cout << "null\n";
		return;
	}
	switch (Type)
	{
	case Tstring:
	case Cstring: cout << *(string*)Point; break;
	case Tint:
	case Cint:	  cout << *(int*)Point; break;
	case Tfloat:
	case Cfloat:  cout << *(float*)Point; break;
	case Tdouble:
	case Cdouble: cout << *(double*)Point; break;
	case Tchar:
	case Cchar:   cout << *(char*)Point; break;
	case Tbool:
	case Cbool:   cout << *(bool*)Point; break;
	case TIP:
	case CIP:
		if(((ip*)Point)->Load.Type >> 1 == DIP || ((ip*)Point)->Load.Type >> 1 == DIC)
			cout << ((ip*)Point)->atr << " ->\n";
		else
			cout << ((ip*)Point)->atr << " # ";
			((ip*)Point)->Load.print();
			break;
	case TIC:
	case CIC:
	{int _i = 0;
	for (auto i : *(vector<ip>*)Point)
	{
		if (i.Load.Type >> 1 == DIP || i.Load.Type >> 1 == DIC) {
			cout << offset << i.atr << " ->\n";
		}
		else
			cout << offset << i.atr << " # ";
		i.Load.print(nullptr, offset + "  ");
		cout << endl;
	}
	break;
	}
	default:
		cout << "unknown load\n";
	}
}


void ProgExec(void *UK, FU* Bus, vector<ip>::iterator *Start) // Исполнение программы из ИК
{
	vector<ip> *Uk = (IC_type) UK;
	if (Uk == nullptr || Bus == nullptr) return;
	if(Start==nullptr)
		for (auto &i : *Uk)
				Bus->ProgFU(i.atr, i.Load);
	else
		for(auto i= *Start;i!=Uk->end();i++)
			Bus->ProgFU(i->atr, i->Load);
}

void FU::CommonMk(int Mk, LoadPoint Uk)
{
	switch (Mk)
	{
		case 990: // ProgExec
			ProgExec(((vector<ip>*)Uk.Point), Bus);
			break;
		case 995: //ContextOut Выдать указатель на контекст ФУ
			if (Uk.Type >> 1 == Dvoid || Uk.Type >> 1 == DPPoint)
				Uk.Point = this;
			break;
		case 999: //ContextOutMK Выдать милликоманду с указателем на контекст ФУ
			if (Uk.Type >> 1 == Dint)
				Bus->ProgFU(*(int*)Uk.Point, { TFU, this });
			break;
	}
}

void FU::MkExec(int MK, LoadPoint Load, FU* BusContext) // Выдача МК с нагрузкой
{
	if(MK<FUMkRange) // Если МК адресована сомому ФУ
		ProgFU(MK, Load);
	else
		if (BusContext != nullptr && BusContext!=NULL)
			BusContext->ProgFU(MK, Load);
		else
			Bus->ProgFU(MK, Load);
}

bool LoadCmp(LoadPoint x, LoadPoint y) // Сравнение двух нагрузок ИП
{
	// Доработать с указателями на ИК и ИП
	if (x.Point == nullptr || y.Point == nullptr)
		return true;
	int xt = x.Type >> 1, yt = y.Type >> 1;
	if( (xt== Dint || xt == Dfloat || xt== Ddouble || xt== Dchar)&&(yt == Dint || yt == Dfloat || yt == Ddouble || yt == Dchar) )
		switch (xt)
		{
		case Dint:
			switch (yt)
			{
			case Dint: return *(int*)x.Point == *(int*)y.Point;
			case Dfloat: return *(int*)x.Point == *(float*)y.Point;
			case Ddouble: return *(int*)x.Point == *(double*)y.Point;
			case Dchar: return *(int*)x.Point == *(char*)y.Point;
			}
		case Dfloat:
			switch (yt)
			{
			case Dint: return *(float*)x.Point == *(int*)y.Point;
			case Dfloat: return *(float*)x.Point == *(float*)y.Point;
			case Ddouble: return *(float*)x.Point == *(double*)y.Point;
			case Dchar: return *(float*)x.Point == *(char*)y.Point;
			}
		case Ddouble:
			switch (yt)
			{
			case Dint: return *(double*)x.Point == *(int*)y.Point;
			case Dfloat: return *(double*)x.Point == *(float*)y.Point;
			case Ddouble: return *(double*)x.Point == *(double*)y.Point;
			case Dchar: return *(double*)x.Point == *(char*)y.Point;
			}
		case Dchar:
			switch (yt)
			{
			case Dint: return *(char*)x.Point == *(int*)y.Point;
			case Dfloat: return *(char*)x.Point == *(float*)y.Point;
			case Ddouble: return *(char*)x.Point == *(double*)y.Point;
			case Dchar: return *(char*)x.Point == *(char*)y.Point;
			}
		}
	if (xt == Dbool && yt == Dbool)
		return *(bool*)x.Point == *(bool*)y.Point;
	if (xt == Dstring && yt == Dstring)
		return *(string*)x.Point == *(string*)y.Point;

		return false;
}

bool IPCmp(ip* x, ip* y) // Сравнение двух  ИП
{
	if (!LoadCmp(x->Load, y->Load))
		return false;
	else
		if (x->atr == y->atr || x->atr == 0 || y->atr == 0)
			return true;
		else
			return false;
}

// Найти в ИК ИП с атрибутом Atr и выполнить программу либо по адр. в нагрузке, либо после найденной ИП
void AtrProgExec(vector<ip>* Prog, int Atr, FU* Bus, bool AfterContinue)
{
	auto i = Prog->begin();
	for (; i != Prog->end() && i->atr != Atr; i++);
	if (i != Prog->end())
	{
		if (i->Load.Point != nullptr)
			ProgExec((vector<ip> *)i->Load.Point, Bus);
		if((++i)!=Prog->end() && AfterContinue)
			ProgExec(Prog, Bus,&i);
	}
}

bool isIPinIC(void* iP, void* iC) //проверка, что ИК входит в ИП
{
	for (auto i = ((IC_type)iC)->begin()._Ptr; i != ((IC_type)iC)->end()._Ptr; i++) {
		if (IPCmp(i, ((ip*)iP)))
			return true;
	}
	return false;
}