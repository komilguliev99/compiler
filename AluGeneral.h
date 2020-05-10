#pragma once
#include "Consts.h"
#include <vector>

struct ALU // ���
{
	LoadPoint Accmulater; // ����������� ���
	ip* AluPC; // ������� ������ ���
	//...
};

struct			local_var // ��������� ����������
{
	string		name;	  // �������� ���������� 
	void*		value;    // ��������
	char		type;	  // ��� ����������
};

struct			accumulator // �����������
{
	void*		value;		// ��������
	char		type;		// ���
};


struct ThreadContext // �������� �������������� ����
{
	vector <ALU> AluStack; // ���
//	LoadPoint Accmulater; // ����������� ���
	ip* ThreadPC=nullptr; // ������� ������ ���
	ip* StartPointer = nullptr; // ����� �� ������ ���������
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
	};
	AluGeneral() : FU() {
		Bus = nullptr;
	};
};