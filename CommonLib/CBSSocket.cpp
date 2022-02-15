#include "CBSSocket.h"

CBSSocket::CBSSocket():CBStream()
	,m_Socket(0)
	,m_Err(0)
	,m_fOpSuccess(false)
	,m_TimeOut(false)
{
	using namespace SocketHelper;
}

CBSSocket::~CBSSocket()
{
}

bool CBSSocket::Create()
{
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		m_Err = GetLastError();
		m_Socket = 0;
		return false;
	}
	return true;
}

bool CBSSocket::Bind(std::string strHostAddr, UINT uiHostPort)
{
	//ホストバインド設定
	struct sockaddr_in addr = { };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(uiHostPort);
	m_Err = inet_pton(AF_INET, strHostAddr.c_str(), &(addr.sin_addr));
	if (m_Err != 1)
	{
		if (m_Err == 0)
		{
			fprintf(stderr, "socket error:Listen inet_pton return val 0\n");
			m_fOpSuccess = false;
		}
		else if (m_Err == -1)
		{
			
			m_fOpSuccess = false;
		}
	}
	m_Err = bind(m_Socket, (struct sockaddr*)&(addr), sizeof(addr));
	if (m_Err == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool CBSSocket::Connect( std::string strPeerAddress, USHORT uPeerPort,long lTimeOut=2)
{

	//コネクト
	struct sockaddr_in addr = { };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(uPeerPort);
	m_Err = ::inet_pton(AF_INET, strPeerAddress.c_str(), &(addr.sin_addr.S_un.S_addr));
	if (m_Err != 1)
	{
		if (m_Err == 0)
		{
			return false;
		}
		else if (m_Err == -1)
		{
			return false;
		}
	}
	m_Err = ::connect(m_Socket, (struct sockaddr*)&(addr), sizeof(addr));
	if (m_Err == SOCKET_ERROR)
	{
		bool result = false;
		m_Err = GetLastError();
		switch (m_Err)
		{
		case WSAEWOULDBLOCK:
			//str = "WSAEWOULDBLOCK:A non-blocking socket operation could not be completed immediately.\r\n";
//			m_bConnected=Wait(lTimeOut);
			return false;
			break;
		case WSAENOTCONN:
			return false;
			break;
		case WSAEADDRINUSE:
			return false;
			break;
		case WSAECONNRESET:
			return false;
			break;
		case WSAECONNREFUSED:
			return false;
		default:
			return false;
			break;
		}
	}
	//ノンブロックに変更
	u_long flag = 1;
	m_Err = ::ioctlsocket(m_Socket, FIONBIO, &flag);
	if (m_Err == SOCKET_ERROR)
	{
		return false;
	}

	m_bConnected = true;
	return true;
}

void CBSSocket::DisConnect()
{
	m_bConnected = false;

	//ブロックに変更
	u_long flag = 0;
	m_Err = ::ioctlsocket(m_Socket, FIONBIO, &flag);
	if (m_Err == SOCKET_ERROR)
	{
		;
	}
	m_Err = ::shutdown(m_Socket, SD_BOTH);
	if (m_Err == SOCKET_ERROR)
	{
		 ;
	}
	m_Err = ::closesocket(m_Socket);
	if (m_Err == SOCKET_ERROR)
	{
		m_Err = GetLastError();
		switch (m_Err)
		{
		case WSAENETDOWN:
			break;
		case WSAENOTSOCK:
			break;
		case WSAEWOULDBLOCK:
			break;
		default:
			break;
		}
	}
	m_Socket = 0;
	return;
}

int CBSSocket::Listen(int backlog)
{
	//ノンブロックに変更
	u_long flag = 1;
	m_Err = ioctlsocket(m_Socket, FIONBIO, &flag);
	if (m_Err == SOCKET_ERROR)
	{
		closesocket(m_Socket);
		m_Socket = 0;
		m_bConnected = false;
		return false;
	}

	if (listen(m_Socket, backlog) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool CBSSocket::Accept(CBSSocket* pConnectionSocket)
{
	if (pConnectionSocket->m_Socket != 0)
	{
		return false;
	}
	//アクセプト
	sockaddr  SevSocAddr;
	int Len = sizeof(sockaddr);
	pConnectionSocket->m_Socket = accept(m_Socket, &(SevSocAddr), &Len);
	if (pConnectionSocket->m_Socket == INVALID_SOCKET)
	{
		m_Err = GetLastError();
		switch (m_Err)
		{
		case WSAEWOULDBLOCK:
			break;
		default:
			pConnectionSocket->m_Socket = 0;
			return false;
		}
	}
	pConnectionSocket->m_bConnected = true;
	//ノンブロックに変更
	u_long flag = 1;
	m_Err = ioctlsocket(pConnectionSocket->m_Socket, FIONBIO, &flag);
	if (m_Err == SOCKET_ERROR)
	{
		closesocket(pConnectionSocket->m_Socket);
		pConnectionSocket->m_Socket = 0;
		pConnectionSocket->m_bConnected = false;
		return false;
	}
	return true;
}

int CBSSocket::Read(CBinaryString& BD)
{
	int len = SocketHelper::SockRead(m_Socket, &BD);
	if (len == 0)
	{
		m_bConnected = false;
		closesocket(m_Socket);
		m_Socket = 0;
		if (m_pEvOnDisconnect != nullptr)
		{
			(m_pEvOnDisconnect)();
		}
	}
	else if (len == SOCKET_ERROR)
	{
		m_Err = GetLastError();
		switch (m_Err)
		{
		case WSAEWOULDBLOCK:
			break;
		case WSAENOTSOCK:
			break;
		default:
			m_bConnected = false;
			closesocket(m_Socket);
			m_Socket = 0;
			if (m_pEvOnDisconnect != nullptr)
			{
				(m_pEvOnDisconnect)();
			}
		}
	}
	return len;
}

int CBSSocket::Write(const CBinaryString& BD)
{
	int len = SocketHelper::SockWrite(m_Socket, &BD);
	if (len == SOCKET_ERROR)
	{
		m_Err = GetLastError();
		switch (m_Err)
		{
		case WSAEWOULDBLOCK:
			break;
		default:
			m_bConnected = false;
			closesocket(m_Socket);
			m_Socket = 0;
			if (m_pEvOnDisconnect != nullptr)
			{
				(m_pEvOnDisconnect)();
			}
		}
	}
	return len;
}

int CBSSocket::GetError()
{
	return m_Err;
}

void CBSSocket::GetSocketStatus(SOCKET Socket, std::string& strResult)
{
		int len = sizeof(sockaddr_in);
		u_short HostPort = 0;
		sockaddr_in addr = {};
		getsockname(Socket, (struct sockaddr*)&addr, &len);
		HostPort = ::ntohs(addr.sin_port);
		char strHostAddr[16]{};
		inet_ntop(AF_INET, (struct in_addr*)&addr.sin_addr, strHostAddr, 16);

		addr = {};
		::getpeername(Socket, (struct sockaddr*)&addr, &len);
		u_short PeerPort = ::ntohs(addr.sin_port);
		char strPeerAddr[16]{};
		inet_ntop(AF_INET, (struct in_addr*)&addr.sin_addr, strPeerAddr, 16);
		if (addr.sin_family == 0)
		{
			strcpy_s(strPeerAddr, "Not set.");
		}
		char buf[256];
		sprintf_s(buf, " Host %s:%d Peer %s:%d HANDLE %lld\r\n", strHostAddr, HostPort, strPeerAddr, PeerPort, Socket);
		strResult=buf;
}


bool CBSSocket::Wait(long ltimeout)
{
	int Err = 0;
	fd_set rmask = {}, wmask = {};
	FD_ZERO(&rmask);
	FD_SET(m_Socket, &rmask); 
	wmask = rmask;
	struct timeval tv = { ltimeout,0 };
	int rc = select((int)m_Socket + 1, &rmask, &wmask, NULL, &tv);
	if (rc == SOCKET_ERROR)
	{
		Err = WSAGetLastError();
		return false;
	}
	if (rc == 0)
	{
		m_TimeOut = true;/*タイムアウト処理*/
		return false;
	}
	if (rc == 2)
	{ //読み書きが同時に出来る場合
		int val;
		int len = sizeof(val);
		if (getsockopt(m_Socket, SOL_SOCKET, SO_ERROR, (char*)&val, &len) != 0)
		{
			// 既にデータが来ている
			return true;
		}
		else {
			// コネクト失敗
			return false;
		}
	}
	return true;
}
