// EchoRepL5.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//


#include "EchoRepL5.h"


//グローバル変数
CommandOpEchoRepL5 cmdOp;
DetectEnd_EchoL5::ThreadParam TP;

//メイン関数
int main(int argc, char* argv[], char* envp[])
{
	using namespace std;
	using namespace DetectEnd_EchoL5;

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

	//コンソール終了コマンド検出スレッド起動
	std::thread ConsoleDetectEnd(StdPipeThread, &TP);

	WSADATA wsaData = { 0 };
	int iResult = 0;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
	}

//モード選択
	if (cmdOp.m_Cli == true)
	{
		MainCliWork::CliWork();
	}
	else
	{
		MainSevWork::SevWork();
	}

	WSACleanup();

	//コンソール入力待ちのスレッドを終了。
	TP.bForcedTermination = true;
	HANDLE hIN = GetStdHandle(STD_INPUT_HANDLE);
	CancelIoEx(hIN, NULL);
	_flushall();
	//スレッドが終了するまで待ち。
	ConsoleDetectEnd.join();
}


// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
