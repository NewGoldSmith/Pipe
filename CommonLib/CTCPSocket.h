#pragma once
#include <CBStream.h>
#include <string>
#include <WinSock2.h>
#include <CBinaryString.h>
#include <SocketHelper.h>

using namespace SocketHelper;
class CTCPCliSocket:public CBStream
{
public:
	CTCPCliSocket();
	~CTCPCliSocket();
	bool Create();
	bool Bind(std::string strHostAddr, UINT uiHostPort);
	bool Connect(std::string strPeerAddress, USHORT uPeerPort, long uiTimeOut);
	void DisConnect();
	SOCKET m_pSocket;
	virtual int Read(CBinaryString& BD);
	virtual int Write(const CBinaryString& BD);
	int GetError();
	void GetSocketStatus(SOCKET Socket,  std::string& strResult);
	int m_Err;
	bool m_fOpSuccess;
protected:
	bool m_TimeOut;
	bool Wait(long ltimeout);
};

