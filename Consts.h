// Описание основных констант и информационных конструкций
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std; 

// Типы переменных
const int Tvoid = 0, Tint = 2, Tfloat = 4, Tdouble = 6, Tchar = 8, Tstring = 10, TIP = 12, Tbool = 14, TIC = 16;
const int TPPoint = 18, TGraph=20, TFU=22;
// Типы констант
const int Cvoid = 1, Cint = 3, Cfloat = 5, Cdouble = 7, Cchar = 9, Cstring = 11, CIP = 13, Cbool = 15, CIC = 17;
const int CPPoint = 19, CGraph = 21, CFU = 23;
// Общие типы данных (остаток от целочисленного деления на 2 типа переменной или константы)
const int Dvoid=0, Dint = 1, Dfloat = 2, Ddouble = 3, Dchar = 4, Dstring = 5, DIP = 6, Dbool = 7, DIC = 8;
const int DPPoint = 9, DGraph = 10, DFU = 11;

// Типы ФУ
const int FUBus = 0, FUCons = 1, FUStrGen = 2, FULex = 3, FUList = 4, FUFind = 5;

// Общие атрибуты
const int ProgAtr = -10, Atr=-6;

bool isIPinIC(void* iP, void* iC); //проверка, что ИК входит в ИП

class LoadPoint
{
public:
	unsigned int Type = 0; // Неизвестный тип
	void *Point;
	bool IsConvert(unsigned int T) {}; // Тест на возможность конвертации значения из Point в определенный тип
	int ToInt() { return *(int*)Point; }; // Перевод в integer
	double ToDouble() { return *(double*)Point; }; // Перевод в integer
	bool ToBool() { return *(bool*)Point; }; // Перевод в integer
	char ToChar() { return *(char*)Point; }; // Перевод в integer
	string ToStr() { return *(string*)Point; }; // Перевод в integer
	void Copy(LoadPoint *LP);
	void Clear(); // Сброс нагрузки ИП
	void VarClear(); // Сброс нагрузки ИП в том числе и с переменной (переменная стирается)
	void print(void* AtrMnemo=nullptr, string offset=""); // Параметр - указатель на табл. мнемоник атрибутов
	LoadPoint Clone(); // Дублировать нагрузку
	void ConstTypeSet(bool F = true) { if (F)Type |= 1; else VarTypeSet(); }; // Установить тип 'константа'
    // Установить тип 'переменная'
	void VarTypeSet(bool F = true) {
		if (!F) { ConstTypeSet(); return; } Type |= 1; if (Type >= 0)Type -= 1; else Type += 1;}
	LoadPoint IpOut() // Возвращается указатель на ИП или на первую ИП из ИК, иначе null
	{
		if (Type >> 1 == DIP) return *this;
//		if (Type >> 1 == DIC) return { DIP,((IC_type)Point)->begin()._Ptr};
//		return { 0,nullptr };
	};
	bool IpTest() { return (Type >> 1 == DIP || Type >> 1 == DIC); } // Является ли нагрузка ИП?

};

// struct TAtrMnrmo

class ip // Информационная пара
{
public:
	int atr = 0;
	LoadPoint Load = {0, nullptr }; // Указатель на нагрузку с типом даннных
	~ip() { }//Load.Clear(); };
	void copy(ip *IP)// Копирование ИП
	{
		atr = IP->atr;
		Load.Point = IP->Load.Point;
		Load.Type = IP->Load.Type;
	};
	void copy(void *IP) { copy(*(ip*)IP); }// Копирование ИП
	void copy(ip &IP)// Копирование ИП
	{
		atr = IP.atr;
		Load.Copy(&IP.Load);
	};
	void copy(LoadPoint &LP)// Копирование ИП
	{
		switch (LP.Type >> 1)
		{
		case DIP:
			atr = ((ip*)LP.Point)->atr;
			Load.Copy(&((ip*)LP.Point)->Load);
			break;
		case DIC:
			atr = ((vector<ip>*)LP.Point)->begin()->atr;
			Load.Copy(&((vector<ip>*)LP.Point)->begin()->Load);
		}
	};
	ip* сlone()
	{
		ip* ip_new = new ip;
		ip_new->copy(*this);
		return ip_new;
	}

};

struct deletedIC //удаленная ИП
{
	vector<ip*>* IC; // информационная капсула, содержащая удаленную ИП
	void* programm; // указатель на милипрограмму (ИК, содержащую набор милликоманд)
};

class FU {  // Ядро функционального устройства
public:
	virtual void ProgFU(int MK, LoadPoint Load) {}; // Реализация логики работы ФУ
	FU() { Bus = nullptr; };
	FU(FU *BusContext) { Bus = BusContext; };
	void MkExec(int MK, LoadPoint Load, FU* BusContext = nullptr); // Выполнить одну милликоманду 
//	void OutMK2(LoadPoint MKL, LoadPoint Load) 
//	{if(MKL.Point!=nullptr && MKL.Type>>1==Dint)
//		MkExec(*((int*)(Load.Point)), Load); };
	FU *Bus; // Ссылка на контекст Шины
	int FUMkRange = 1000; // Диапазон МК для каждого ФУ
	void CommonMk(int Mk, LoadPoint Uk); // Выполнение общих МК для ФУ
};

typedef  vector<vector<ip>*> ICVect;
typedef  vector<ip>* IC_type;

void ICDel(void* Uk);// Удаление ИК

void* ICCopy(LoadPoint uk);// Копирование ИК

void ProgExec(void *Uk, FU* Bus, vector<ip>::iterator *Start=nullptr); // Исполнение программы из ИК

bool LoadCmp(LoadPoint x, LoadPoint y); // Сравнение двух нагрузок ИП
bool IPCmp(ip* x, ip* y); // Сравнение двух  ИП
void AtrProgExec(vector<ip>* Prog, int Atr, FU* Bus, bool AfterContinue = false); // Найти в ИК ИП с атрибутом Atr и выполнить программу либо по адр. в нагрузке, либо после найденной ИП
