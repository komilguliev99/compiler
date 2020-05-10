// ФУ Консоль
#pragma once
#include "Consts.h"

class Console : public FU
{
private:
	string prefix;
public:
	void ProgFU(int MK, LoadPoint Load) override;
	Console(FU *BusContext, FU *Templ) : FU(BusContext) { Bus = BusContext; };
	Console() : FU() { Bus = nullptr; };
private:
};
