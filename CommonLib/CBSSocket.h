#pragma once
#include <CBStream.h>
#include <string>
#include <WinSock2.h>
#include <CBinaryString.h>
#include <SocketHelper.h>

using namespace SocketHelper;
class CBSSocket:public CBStream
{
public:
	CBSSocket();
	~CBSSocket();
	bool Create();
	bool Bind(std::string strHostAddr, UINT uiHostPort);
	bool Connect(std::string strPeerAddress, USHORT uPeerPort, long uiTimeOut);
	void DisConnect();
	int Listen(int backlog=5);
	bool Accept(CBSSocket* pConnectionSocket);
	virtual int Read(CBinaryString& BD);
	virtual int Write(const CBinaryString& BD);
	int GetError();
	void GetSocketStatus(SOCKET Socket,  std::string& strResult);
protected:
	SOCKET m_Socket;
	bool m_TimeOut;
	int m_Err;
	bool m_fOpSuccess;
	bool Wait(long ltimeout);
};

