#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <CBinaryString.h>
#include <CBStreamQue.h>
#include <CPConsole.h>
#include <iostream>
#include <string>
#include <CBStreamTerm.h>
#include <CBWinConv.h>

using namespace std;
using namespace CBWinConv;
#include <CPConsole.h>
int main()
{
	CBinaryString str;
	str.SetTextStringW(L"cmd.exe");
	CPConsole con;
	con.SetCommand(str);
	con.SetAccessMode(CPConsole::emAccessMode::Write);
	con.SetStdAccessMode(CPConsole::emAccessMode::No);
	con.SetInterceptMode(CPConsole::emInterceptMode::Cover);
	con.Connect();
	con.Write("ping localhost\r\n");

	while (con.IsConnect())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
