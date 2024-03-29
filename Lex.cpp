	#include "stdafx.h"
	#include "Lex.h"
	#include <string.h>

	using namespace std;

	void Lex::LexOut(bool Copy) // ������ ������� �����������
	{
		auto uk = UnicAtr.find(LexBuf[ib].atr);
		if (uk!= UnicAtr.end())
		{
			if (uk->second.Fu != nullptr && uk->second.Mk!= 0)
				if(Copy)
					uk->second.Fu->ProgFU(uk->second.Mk, { TIP, &LexBuf[ib] });
				else
					uk->second.Fu->ProgFU(uk->second.Mk, { TIP, LexBuf[ib].�lone() });
		}
		else
			if (Receiver != nullptr) 
				if (Copy)
					Receiver->ProgFU(ReceiverMK, { TIP, LexBuf[ib].�lone() });
				else
					Receiver->ProgFU(ReceiverMK, { TIP, &LexBuf[ib] });
	}

	void Lex::ProgFU(int MK, LoadPoint Load)
	{
		// �������� ����� �� � ���������
		switch (MK)
 		{
		case 0: // Reset
			S = 0; // ����� ��������� ������������� ��������
				   //		FigureBuf = "";
			UnicAtr.clear();
			ReceiverMK = 0;
			ErrProg = nullptr;
			break;
		case 5: //ReceiverMKSet
			if (Load.Type >> 1 == Dint) ReceiverMK = *(int*)Load.Point; break;
		case 10: //ErrProgSet
			ErrProg = (vector<ip> *)Load.Point;
			break;
		case 12: // StartProgSet ���������� ���������, ����������� ����� ������� ���������� 
			StartProg = Load.Point;
			break;
		case 13: // StopProgSet ���������� ���������, ����������� ��� ��������� ���������� ������������� ������� �� �� Stop 
			StopProg = Load.Point;
			break;
		case 14: // FinProgSet  ���������� ���������, ����������� ����� ��������� ������� ������
			FinProg = Load.Point;
			break;
		case 15:// UnicAtrSet ���������� ���������� �������
			UnicAtr[*(int*)Load.Point] = { 0,Bus };
			break;
		case 16:// UnicMkSet ���������� �� ��� ����������� ��������
			if(UnicAtr.size())
				(--UnicAtr.end())->second.Mk= *(int*)Load.Point;
			break;
		case 17:// UnicFuSet ���������� �������� ��� ����������� ��������
			if (UnicAtr.size())
				(--UnicAtr.end())->second.Fu = (FU*)Load.Point;
			break;
		case 18:// UnicReset �������� ������ ���������� ���������
			UnicAtr.clear();
			break;
		case 20: // LexBufSizeSet ���������� ������ ������ ������
			if (Load.Type >> 1 != Dint) break;
			for (int i = 0; i < SizeBuf; LexBuf[i++].Load.Clear());
			delete[] LexBuf;
			SizeBuf = *(int*)Load.Point;
			LexBuf = new ip[SizeBuf];
			for (int i = 0; i < SizeBuf; LexBuf[i++].Load = { 0,nullptr });
			break;
		case 27: //LexemaReplace �������� ������� ������� � ������
		case 28: //LexemaReplaceToReceiver ������ ������� �� �������� �� ����������, ������� ������� ������� � ������
		case 29: //LexemaReplaceCopyToReceiver ������ ����� ������� �� �������� �� ����������, ������� ������� ������� � ������
		case 30: //LexemaToReceiver ������ ������� �� �������� �� ����������
		case 31: //LexemaCopyToReceiver ������ ����� ������� �� �������� �� ����������
			if(MK==30 || MK==31) ib = (ib + 1) % SizeBuf;
			LexBuf[ib].Load.Clear(); // ������� �������� ��
			LexBuf[ib].copy(Load);
			if(MK!=27)LexOut(MK==31);
			break;
		case 32: //LastLexemaToReceiver ������ ��������� ������� ����������
		case 33: //LastLexemaCopyToReceiver ������ ��������� ������� ����������
			if (Receiver != nullptr)
				if (MK==33)
					Receiver->ProgFU(ReceiverMK, { TIP, LexBuf[ib].�lone() });
				else
					Receiver->ProgFU(ReceiverMK, { TIP, &LexBuf[ib] });
	//		LexOut(MK==33);
			break;
		case 35: //LastLexemaOutMk ������ MK c ��������� ��������
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, &LexBuf[ib] });
			break;
		case 36: // LastLexemaCopyOutMk ������ �� � ������ ��������� �������
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[ib].�lone() });
			break;
		case 40: //PrevLexemaToReseiver ������ ���������� ������� ����������
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, &LexBuf[(ib-1+ SizeBuf)% SizeBuf] });
			break;
		case 41: //PrevLexemaCopyToReseiver ������ ����� ���������� ������� ����������
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[(ib - 1 + SizeBuf) % SizeBuf].�lone() });
			break;
		case 45: //PrevLexemaOutMk ������ �� � ���������� ��������
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
					Receiver->ProgFU(*(int*)Load.Point, { TIP, &LexBuf[(ib - 1 + SizeBuf) % SizeBuf] });
			break;
		case 46: //PrevLexemaCopyOutMk ������ �� � ������ ���������� �������
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[(ib - 1 + SizeBuf)% SizeBuf].�lone() });
			break;
		case 50: // LastLexemaAtrSet ���������� ������� ��������� �������
			if (Load.Type >> 1 == Dint)
				LexBuf[ib].atr=*(int*)Load.Point;
			break;
		case 51: // PrevLexemaAtrSet ���������� ������� ���������� �������
			if (Load.Type >> 1 == Dint)
				LexBuf[(ib - 1 + SizeBuf) % SizeBuf].atr = *(int*)Load.Point;
			break;
		case 55: // LastLexemaLoadSet ���������� ��������� � ��������� �������
			LexBuf[ib].Load = Load;
			break;
		case 56: // PrevLexemaLoadSet ���������� ��������� � ���������� �������
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load = Load;
			break;
		case 57: // LastLexemaLoadCopySet ���������� ����� ��������� � ��������� �������
			LexBuf[ib].Load.Copy(&Load);
			break;
		case 58: // PrevLexemaLoadCopySet ���������� ����� ��������� � ���������� �������
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Copy(&Load);
			break;
		case 65: // LastLexemaVarSet ���������� ��� ���������� ��� �������� ��������� �������
			LexBuf[ib].Load.Type |= 1;
			LexBuf[ib].Load.Type --;
			break;
		case 66: // PrevLexemaVarSet ���������� ��� ���������� ��� �������� ���������� �������
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Type |= 1;
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Type--;
			break;

		case 99: // Stop ���������� ����������� ������
			Work = false;
			ProgExec((IC_type)StopProg,Bus); // ��������� ��������� �� �������� ������������ �������
			break;
		case 100: // Lexing
		{
//			ProgExec((IC_type)StartProg,Bus);
			if (Load.Type >> 1 == Dstring && *((string*)Load.Point) == "")
			{
				ib = (ib + 1) % SizeBuf;
				LexBuf[ib].Load.Clear(); // ������� �������� ��
				LexBuf[ib] = { StrAtr,Tstring , new string("") };
				LexOut();
			}
			(*(string*)Load.Point) += " "; // ���������� ������� ��������� ��������
			Work = true;
			string FigureBuf;
			for (auto i = (*(string*)Load.Point).begin(); i != (*(string*)Load.Point).end() && Work; i++)
				switch (S)
				{
				case 0: // ��������� ���������
					if (*i == ' ') break;
					if (*i == '"')
					{
						FigureBuf = "";
						S = 4;
					}
					if (Seps.count(*i))
					{
						string  *tstr = new string;
						*tstr += *i;
						ib = (ib + 1) % SizeBuf;
						LexBuf[ib].Load.Clear();
						LexBuf[ib] = { SeperatAtr,Tstring , tstr };
						LexOut();
						break;
					}
					if (*i >= '0' && *i <= '9')
					{
						FigureBuf = *i;
						S = 1;
						ib = (ib + 1) % SizeBuf; // ������ ������ ������
						break;
					}
					if (ABC.count(*i))
					{
						S = 3;
						FigureBuf = *i;
						break;
					}
					Work = false;
					if (ErrProg != nullptr) ProgExec(ErrProg, Bus);
					break;
				case 1: // ����������� �����
					if (*i >= '0' && *i <= '9')
					{
						FigureBuf += *i;
						break;
					}
					if (*i == '.')
					{
						FigureBuf = FigureBuf + (*i);
						S = 2;
						break;
					}
					if (Seps.count(*i) || *i == ' ')
					{
						int  *tint = new int;
						*tint = atoi(FigureBuf.c_str());
						ib = (ib + 1) % SizeBuf;
						LexBuf[ib].Load.Clear(); // ������� �������� ��
						LexBuf[ib] = { IntAtr,Tint , tint };
						LexOut();

						S = 0;
						i--; // ��� ��������� ����������
						break;
					}
					Work = false;
					ProgExec(ErrProg, Bus);
					break;
				case 2: // ����� ���������� �����
					if (*i >= '0' && *i <= '9')
					{
						FigureBuf = FigureBuf + (*i);
						break;
					}
					if (Seps.count(*i) || *i == ' ')
					{
						double *ft = new double;
						*ft = atof(FigureBuf.c_str());
						ib = (ib + 1) % SizeBuf;
						LexBuf[ib].Load.Clear();
						LexBuf[ib] = { DoubleAtr, Tdouble, ft };
						LexOut();
						S = 0;
						i--;
					}
					Work = false;
					ProgExec(ErrProg, Bus);
					break;
				case 3: // ����� ����� ���������� �����
					if (Seps.count(*i) || *i == ' ')
					{
						S = 0;
						i--;
						string *st = new string;
						*st = FigureBuf;
						ib = (ib + 1) % SizeBuf;
						LexBuf[ib].Load.Clear();
						LexBuf[ib] = { MnemoAtr, Tstring, st };
						LexOut();
						break;
					}
					if (ABC.count(*i) || *i >= '0' && *i <= '9')
					{
						FigureBuf += *i;
						break;
					}
					Work = false;
					ProgExec(ErrProg, Bus);
					break;
				case 4:
					if (*i != '"')
						FigureBuf += *i;
					else
					{
						S = 0;
						string *st = new string;
						*st = FigureBuf;
						ib = (ib + 1) % SizeBuf;
						LexBuf[ib].Load.Clear();
						LexBuf[ib] = { StrAtr, Cstring, st };
						LexOut();
					}
					Work = false;
					ProgExec(ErrProg, Bus);
					break;
				default:
					break;
				}
		}
		ProgExec(FinProg, Bus); // ��������� 
		break;
		default:
			CommonMk(MK, Load);
		}
	}

	Lex::Lex(FU *BusContext, FU *Templ) 
	{
		Bus = BusContext;
		Receiver = BusContext;
		copy(ABC_templ.begin(), ABC_templ.end(), inserter(ABC, ABC.end()));
		copy(Seps_templ.begin(), Seps_templ.end(), inserter(Seps, Seps.end()));
		LexBuf = new ip[SizeBuf];
		for (int i = 0; i < SizeBuf; LexBuf[i++].Load = { 0,nullptr });
		ProgFU(0, {0,nullptr});
	}

	Lex::Lex() : FU() 
	{
		Lex(nullptr, nullptr); 
	}
