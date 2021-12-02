#pragma once
#include <WinSock2.h>
#include <string>
#include <tchar.h>
#include <ws2tcpip.h>
#include <CByteData.h>
namespace SocketHelper
{
	void SockPrintErr();
	void ShowSocketStatus(bool show, SOCKET Socket, const std::string& strPrefix);
	void SockPrintMessage(bool show, const std::string& strMessage);
	//ÉNÉâÉXCByteDataêÍópä÷êî
	int SockRead(const SOCKET Socket, CByteData* pByteData);
	int SockWrite(const SOCKET Socket, const CByteData* pByteData);

}