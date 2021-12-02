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
	case WSAEWOULDBLOCK:    return _T("WSAEWOULDBLOCK\r\n���\�[�X�͈ꎞ�I�Ɏg�p�ł��܂���B");
	case WSAEISCONN:		return _T("WSAEISCONN\r\n�\�P�b�g�͊��ɐڑ�����Ă��܂��B");
	case WSAEADDRINUSE:		return _T("WSAEADDRINUSE\r\n�A�h���X�͊��Ɏg�p���ł��B");
	case WSAEINVAL:			return _T("WSAEINVAL\r\n�����������ł��B");
	case WSAENOTSOCK:       return _T("WSAENOTSOCK\r\n�\�P�b�g�ȊO�ł̃\�P�b�g����ł��B");
	case WSAEADDRNOTAVAIL:  return _T("WSAEADDRNOTAVAIL\r\n�v�����ꂽ�A�h���X�����蓖�Ă邱�Ƃ��ł��܂���B");
	case WSAECONNREFUSED:   return _T("WSAECONNREFUSED\r\n�ڑ������ۂ���܂����B");
	case WSAENOTCONN:       return _T("WSAENOTCONN\r\n�\�P�b�g���ڑ�����Ă��܂���B");
	case WSAENETUNREACH:    return _T("WSAENETUNREACH\r\n�l�b�g���[�N�ɓ��B�ł��܂���B");
	case WSAECONNABORTED:   return _T("WSAENETUNREACH\r\n�\�t�g�E�F�A�ɂ���Đڑ������f����܂����B");
	case WSAECONNRESET:     return _T("WSAECONNRESET\r\n�s�A�ɂ���Đڑ������Z�b�g����܂����B");
	case WSAESHUTDOWN:      return _T("WSAESHUTDOWN\r\n�\�P�b�g�̃V���b�g�_�E����ɑ��M�ł��܂���B");
	case WSAEFAULT:			return _T("WSAEFAULT\r\n�A�h���X������������܂���B");
	default:
		_stprintf_s(buf, 1024, _T("���̑��̃G���[%d"), ErrCode);
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

//�o�C���h�ݒ�
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
	//�ڑ�����i�[����sockaddr_in�\���̐ݒ�B
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
