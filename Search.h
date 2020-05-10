#pragma once
#include "Consts.h"
#include "Search.h"
#include <set>

class Search
{
private:
	bool null_check();// �������� �� ���� (false, ���� �� � �������)
	void MkAtrExec();
public:
//	vector<ip>* Template; // ������ ��� ������
	LoadPoint Template={0, nullptr}; // ������ ��� ������
	LoadPoint Obj = { 0, nullptr }; //�� ��� ������
	bool MkMode = false; //  ����� ���������� ���� �� � ��-������� (��-�� ��������� ����� �������, ������ �������� ������ 0)
	set<int> MkAtr; // ������ ��������, �������������� ��, ������������� � ������ ��������� ������
	FU *MainFU; // ������ �� ������� ��, ��������� ���������� ������������ searcher-�
	bool Rez=false; // ��������� ������
	vector<ip> *SuccessProg = nullptr, *FailProg = nullptr; // ��������� �� ��������� �� ������ � �������� ������
	vector<ip> *SuccessAfterProg = nullptr, *FailAfterProg = nullptr; // ��������� �� ��������� �� ������ � �������� ������, ������������� ����� ��������� ��������� �����
	ip *IPTemplPoint = nullptr, *IPPoint = nullptr; // ��������� �� ��������� �� � ������� � � ��
	int Prog_atr = ProgAtr; // ������� ���������, ������������� ��� �������� ������ (<0 ������� �� �����������)
	void MkAtrAdd(int MK) { MkAtr.insert(MK); };
	void MkAtrClear() { MkAtr.clear(); };
	bool Search::FindIPObj(LoadPoint obj, LoadPoint Templ, bool XOR=false); // �����, ���� obj �������� �� (��� Or And)
	bool FindOr(LoadPoint obj); // �����
	bool FindXor(LoadPoint obj); // �����
	bool FindAnd(LoadPoint obj); // �����
	bool FindAndSource(LoadPoint obj); // �����
	bool FindIP(LoadPoint obj) {}; // �������� ��
};