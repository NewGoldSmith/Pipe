// ByteDataTest.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <CBinaryString.h>
#include <CBSSocket.h>
#include <CPipeTerm.h>
#include <CSecTimer.h>
#include <iostream>
#include <functional>
#include <WinExeEngine.h>
#include <string>
#include <map>
#include <vector>
#include <CProtoNPCP11.h>
#include <CJointStream.h>

    using namespace std;
	void func(){};
int main()
{
    CPipeTerm pipeStd;
    CPipeTerm pipeChild;
    CJointStream JointStream;
    WinExeEngine engine;
	SECURITY_ATTRIBUTES saAttr = {};

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;
	HANDLE  hChild_Up_Wr, hChild_Down_Rd;
	HANDLE hChild_Down_Wr, hChild_Up_Rd;
	HANDLE hStdIn, hStdOut;
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!CreatePipe(&hChild_Up_Rd, &hChild_Up_Wr, &saAttr, 0))
		return false;
	if (!SetHandleInformation(hChild_Up_Rd, HANDLE_FLAG_INHERIT, 0))
		return false;

	if (!CreatePipe(&hChild_Down_Rd, &hChild_Down_Wr, &saAttr, 0))
		return false;
	if (!SetHandleInformation(hChild_Down_Wr, HANDLE_FLAG_INHERIT, 0))
		return false;

	if (!engine.Execute(L"WCmdCliant.exe", hChild_Down_Rd, hChild_Up_Wr, hChild_Up_Wr, STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW))
	{
		return false;
	}
	CloseHandle(hChild_Down_Rd);
	CloseHandle(hChild_Up_Wr);
	pipeStd.InitPipeWork(hStdIn, hStdOut);
	pipeChild.InitPipeWork(hChild_Up_Rd, hChild_Down_Wr);
	JointStream.SetStream(&pipeStd, &pipeChild);
	JointStream.StartWork();
	for (; engine.IsRun();)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	engine.ForcedTermination();
	JointStream.StopWork();
	pipeStd.SetEventHandler(&func);
	pipeStd.ClearEventHandler();
	pipeStd.FinPipeWork();
	pipeChild.FinPipeWork();
	CloseHandle(hChild_Down_Wr);
	CloseHandle(hChild_Up_Rd);

    
}
