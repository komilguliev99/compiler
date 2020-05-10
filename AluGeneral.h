#pragma once
#include "Consts.h"
#include <vector>

struct ALU // АЛУ
{
	LoadPoint Accmulater; // Аккумулятор АЛУ
	ip* AluPC; // Счетчик адреса АЛУ
	//...
};

struct			local_var // Локальная переменная
{
	string		name;	  // Названия переменной 
	void*		value;    // Значения
	char		type;	  // тип переменной
};

struct			accumulator // Аккумулятор
{
	void*		value;		// значение
	char		type;		// тип
};


struct ThreadContext // Контекст вычислительной нити
{
	vector <ALU> AluStack; // АЛУ
//	LoadPoint Accmulater; // Аккумулятор АЛУ
	ip* ThreadPC=nullptr; // Счетчик адреса АЛУ
	ip* StartPointer = nullptr; // Адрес ИК начала программы
};

class AluGeneral : public FU
{
private:
	vector <ThreadContext> TreadStack; // Вектор контекстов вычислительных нитей
	int breakCounter = 0;
public:
	void ProgFU(int MK, LoadPoint Load) override;
	AluGeneral(FU* BusContext, FU* Templ) : FU(BusContext) {
		Bus = BusContext;
	};
	AluGeneral() : FU() {
		Bus = nullptr;
	};
};