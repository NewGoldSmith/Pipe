#pragma once
#pragma comment(lib, "WS2_32.LIB")
#include <WinSock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <string>
#include "CByteData.h"
#include <vector>


class CTCPNetwork
{
public:
	CTCPNetwork();
	virtual ~CTCPNetwork();
	int Read(CByteData* pByteData);
	int Write(const CByteData* pByteData);
	SOCKET m_Socket;
protected:
	std::basic_string<TCHAR> CodeToString(int ErrCode);
	bool* m_pbContinue;
public:
	bool Create(const  unsigned int port, const std::string address);
	bool Listen(int backlog);
	SOCKET Accept(CTCPNetwork* pTCPNetwork);
//	SOCKET Attach(SOCKET soc);
	int Connect(const std::string peeraddress, const unsigned int peerport,bool * const pbContinue);
	void Close();
	void Shutdown(int nHow);
	SOCKET Detach();
	void Attach(const CTCPNetwork&  srcTCPNetwork );
	int IOCtl(long lcmd, u_long* argp);
};

