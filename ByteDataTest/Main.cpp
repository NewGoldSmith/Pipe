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
#include <CJointStream3.h>
//#include <CPConsole.h>

using namespace std;
void func() {};
int main()
{
	HANDLE hIN = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOUT = GetStdHandle(STD_OUTPUT_HANDLE);
	CPipeTerm stdPipe;
	CPipeTerm stdPipe2;
	stdPipe.Connect(hIN, hOUT);
	CJointStream stream;
	stream.SetStream(&stdPipe, &stdPipe);
	CSecTimer timer;
	bool bEnd;
	timer.SetParam(&bEnd, 10);
	timer.Start();
	stream.StartWork();
	while (!bEnd)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		;
	}

	stream.StopWork();
	stdPipe.Disconnect();
	CloseHandle(hIN);
	CloseHandle(hOUT);
}