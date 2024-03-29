// �� ������
#pragma once
#include "Consts.h"
#include "Search.h"

class List : public FU
{
public:
	vector<ip>* ListHead = nullptr; // ��������� �� ������ ������
	ip* LineUk=nullptr; // ��������� �� ��������� ������ ������
	int LineNum = 0; // ����� ������ ����������� �����
	int MultiLineMode = 0; // 0 - ����� ������ ������� ����������, 1 - ����� ���� ����������
	void ProgFU(int MK, LoadPoint Load) override;
	int ReceiverMk = 0; // �� ��� ���������� �������
	int *ReceiverMkUk = &ReceiverMk; // ��������� �� �� ��� ���������� �������
	vector<int> LineStack; // ���� ������� �����
	Search Searcher;
	IC_type FailAllProg = nullptr;
//	vector<ip> *LineProg = nullptr;
	vector<ip> *DefProg = nullptr;
	List(FU *BusContext, FU *Templ) : FU(BusContext) { ProgFU(0, { 0, nullptr }); Bus = BusContext; Searcher.MainFU = this; };
	List() : FU() { List(nullptr, nullptr); };
private:
};