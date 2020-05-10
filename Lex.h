#pragma once
#include "Consts.h"
#include <set>
#include <map>

struct MkFu
{
	int Mk;
	FU* Fu;
};




class Lex : public FU {
private:
	void LexOut(bool Copy=false);
	string ABC_templ = "_QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"; // Алфавит символов
	string Seps_templ = "`!@#$%^&*()+=-{}[]?;:,.\\/|\"";// Алфавит разделителей
	set<char> ABC, Seps, Digit;
	void *StartProg = nullptr, *StopProg = nullptr, *FinProg = nullptr; // Программы, выполняемые при запуске процесса лексического анализа
public:
	void ProgFU(int MK, LoadPoint Load);
	Lex(FU *BusContext, FU *Templ);
	Lex() ;
	FU *Receiver = nullptr;
	int ReceiverMK = 0;
	int S = 0; // Номер состояния распознающего автомата
	bool Work = true; // Флаг рабочего режима лексера
	int MnemoAtr = -1, SeperatAtr=-2, IntAtr=-3, DoubleAtr=-4, BoolAtr=-5, StrAtr=-6;
	int ib = 0, SizeBuf = 5; // Текущая позиция выходной ИП выходной лексемы и размер буфера
	ip *LexBuf; // Буфер выходных лексем
	vector<ip> *ErrProg=nullptr; // Программа, запускаемая при ошибке
	map<int,MkFu> UnicAtr; // Список специфических атрибутов, по которым идет обработки другими ФУ
	// Буфер описаний лексем {"dsfsdf",-3,"dfsdfs",""}
};