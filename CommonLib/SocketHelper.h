#pragma once
//#pragma comment(lib, "WS2_32.LIB")
#include <WinSock2.h>
#include <string>
#include <tchar.h>
#include <ws2tcpip.h>
#include <CBinaryString.h>
namespace SocketHelper
{
	void SockPrintErr();
	void ShowSocketStatus(bool show, SOCKET Socket, const std::string& strPrefix);
	void SockPrintMessage(bool show, const std::string& strMessage);
	//クラスCByteData専用関数
	//SockRead
	// エラーが発生しない場合、戻り値は読み込みバイト数であり、
	//ソケットが正常に閉じられている時は０。
	//それ以外は、SOCKET＿ERRORが返され、WSAGetLastError関数で
	//特定のエラーコードを取得する事ができる。
	//SockWrite
	// エラーが発生しない場合、 sendは送信された合計バイト数
	// を返します。
	//これは、 lenパラメーターで送信が要求された数よりも少ない場合があ
	//ります。それ以外の場合は、SOCKET_ERRORの値が返され、
	//WSAGetLastErrorを呼び出すことで特定のエラーコードを取得できます 。
	int SockRead(const SOCKET Socket, CBinaryString* pByteData);
	int SockWrite(const SOCKET Socket, const CBinaryString* pByteData);

}