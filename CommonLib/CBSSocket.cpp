#include "CBSSocket.h"

CBSSocket::CBSSocket():CBStream()
{
	using namespace SocketHelper;
	// Initialize Winsock
	m_pSocket = 0;
	bool bContinue = true;
	m_Err = 0;
	m_fOpSuccess = false;
	m_TimeOut = false;
	m_bConnected = false;
}

CBSSocket::~CBSSocket()
{
}

bool CBSSocket::Create()
{
	m_pSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_pSocket == INVALID_SOCKET)
	{
		m_Err = GetLastError();
		return false;
	}
	return true;
}

bool CBSSocket::Bind(std::string strHostAddr, UINT uiHostPort)
{
	//�z�X�g�o�C���h�ݒ�
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
	m_Err = bind(m_pSocket, (struct sockaddr*)&(addr), sizeof(addr));
	if (m_Err == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool CBSSocket::Connect( std::string strPeerAddress, USHORT uPeerPort,long lTimeOut=2)
{

	//�R�l�N�g
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
	m_Err = ::connect(m_pSocket, (struct sockaddr*)&(addr), sizeof(addr));
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
	//�m���u���b�N�ɕύX
	u_long flag = 1;
	m_Err = ::ioctlsocket(m_pSocket, FIONBIO, &flag);
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

	//�u���b�N�ɕύX
	u_long flag = 0;
	m_Err = ::ioctlsocket(m_pSocket, FIONBIO, &flag);
	if (m_Err == SOCKET_ERROR)
	{
		return;
	}
	m_Err = ::shutdown(m_pSocket, SD_BOTH);
	if (m_Err == SOCKET_ERROR)
	{
		return ;
	}
	m_Err = ::closesocket(m_pSocket);
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
	m_pSocket = 0;
	return;
}

int CBSSocket::Read(CBinaryString& BD)
{
	int Err;
	int len = SocketHelper::SockRead(m_pSocket, &BD);
	if (len == 0)
	{
		m_bConnected=false;
		if (m_pEvOnDisconnect != nullptr)
		{
			(m_pEvOnDisconnect)();
		}
	}
	else if (len== SOCKET_ERROR)
	{
		Err = GetLastError();
		switch (Err)
		{
		case WSAEWOULDBLOCK:
			break;
		case WSAENOTSOCK:
			break;
		default:
			break;
		}
	}
	return len;
}

int CBSSocket::Write(const CBinaryString& BD)
{
	int Err = 0;
	int len = SocketHelper::SockWrite(m_pSocket, &BD);
	if (len == SOCKET_ERROR)
	{
		Err = GetLastError();
		switch (Err)
		{
		case WSAEWOULDBLOCK:
			break;
		default:
			m_bConnected = false;
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
	FD_SET(m_pSocket, &rmask); 
	wmask = rmask;
	struct timeval tv = { ltimeout,0 };
	int rc = select((int)m_pSocket + 1, &rmask, &wmask, NULL, &tv);
	if (rc == SOCKET_ERROR)
	{
		Err = WSAGetLastError();
		return false;
	}
	if (rc == 0)
	{
		m_TimeOut = true;/*�^�C���A�E�g����*/
		return false;
	}
	if (rc == 2)
	{ //�ǂݏ����������ɏo����ꍇ
		int val;
		int len = sizeof(val);
		if (getsockopt(m_pSocket, SOL_SOCKET, SO_ERROR, (char*)&val, &len) != 0)
		{
			// ���Ƀf�[�^�����Ă���
			return true;
		}
		else {
			// �R�l�N�g���s
			return false;
		}
	}
	return true;
}
