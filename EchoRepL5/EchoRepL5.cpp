// EchoRepL5.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

//#define _WIN32_WINNT _WIN32_WINNT_WIN10
// argument_definitions.cpp
// compile with: /EHsc
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>
#include <tchar.h>
//#include "CAsyncSocketCon.h"

using namespace std;
int main(int argc, char* argv[], char* envp[])
{
	WSADATA wsaData = { 0 };
	int iResult = 0;

	bool numberLines = false;    // Default is no line numbers.
	char8_t message[20];
	int i = 6;
	size_t nWrite = 0;

	strcpy_s((char*)message, 20, "Test\r\n");
	fwrite(message, sizeof(char8_t), 6, stdout);

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return 1;
	}
	SOCKET soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (soc == INVALID_SOCKET) fprintf(stderr, "socket error:%d\n", WSAGetLastError());

	//ノンブロックに変更
	u_long flag = 1;
	if (ioctlsocket(soc, FIONBIO, &flag) == SOCKET_ERROR) fprintf(stderr, "ioctl(NONBLOCK) error:%d", WSAGetLastError());

	//ローカルアドレス・ポート設定
	sockaddr_in local_addr = {};
	local_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.4", &local_addr.sin_addr);
	local_addr.sin_port = htons(0);
	if (bind(soc, (sockaddr*)&local_addr, sizeof(local_addr)) == SOCKET_ERROR) fprintf(stderr, "ioctl(NONBLOCK) error:%d", WSAGetLastError());
	
	//接続先アドレス・ポート設定
	sockaddr_in peer_addr = {};
	peer_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.3", &(peer_addr.sin_addr));
	peer_addr.sin_port = htons(50000);

	if (connect(soc, (struct sockaddr*)&peer_addr, sizeof(peer_addr)) == SOCKET_ERROR)
	{
		int Err = WSAGetLastError();
		if (Err != WSAEWOULDBLOCK) fprintf(stderr, "connect error:%d\n", errno);

		fd_set rmask, wmask; FD_ZERO(&rmask); FD_SET(soc, &rmask); wmask = rmask;
		struct timeval tv = { 20,0 };
		int rc = select((int)soc + 1, &rmask, &wmask, NULL, &tv);
		if (rc == SOCKET_ERROR) fprintf(stderr, "connect-select error:%d\n", WSAGetLastError());
		if (rc == 0) { int i = 0; }
		if (rc == 2) { //読み書きが同時に出来る場合
			int val;
			int len = sizeof(val);
			if (getsockopt(soc, SOL_SOCKET, SO_ERROR, (char*)&val, &len) != 0) {
				int i = 0;
			}
			else {
				//コネクト失敗
				int i = 0;
			}
		}
		char buf[1024]={};
		int slen = send(soc, buf, 0, 0);
		if (slen == SOCKET_ERROR) fprintf(stderr, "send error:%d\n", WSAGetLastError());
		int rlen = recv(soc, buf, 1024, 0);
		if (rlen == SOCKET_ERROR) fprintf(stderr, "recv error:%d\n", WSAGetLastError());
	}

    
    // If /n is passed to the .exe, display numbered listing
    // of environment variables.
    if ((argc == 2) && _stricmp(argv[1], "/n") == 0)
        numberLines = true;

    // Walk through list of strings until a NULL is encountered.
    for (int i = 0; envp[i] != NULL; ++i)
    {
        if (numberLines)
            cout << i << ": "; // Prefix with numbers if /n specified
        cout << envp[i] << "\n";
    }
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
