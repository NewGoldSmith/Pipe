#pragma once
#include<Windows.h>
#include <CBinaryString.h>
//#include <CBinaryStringCore.h>

//e.g.
//#include <string>
//#include <CBStreamTerm.h>
//#include <CBWinConv.h>
//
//using namespace std;
//int main()
//{
//	using namespace CBWinConv;
//
//	CBinaryString str(u8"0123456789");
//	CBinaryString wstr;
//	CBinaryString u8str;
//	wstring stdwstr;
//	u8string stdu8str;
//	Utf8toW(str, wstr);
//	stdwstr = wstr.c_strw();
//	WtoUtf8(wstr, u8str);
//	stdu8str = u8str.c_str8();
//}
//

namespace CBWinConv
{
	int WtoUtf8(const CBinaryString& wstr, CBinaryString& utf8str);
	int Utf8toW(const CBinaryString& utf8str, CBinaryString& wstr);
};

