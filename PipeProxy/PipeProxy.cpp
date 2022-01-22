// PipeProxy.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <string>
#include <WinSock2.h>
#include <CBSSocket.h>
#include <CPipeTerm.h>
#include <CBinaryString.h>
#include <WinExeEngine.h>
#include <CommandLine.h>
#include <CShowLicenseMIT.h>
#include <CommandLineNetConnection.h>
#include "CShowHelpPipeProxy.h"
#include "CPipeProxy.h"

#include <CProtoNPCP11.h>

int main(int argc, char* argv[])
{
	using namespace std;
	using namespace PipeHelper;
	using namespace SocketHelper;
	HANDLE h = GetModuleHandle(0);

	CommandLine cmdline(argc, argv);
	CommandLineNetConnection cmdOption;
	cmdline.ParseCommand(&cmdOption);
	cmdOption.m_Cli = true;

	CShowLicenseMIT License;
	License.SetYearly("2021");
	License.SetAppName("PipeProxy");

	CShowHelpPipeProxy Help;

	if (cmdOption.m_Licence == true)
	{
		License.ShowPrompt();
		return 0;
	}
	if (cmdOption.m_Help == true)
	{
		Help.ShowPrompt();
		return 0;
	}
	if (cmdOption.m_NoOp == true)
	{
		License.ShowPrompt();
		Help.ShowPrompt();
		return 0;
	}
	if (!cmdOption.IsArgValid())
	{
		License.ShowPrompt();
		cout << "\r\nThe argument is wrong or missing.\r\nSee help with the /h option." << endl;
		return 1;
	};

	WSADATA wsaData = { 0 };
	int iResult = 0;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
	}

	CBSSocket Socket;
	Socket.Create();
	Socket.Bind(cmdOption.m_strHostAddress, cmdOption.m_HostPort);
	Socket.Connect(cmdOption.m_strPeerAddress, cmdOption.m_PeerPort,cmdOption.m_BackLog);
	if (!Socket.IsConnect())
	{
		WSACleanup();
		return 1;
	}
	CPipeProxy pxy;
	pxy.Init((CBStream*)&Socket, CNPCP11::emDeviceType::PXY, "ID01PXY");
	bool rVal{ true };
	for (;rVal;)
	{
		rVal=pxy.OneWork();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}


	WSACleanup();

}

