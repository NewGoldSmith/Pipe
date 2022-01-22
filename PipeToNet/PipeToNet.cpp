// PipeToNet.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//


#include "PipeToNet.h"

//グローバル変数
PipeToNet::CommandOp cmdOp;

int main(int argc, char* argv[], char* envp[])
{
	using namespace std;
	using namespace PipeToNet;

	CShowLicenseMIT License;
	CommandLine cmdline(argc, argv);
	CShowHelp Help;

	cmdline.ParseCommand(&cmdOp);

	if (cmdOp.m_Licence == true)
	{
		License.ShowPrompt();
		return 0;
	}
	if (cmdOp.m_Help == true)
	{
		Help.ShowPrompt();
		return 0;
	}
	if (cmdOp.m_NoOp == true)
	{
		License.ShowPrompt();
		Help.ShowPrompt();
		return 0;
	}
	if (!cmdOp.IsArgValid())
	{
		License.ShowPrompt();
		cout << "\r\nThe argument is wrong or missing.\r\nSee help with the /h option." << endl;
		return 1;
	};

    CBinaryString bd(1024);
    CStdPipe pipe;

    // Initialize Winsock
    WSADATA wsaData = { 0 };
    int iResult = 0;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        wprintf(L"WSAStartup failed: %d\n", iResult);
    }

    pipe.InitPipeWork();
    bool bContinue = true;
    CBSSocket socket;
    socket.Create();
    socket.Bind(cmdOp.m_strHostAddress , cmdOp.m_HostPort);
    socket.Connect(cmdOp.m_strPeerAddress, cmdOp.m_PeerPort, cmdOp.m_TimeOut);
    while (bContinue)
    {
        pipe.Read(bd);
        if (strcmp(bd.c_strA(), "quit\n") == 0)
        {
            bContinue = false;
            break;
        };
        socket.Write(bd);
        if (!socket.IsConnect())
        {
            bContinue = false;
        }
        bd.SetDataSize(0);
        socket.Read(bd);
        if (!socket.IsConnect())
        {
            bContinue = false;
        }
        pipe.Write(bd);
        Sleep(100);
    }
    socket.DisConnect();
    WSACleanup();
    pipe.FinPipeWork();

}


