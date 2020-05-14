#pragma once
#include "Consts.h"
#include <vector>

class Accumulator
{
	LoadPoint	accumulator;

	void		add(LoadPoint *load);
	void		sub(LoadPoint *load);
	void		div(LoadPoint *load);
	void		mult(LoadPoint *load);

public:
	void		calc(int MK, LoadPoint *load);
	LoadPoint	*get_accumulator();
}

struct ALU // ���
{
	LoadPoint Accmulater; // ����������� ���
	ip* AluPC; // ������� ������ ���
	//...
};

struct ThreadContext // �������� �������������� ����
{
	vector <ALU> AluStack; // ���
//	LoadPoint Accmulater; // ����������� ���
	ip* ThreadPC=nullptr; // ������� ������ ���
	ip* StartPointer = nullptr; // ����� �� ������ ���������
public:
	~ThreadContext() {
		AluStack.clear(); // ������� ����� ���
		// ������� ��������� ����������
		// ...
	}
};

class AluGeneral : public FU
{
private:
	vector <ThreadContext> TreadStack; // ������ ���������� �������������� �����
	int breakCounter = 0;
public:
	void ProgFU(int MK, LoadPoint Load) override;
	AluGeneral(FU* BusContext, FU* Templ) : FU(BusContext) {
		Bus = BusContext;
		ProgFU(0, { 0,nullptr });
	};
	AluGeneral() : FU() {
		Bus = nullptr;
	};
};