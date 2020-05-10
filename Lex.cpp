	#include "stdafx.h"
	#include "Lex.h"
	#include <string.h>

	using namespace std;

	void Lex::LexOut(bool Copy) // Выдача лексемы потребителю
	{
		auto uk = UnicAtr.find(LexBuf[ib].atr);
		if (uk!= UnicAtr.end())
		{
			if (uk->second.Fu != nullptr && uk->second.Mk!= 0)
				if(Copy)
					uk->second.Fu->ProgFU(uk->second.Mk, { TIP, &LexBuf[ib] });
				else
					uk->second.Fu->ProgFU(uk->second.Mk, { TIP, LexBuf[ib].сlone() });
		}
		else
			if (Receiver != nullptr) 
				if (Copy)
					Receiver->ProgFU(ReceiverMK, { TIP, LexBuf[ib].сlone() });
				else
					Receiver->ProgFU(ReceiverMK, { TIP, &LexBuf[ib] });
	}

	void Lex::ProgFU(int MK, LoadPoint Load)
	{
		// Доделать буфер ИП с лексемами
		switch (MK)
 		{
		case 0: // Reset
			S = 0; // Номер состояния распознающего автомата
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
		case 12: // StartProgSet Установить программу, запускаемую перед началом компиляции 
			StartProg = Load.Point;
			break;
		case 13: // StopProgSet Установить программу, запускаемую при досрочном завершении лексисческого анализа по МК Stop 
			StopProg = Load.Point;
			break;
		case 14: // FinProgSet  Установить программу, запускаемую после окончания анализа строки
			FinProg = Load.Point;
			break;
		case 15:// UnicAtrSet Установить уникальный атрибут
			UnicAtr[*(int*)Load.Point] = { 0,Bus };
			break;
		case 16:// UnicMkSet Установить МК для уникального атрибута
			if(UnicAtr.size())
				(--UnicAtr.end())->second.Mk= *(int*)Load.Point;
			break;
		case 17:// UnicFuSet Установить контекст для уникального атрибута
			if (UnicAtr.size())
				(--UnicAtr.end())->second.Fu = (FU*)Load.Point;
			break;
		case 18:// UnicReset Сбросить список уникальных атрибутов
			UnicAtr.clear();
			break;
		case 20: // LexBufSizeSet Установить размер буфера лексем
			if (Load.Type >> 1 != Dint) break;
			for (int i = 0; i < SizeBuf; LexBuf[i++].Load.Clear());
			delete[] LexBuf;
			SizeBuf = *(int*)Load.Point;
			LexBuf = new ip[SizeBuf];
			for (int i = 0; i < SizeBuf; LexBuf[i++].Load = { 0,nullptr });
			break;
		case 27: //LexemaReplace Заменить тукущую лексему в буфере
		case 28: //LexemaReplaceToReceiver Выдать лексему из нагрузка МК получателю, заменив тукущую лексему в буфере
		case 29: //LexemaReplaceCopyToReceiver Выдать копию лексемы из нагрузки МК получателю, заменив тукущую лексему в буфере
		case 30: //LexemaToReceiver Выдать лексему из нагрузка МК получателю
		case 31: //LexemaCopyToReceiver Выдать копию лексемы из нагрузки МК получателю
			if(MK==30 || MK==31) ib = (ib + 1) % SizeBuf;
			LexBuf[ib].Load.Clear(); // Удаляем нагрузку ИП
			LexBuf[ib].copy(Load);
			if(MK!=27)LexOut(MK==31);
			break;
		case 32: //LastLexemaToReceiver Выдать последнюю лексему получателю
		case 33: //LastLexemaCopyToReceiver Выдать последнюю лексему получателю
			if (Receiver != nullptr)
				if (MK==33)
					Receiver->ProgFU(ReceiverMK, { TIP, LexBuf[ib].сlone() });
				else
					Receiver->ProgFU(ReceiverMK, { TIP, &LexBuf[ib] });
	//		LexOut(MK==33);
			break;
		case 35: //LastLexemaOutMk Выдать MK c последней лексемой
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, &LexBuf[ib] });
			break;
		case 36: // LastLexemaCopyOutMk Выдать МК с копией последней лексемы
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[ib].сlone() });
			break;
		case 40: //PrevLexemaToReseiver Выдать предыдущую лексему получателю
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, &LexBuf[(ib-1+ SizeBuf)% SizeBuf] });
			break;
		case 41: //PrevLexemaCopyToReseiver Выдать копию предыдущей лексемы получателю
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[(ib - 1 + SizeBuf) % SizeBuf].сlone() });
			break;
		case 45: //PrevLexemaOutMk Выдать МК с предыдущей лексемой
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
					Receiver->ProgFU(*(int*)Load.Point, { TIP, &LexBuf[(ib - 1 + SizeBuf) % SizeBuf] });
			break;
		case 46: //PrevLexemaCopyOutMk Выдать МК с копией предыдущей лексемы
			if (Receiver != nullptr && Load.Type >> 1 == Dint) 
				Receiver->ProgFU(*(int*)Load.Point, { TIP, LexBuf[(ib - 1 + SizeBuf)% SizeBuf].сlone() });
			break;
		case 50: // LastLexemaAtrSet Установить атрибут последней лексемы
			if (Load.Type >> 1 == Dint)
				LexBuf[ib].atr=*(int*)Load.Point;
			break;
		case 51: // PrevLexemaAtrSet Установить атрибут предыдущей лексемы
			if (Load.Type >> 1 == Dint)
				LexBuf[(ib - 1 + SizeBuf) % SizeBuf].atr = *(int*)Load.Point;
			break;
		case 55: // LastLexemaLoadSet Установить нагрзузку у последней лексемы
			LexBuf[ib].Load = Load;
			break;
		case 56: // PrevLexemaLoadSet Установить нагрзузку у предыдущей лексемы
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load = Load;
			break;
		case 57: // LastLexemaLoadCopySet Установить копию нагрзузки у последней лексемы
			LexBuf[ib].Load.Copy(&Load);
			break;
		case 58: // PrevLexemaLoadCopySet Установить копию нагрзузки у предыдущей лексемы
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Copy(&Load);
			break;
		case 65: // LastLexemaVarSet Установить тип переменной для нагрузки последней лексемы
			LexBuf[ib].Load.Type |= 1;
			LexBuf[ib].Load.Type --;
			break;
		case 66: // PrevLexemaVarSet Установить тип переменной для нагрузки предыдущей лексемы
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Type |= 1;
			LexBuf[(ib - 1 + SizeBuf) % SizeBuf].Load.Type--;
			break;

		case 99: // Stop Остановить лексический анализ
			Work = false;
			ProgExec((IC_type)StopProg,Bus); // выполнить программу по останову лексического анализа
			break;
		case 100: // Lexing
		{
//			ProgExec((IC_type)StartProg,Bus);
			if (Load.Type >> 1 == Dstring && *((string*)Load.Point) == "")
			{
				ib = (ib + 1) % SizeBuf;
				LexBuf[ib].Load.Clear(); // Удаляем нагрузку ИП
				LexBuf[ib] = { StrAtr,Tstring , new string("") };
				LexOut();
			}
			(*(string*)Load.Point) += " "; // Добавление мнимого конечного элемента
			Work = true;
			string FigureBuf;
			for (auto i = (*(string*)Load.Point).begin(); i != (*(string*)Load.Point).end() && Work; i++)
				switch (S)
				{
				case 0: // Стартовое состояние
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
						ib = (ib + 1) % SizeBuf; // Индекс буфера лексем
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
				case 1: // Распознание числа
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
						LexBuf[ib].Load.Clear(); // Удаляем нагрузку ИП
						LexBuf[ib] = { IntAtr,Tint , tint };
						LexOut();

						S = 0;
						i--; // Для обработки сепаратора
						break;
					}
					Work = false;
					ProgExec(ErrProg, Bus);
					break;
				case 2: // После десятичной точки
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
				case 3: // Число после десятичной точки
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
		ProgExec(FinProg, Bus); // Выполнить 
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
