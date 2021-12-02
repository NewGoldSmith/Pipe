#include "TCPNetwork.h"

CTCPNetwork::CTCPNetwork():
	m_pbContinue( nullptr)
	,m_Socket(-1)
{
}

CTCPNetwork::~CTCPNetwork()
{
	if (m_Socket != -1)
	{
		closesocket(m_Socket);
	}
}


int CTCPNetwork::Read(CByteData* pByteData)
{
	int len= recv(m_Socket, (char*)pByteData->GetBuffer(), pByteData->GetBufSize(), 0);
	if (len == SOCKET_ERROR)
	{
		pByteData->SetDataSize(0);
		return len;
	}
	pByteData->SetDataSize(len);
	return len;
}


int CTCPNetwork::Write(const CByteData  * pByteData)
{
	int rlen = send(m_Socket, (const char* )pByteData->GetBuffer(), pByteData->GetDataSize(), 0);
	return rlen;
}

std::basic_string<TCHAR> CTCPNetwork::CodeToString(int ErrCode)
{
	TCHAR buf[1024];
	switch (ErrCode) {
	case WSAEWOULDBLOCK:    return _T("WSAEWOULDBLOCK\r\nリソースは一時的に使用できません。");
	case WSAEISCONN:		return _T("WSAEISCONN\r\nソケットは既に接続されています。");
	case WSAEADDRINUSE:		return _T("WSAEADDRINUSE\r\nアドレスは既に使用中です。");
	case WSAEINVAL:			return _T("WSAEINVAL\r\n引数が無効です。");
	case WSAENOTSOCK:       return _T("WSAENOTSOCK\r\nソケット以外でのソケット操作です。");
	case WSAEADDRNOTAVAIL:  return _T("WSAEADDRNOTAVAIL\r\n要求されたアドレスを割り当てることができません。");
	case WSAECONNREFUSED:   return _T("WSAECONNREFUSED\r\n接続が拒否されました。");
	case WSAENOTCONN:       return _T("WSAENOTCONN\r\nソケットが接続されていません。");
	case WSAENETUNREACH:    return _T("WSAENETUNREACH\r\nネットワークに到達できません。");
	case WSAECONNABORTED:   return _T("WSAENETUNREACH\r\nソフトウェアによって接続が中断されました。");
	case WSAECONNRESET:     return _T("WSAECONNRESET\r\nピアによって接続がリセットされました。");
	case WSAESHUTDOWN:      return _T("WSAESHUTDOWN\r\nソケットのシャットダウン後に送信できません。");
	case WSAEFAULT:			return _T("WSAEFAULT\r\nアドレスが正しくありません。");
	default:
		_stprintf_s(buf, 1024, _T("その他のエラー%d"), ErrCode);
		return std::basic_string<TCHAR> (buf);
	}
	return std::basic_string<TCHAR>(_T(""));
}



bool CTCPNetwork::Create(const  unsigned int port, const std::string address)
{
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		fprintf(stderr, "socket error:%d\n", WSAGetLastError());
		return false;
	}

//バインド設定
	struct sockaddr_in addr = { };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	int err =inet_pton(AF_INET,address.c_str(),&(addr.sin_addr));
	if (err != 1)
	{
		if (err == 0)
		{
			return false;
		}
		else if (err == -1)
		{
			fprintf(stderr, "socket error:%d\n", WSAGetLastError());
			return false;
		}
	}
	if (bind(m_Socket, (struct sockaddr*) &( addr), sizeof(addr)) == SOCKET_ERROR)
	{
		fprintf(stderr, "bind error:%d\n", WSAGetLastError());
		return false;
	}


	return true;
}


bool CTCPNetwork::Listen(int backlog)
{
	if (listen(m_Socket, backlog) == SOCKET_ERROR)
	{
		fprintf(stderr, "listen error:%d\n", WSAGetLastError());
		return false;
	}
	return true;
}


SOCKET CTCPNetwork::Accept(CTCPNetwork* pTCPNetwork)
{
		SOCKET soc = accept(m_Socket, NULL, NULL);
		pTCPNetwork->m_Socket = soc;
		return soc;
}


int CTCPNetwork::Connect(const std::string address, const unsigned int port, bool* const pbContinue)
{
	m_pbContinue = pbContinue;
	*m_pbContinue = true;
	//接続先を格納するsockaddr_in構造体設定。
	struct sockaddr_in addr = { };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	int err = inet_pton(AF_INET, address.c_str(), &(addr.sin_addr));
	if (err != 1)
	{
		if (err == 0)
		{
			*m_pbContinue = false;
			return false;
		}
		else if (err == -1)
		{
			fprintf(stderr, "socket error:%d\n", WSAGetLastError());
			*m_pbContinue = false;
			return false;
		}
	}
	bool loop_continue = true;
	int nloop = 0;
	do
	{
		nloop++;
		if (connect(m_Socket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			int Err = WSAGetLastError();
			switch (Err)
			{
			case WSAEWOULDBLOCK:
				Err = Err;
				break;
			case WSAEISCONN:
				loop_continue = false;
				*pbContinue = true;
				break;
			case WSAENOTCONN:
				Err = Err;
				break;
			default:
				fprintf(stderr, "connect error:%d\n", errno);
				*m_pbContinue = false;
				return false;

			}
		}
		else
		{
			loop_continue = false;
			break;
		}
		if (loop_continue == false) {
			break;
		}
	} while (loop_continue && nloop < 1);
	return true;
}



void CTCPNetwork::Close()
{
	if (m_Socket != -1)
	{
		closesocket(m_Socket);
		m_Socket = -1;
	}
}


void CTCPNetwork::Shutdown(int nHow)
{
	shutdown(m_Socket, nHow);
}

SOCKET CTCPNetwork::Detach()
{
	SOCKET tmpSocket = m_Socket;
	m_Socket = -1;
	m_pbContinue = nullptr;
	return tmpSocket;
}


void CTCPNetwork::Attach(const CTCPNetwork&  srcTCPNetwork)
{
	m_pbContinue = srcTCPNetwork.m_pbContinue;
	m_Socket = srcTCPNetwork.m_Socket;
}


int CTCPNetwork::IOCtl(long lcmd, u_long* argp)
{
	return ::ioctlsocket(m_Socket, lcmd, argp);
}
