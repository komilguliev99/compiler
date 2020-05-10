// ФУ Список
#include "stdafx.h"
#include "List.h"

using namespace std;

void List::ProgFU(int MK, LoadPoint Load)
{
	switch (MK)
	{
	case 0: // Reset
		break;
	case 1:	// Set
		ListHead = (IC_type)Load.Point;
		break;
	case 2:// Out Выдать ссылку на список
		if (Load.Type >> 1 == Cvoid)
			*(void**)Load.Point = ListHead;
		break;
	case 3:// OutMk Выдать МК со ссылкой на список
		if (Load.Type >> 1 == Dint)
			MkExec(*(int*)Load.Point, { TIC,(void*)ListHead });
		break;
	case 5:	// MultiLineModeSet
		if(Load.Type>>1==Dint)
			MultiLineMode = *(int *)Load.Point;
	case 7: // MkModeSet Режим выполнения всех МК в ИК-шаблоне (МК-ой считается любой атрибут, индекс которого больше 0) При пустой нагрузке режим устанавливается
		if (Load.Point == nullptr)
			Searcher.MkMode = true;
		else
			switch (Load.Type >> 1)
			{
			case Dbool:
				Searcher.MkMode = *(bool*)Load.Point;
			case Dint:
				Searcher.MkMode = *(int*)Load.Point;
			}
		break;
	case 8:// EmptyProgExec Выполнить программу, если список пуст
		if (ListHead == nullptr || ListHead->size() == 0)
			if(Load.Type>>1==DIC)
				ProgExec(Load.Point, Bus);
		break;
	case 9:// FullProgExec Выполнить программу, если список не пуст
		if (ListHead != nullptr && ListHead->size() != 0)
			if (Load.Type >> 1 == DIC)
				ProgExec(Load.Point, Bus);
		break;
	case 10: // SuccessProgSet Установить указатель на программу, выполняемую при удачном поиска в линии списка
		Searcher.SuccessProg = (IC_type)Load.Point;
		break;
	case 11: // SuccessAfterProgSet Установить указатель на программу, выполняемую при удачном поиска в линии списка после обоработки программ линии
		Searcher.SuccessAfterProg = (IC_type)Load.Point;
		break;
	case 15: // FailProgSet Установить указатель на программу, выполняемую в случае неудачного поиска в линии списка
		Searcher.FailProg = (IC_type)Load.Point;
		break;
	case 16: // FailAfterProgSet Установить указатель на программу, выполняемую в случае неудачного поиска в линии списка после обоработки программ линии
		Searcher.FailAfterProg = (IC_type)Load.Point;
		break;
	case 17: // FailAllProgSet Установить указатель на программу, выполняемую в случае неудачного поиска во всем  списке
		FailAllProg = (IC_type)Load.Point;
		break;

	case 20: // ProgAtrSet Установить атрибут программы
		if (Load.Point != nullptr && Load.Type >> 1 == Dint)
			Searcher.Prog_atr = *(int *)Load.Point;
		break;
	case 25: // BackOut Вылаить входной объект для поиска
		if (Load.Type == TPPoint)
			(*(LoadPoint*)Load.Point) = Searcher.Obj;
		break;
	case 30: // BackOutMk Вылаить МК со входным объектом для поиска
		if (Load.Type >> 1 == Dint)
			MkExec(*(int*)Load.Point, Searcher.Obj.Clone());
		break;
	case 31: // CopyBackOutMk Вылаить МК со входным объектом для поиска
		if (Load.Type >> 1 == Dint)
			MkExec(*(int*)Load.Point, Searcher.Obj.Clone());
		break;

	case 35: // LoadBackOutMk
		if (Load.Type >> 1 == Dint)
			MkExec(*(int*)Load.Point, Searcher.Obj);
		break;
	case 36: // LoadCopyBackOutMk
		if (Load.Type >> 1 == Dint)
			MkExec(*(int*)Load.Point, Searcher.Obj.Clone());
		break;

	case 90: // ProgAtrSet  Установить атрибут программы
		if ((Load.Type >> 1) == Dint)
			Searcher.Prog_atr = *(int*)Load.Point;
		break;
	case 100: // RezOut Выдача результата сравнения
		if (Load.Type == Tbool)
			*(bool*)Load.Point = Searcher.Rez;
		break;
	case 101: // RezOutMk Выдача МК с результатом сравнения
		MkExec(*(int*)(Load.Point), { Cbool , &Searcher.Rez });
		break;
	case 105: // MkAtrAdd Добавить МК
		if (Load.Type >> 1 == Dint)
			Searcher.MkAtrAdd(*(int*)Load.Point);
		break;
	case 106: // MkAtrClear Очистить список МК
		Searcher.MkAtrClear();
		break;


	case 150: //LastLineOut Выдать ссылку на последнюю линию списка
	case 151: //LastLinePop Выдать ссылку на последнюю линию списка и удалить из списка
	case 152: //LastLineDel Выдать ссылку на последнюю линию списка и удалить из ИК
		if (Load.Type == Tvoid)
			*(void**)Load.Point = (void*)ListHead->back().Load.Point;
		if (MK == 152) ICDel((void*)ListHead->back().Load.Point);
		if (MK == 151 || MK == 152) ListHead->pop_back();
		break;
	case 155: //LastLineOutMK Выдать ссылку на последнюю линию списка
	case 156: //LastLinePopMK Выдать ссылку на последнюю линию списка и удалить из списка
	case 157: //LastLineDelMk Выдать ссылку на последнюю линию списка и удалить из ИК
		if (ListHead == nullptr) break;
		MkExec(*(int*)Load.Point, { Tvoid , (void*)ListHead->back().Load.Point });
		if (MK == 157) ICDel((void*)ListHead->back().Load.Point);
		if (MK == 156 || MK == 157) ListHead->pop_back();
		break;
	case 160: // LineAdd Добавить строку
		if (ListHead == nullptr) ListHead = new vector<ip>;
		if (ListHead == nullptr)
			ListHead = new vector<ip>;
		ListHead->push_back({ Atr, Load });
		break;
	case 161: // LineCopyAdd Добавить копию строки
		if (ListHead == nullptr) ListHead = new vector<ip>;
		ListHead->push_back({ Atr, TIC, ICCopy(Load) });
		break;
	case 162: // LineCopyTreeAdd Добавить копию ОА-графа
		if (ListHead == nullptr) ListHead = new vector<ip>;
		// ....
		break;
	
	case 165: // LineExcludeMk Исключить линию списка
		if (ListHead == nullptr || ListHead->size() == 0) break;
		if(Load.Point!=nullptr && Load.Type>>1==Dint) MkExec(*(int*)Load.Point, { TIC , (void*)ListHead->back().Load.Point });
		ListHead->pop_back();
		break;
	case 166: // LineDelMk Выдать МК со ссылкой на ИК и удалить ИК
		if (ListHead == nullptr || ListHead->size() == 0) break;
		MkExec(*(int*)Load.Point, { TIC , (void*)ListHead->back().Load.Point });
		ICDel(ListHead->back().Load.Point);
		ListHead->pop_back();
		break;
	case 167: // LineIpCut Удалить последние ИП из текущей линии (в нагрузке количество удаляемых ИП, по умолчанию 1)
	case 168: // LineVarIpCut" Hint = "Удалить последние ИП c пемеменными из текущей линии (в нагрузке количество удаляемых ИП, по умолчанию 1)" }
		if (LineUk != nullptr && LineUk->Load.Type>>1==DIC && LineUk->Load.Point!=nullptr)
		{
			int n;
			if (Load.Point == nullptr || Load.Type >> 1 != Dint)
				n = 1;
			else
				n = *(int*)Load.Point;
			while (n-- && ((IC_type)LineUk->Load.Point)->size() != 0)
			{
//				if(MK==167)
//					((IC_type)LineUk->Load.Point)->back().Load.Clear();
//				else
//					((IC_type)LineUk->Load.Point)->back().Load.VarClear();
//				((IC_type)LineUk->Load.Point)->pop_back();
				((IC_type)LineUk->Load.Point)->resize(((IC_type)LineUk->Load.Point)->size() - 1);
			}
		}
		break;
	case 170: // LastAttach Конкатенация ИК к последней линии списка
	case 171: // LastCopyAttach Конкатенация копии ИК к последней линии списка
		if (ListHead != nullptr && Load.Point != nullptr)
			if (Load.Type >> 1 == DIP)
			{
				((IC_type)ListHead->back().Load.Point)->push_back(*(ip*)Load.Point);
				if (MK == 170 && Load.Type == CIP) { delete (ip*)Load.Point; Load.Point = nullptr; Load.Type = 0; }
			}
		else
			{
			if (Load.Type >> 1 == DIC)
			{
				copy(((IC_type)Load.Point)->begin(), ((IC_type)Load.Point)->end(), inserter(*((IC_type)ListHead->back().Load.Point), ((IC_type)ListHead->back().Load.Point)->end()));
				if (MK == 170 && Load.Type==CIP) { ICDel(Load.Point); Load.Point = nullptr; Load.Type = 0; };
			}
		}
		break; 
	case 172: // LastCopyGrahpAttach
		// ....
		break;
	case 175: // LineAttach Конкатенация ИК к текущей линии списка
	case 176: // LineCopyAttach Конкатенация копии ИК к текущей линии списка
		if (LineUk != nullptr && Load.Point != nullptr)
			if (Load.Type >> 1 == DIP)
			{
				((IC_type)LineUk->Load.Point)->push_back(*(ip*)Load.Point);
				if (MK == 175 && Load.Type == CIP) { delete (ip*)Load.Point; Load.Point = nullptr; Load.Type = 0; }
			}
			else
			{
				if (Load.Type >> 1 == DIC)
				{
					copy(((IC_type)Load.Point)->begin(), ((IC_type)Load.Point)->end(), inserter(*((IC_type)LineUk->Load.Point), ((IC_type)LineUk->Load.Point)->end()));
					if (MK == 175 && Load.Type == CIP) { ICDel(Load.Point); Load.Point = nullptr; Load.Type = 0; };
				}
			}
		break;
	case 177: // LineCopyGrahpAttach
			  // ....
		break;
	case 185: // LastLoadSet Записать адрес в нагрузку последней ИП последней строки
	case 195: // LineLoadSet Записать адрес в нагрузку последней ИП последней строки
	case 186: // LastLoadCopySet Записать адрес в нагрузку последней ИП последней строки
	case 196: // LineLoadCopySet Записать адрес в нагрузку последней ИП последней строки
	case 187: // LastPointLoadCopyGraphSet
	case 197: // LinePointLoadCopyGraphSet
	case 189: // LastAtrSet Установить атрибут у последней ИК последней линии 
	case 199: // LineAtrSet Установить атрибут у последней ИК последней линии 
	{	IC_type t;
		if (ListHead == nullptr || ListHead->back().Load.Point == nullptr || ListHead->back().Load.Type >> 1 != DIC) break;
		if (((IC_type)ListHead->back().Load.Point)->size() == 0) break;
		if (MK < 190)
			t = ((IC_type)ListHead->back().Load.Point);
		else if (LineUk != nullptr && LineUk->Load.Point != nullptr && LineUk->Load.Type >> 1 != DIC)
			break;
		else
			t = (IC_type)LineUk->Load.Point;
		switch (MK)
			{
			case 185:
			case 195:
				t->back().Load = Load;	break;
			case 186:
			case 196:
				if (Load.Type >> 1 != DIC)
					t->back().Load.Copy(&Load);
				else
				{
					t->back().Load.Type = Load.Type;
					t->back().Load.Point = ICCopy(Load);
				}
				break;
			case 189:
			case 199:
				if (Load.Type >> 1 == Dint)
					t->back().atr = *(int*)Load.Point;
				break;
				//	case 177: // LastPointLoadCopyGraphSet
						// ...
						//break;
				}

		break;
	}
	case 180: // LastPointVarTypeSet // Установить тип 'переменная' в последней ИП последней линии
	case 182: // LinePointVarTypeSet // Установить тип 'переменная' в последней ИП текущей линии
	case 181: // LastPointConstTypeSet // Установить тип 'константа' в последней ИП последней линии
	case 183: // LinePointConstTypeSet // Установить тип 'константа' в последней ИП текущей линии
	{	
		IC_type t;
		if (ListHead != nullptr || ListHead->size()==0) break;
		if (MK  == 180 || MK==181)
		{
			if (ListHead->back().Load.Point == nullptr || ListHead->back().Load.Type >> 1 != DIC || ((IC_type)ListHead->back().Load.Point)->size() > 0)
				break;
			t = (IC_type)ListHead->back().Load.Point;
		}
		else
		{
			if (LineUk == nullptr || LineUk->Load.Point == nullptr || LineUk->Load.Type >> 1 != DIC || ((IC_type)LineUk->Load.Point)->size()==0)
				break;
			t = (IC_type)LineUk->Load.Point;
		}
				switch (MK)
				{
				case 180: 
				case 182: t->back().Load.VarTypeSet(); break;
				case 181: 
				case 183: t->back().Load.ConstTypeSet(); break;
				}
	break;
	}
	case 200: // LineToLast Установить текущую строку на последнюю строку
		if(ListHead!=nullptr && ListHead->size()>0 && ListHead->back().Load.Point!=nullptr && ListHead->back().Load.Type>>1==DIC)
			LineUk = &ListHead->back();
		break;
	case 205: // LinePush Положить текущую линию в стек
		if (LineNum >= 0 && LineNum < ListHead->size() > 0) 
			LineStack.push_back(LineNum);
		break;
	case 206: // LineLastPush Положить последнюю линию в стек
		if (ListHead->size() > 0) LineStack.push_back(ListHead->size()-1);
		break;
	case 207: // LinePop Взять текущую линию из стека
		if (LineStack.size() && ListHead->size()> LineStack.back() && LineStack.back()>=0)
		{
			LineNum = LineStack.back();
			LineStack.pop_back();
			LineUk = ListHead->begin()._Ptr+ LineNum;
		}
		break;
	case 220: // FindOr Поиск ИЛИ
	case 221: // FindOrLastLine Поиск ИЛИ в последней строке
	case 226: // FindAnd Поиск И
	case                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              227: // FindAndLastLine Поиск И в последней строке
	case 230: // FindXor Поиск XOR
	case 231: // FindXorLastLine Поиск XOR в последней строке
	case 236: // FindAndSource Поиск И в источнике
	case 237: // FindAndSourceLastLine Поиск И в источнике в последней строке
	{
		int Count = 0; // счетчик совпадений
		LineNum = 0; // Номер первой совпадающей линии
		int LineNumFirst = -1;
		if (ListHead == nullptr)
		{
			Searcher.Template = { 0,nullptr };
			Searcher.FindOr({ 0,nullptr });
			ProgExec(FailAllProg, Bus);
			break;
		}
		auto i = ListHead->begin();
		if (MK % 2 == 1)
			i = ListHead->end() - 1;
		for (; i != ListHead->end(); i++)
		{
			LineUk = i._Ptr;
			Searcher.Template = i->Load;
			switch (MK)
			{
			case 220: // FindOr		
				Searcher.FindOr(Load);
				break;
			case 225: // FindAnd 
				Searcher.FindAnd(Load);
				break;
			case 230: // FindXor
				Searcher.FindXor(Load);
				break;
			case 235: // FindAndSource
				Searcher.FindAndSource(Load);
				break;
			}
			if (Searcher.Rez)
			{
				Count++;
				if (Count==1)
				{
					if (MK % 2 == 1)
						LineNumFirst = ListHead->size() - 1;
					else
						LineNumFirst = LineNum;

				}
				if (MultiLineMode == 0) break;
			}
			LineNum++;
		}
		if (Count)
		{
			LineUk = &ListHead->at(LineNumFirst);
			LineNum = LineNumFirst;
		}
		else
		{
			LineNum == -1;
			LineUk = nullptr;
			ProgExec(FailAllProg, Bus);
		}
	}
		break;
	case 400: // LineOutMk Выдать МК с найденной линией
		if (Load.Point != nullptr && Load.Type >> 1 == Dint)
			MkExec(*(int*)Load.Point, LineUk->Load);
		break;
	default:
		CommonMk(MK, Load);
		break;
	}
}
