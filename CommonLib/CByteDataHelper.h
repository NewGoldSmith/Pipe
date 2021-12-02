#pragma once
#pragma comment(lib, "WS2_32.LIB")
#include <WinSock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>
#include "CByteData.h"

namespace SocketHelper
{
	//ƒNƒ‰ƒXCByteDataê—pŠÖ”
	int SockRead(const SOCKET Socket, CByteData* pByteData);
	int SockWrite(const SOCKET Socket, const CByteData* pByteData);
}