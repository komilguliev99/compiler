// Подпрограммы инициализации ФУ
#include "stdafx.h"
#include "FUini.h"
#include "Consts.h"
#include "Bus.h"
#include "Console.h"
#include "StrGen.h"
#include "Lex.h"
#include "Find.h"
#include "List.h"
#include "GraphTransf.h"
#include "AluGeneral.h"

using namespace std;

FU *ConsIni(FU *Bus, FU *TEmpl)
{
 	return (FU*)new Console(Bus, TEmpl);
}

FU *BusIni(FU *BusContext, FU *TEmpl)
{
	return (FU*)new BusFU(BusContext, TEmpl);
}

FU *LexIni(FU *BusContext, FU *TEmpl)
{
	return (FU*)new Lex(BusContext, TEmpl);
}
FU *StrGenIni(FU *BusContext, FU *TEmpl)
{
	return (FU*)new StrGenContext(BusContext, TEmpl);
}

FU *FindIni(FU *BusContext, FU *TEmpl)
{
	return (FU*)new Find(BusContext, TEmpl);
}

FU *ListIni(FU *BusContext, FU *TEmpl)
{
	return (FU*)new List(BusContext, TEmpl);
}

FU* GraphTrasfIni(FU* BusContext, FU* TEmpl)
{
	return (FU*) new GraphTransf(BusContext, TEmpl);
}

FU* AluGeneralIni(FU* BusContext, FU* TEmpl)
{
	return (FU*) new AluGeneral(BusContext, TEmpl);
}