#include <SocketHelper.h>

namespace SocketHelper
{
	void SockPrintErr()
	{

        DWORD errorcode = GetLastError();
		std::basic_string<char> str;
		switch (errorcode)
		{
		case WSAEWOULDBLOCK:
			return;
			//str = "WSAEWOULDBLOCK:A non-blocking socket operation could not be completed immediately.\r\n";
			break;
		case WSAENOTCONN:
			str = "A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.";
			break;
		case WSAEADDRINUSE:
			str = "Only one usage of each socket address (protocol/network address/port) is normally permitted.";
			break;
		case WSAECONNRESET:
			str = "An existing connection was forcibly closed by the remote host.";
			break;
		default:
			str = "OtherErr";
		}
		fprintf_s(stderr, "Err=%d %s\r\n",errorcode,str.c_str() );
	}

	void ShowSocketStatus(bool show, SOCKET Socket, const std::string& strPrefix)
	{
		if (show)
		{
			int len = sizeof(sockaddr_in);
			u_short HostPort = 0;
			sockaddr_in addr = {};
			getsockname(Socket, (struct sockaddr*)&addr, &len);
			HostPort = ntohs(addr.sin_port);
			char strHostAddr[16]{};
			::inet_ntop(AF_INET,(struct in_addr*)&addr.sin_addr, strHostAddr,16);

			addr = {};
			getpeername(Socket, (struct sockaddr*)&addr, &len);
			u_short PeerPort = ntohs(addr.sin_port);
			char strPeerAddr[16]{};
			inet_ntop(AF_INET, (struct in_addr*)&addr.sin_addr, strPeerAddr, 16);
			if (addr.sin_family == 0)
			{
				strcpy_s(strPeerAddr, "Not set.");
			}
			printf("%s Host %s:%d Peer %s:%d HANDLE %lld\r\n", strPrefix.c_str(), strHostAddr, HostPort, strPeerAddr, PeerPort, Socket);
		}
	}

	void SockPrintMessage(bool show, const std::string& strMessage)
	{
		if (show)
		{
			printf("%s\r\n", strMessage.c_str());
		}
	}

	//ソケットからCByteDataへデータを読み込む。
	int SockRead(const SOCKET Socket, CBinaryString* pByteData)
	{
		int len = recv(Socket, (char*)pByteData->GetBuffer8(), pByteData->GetBufSize(), 0);
		if (len == SOCKET_ERROR)
		{
			pByteData->SetDataSize(0);
			return len;
		}
		pByteData->SetDataSize(len);
		return len;
	}

	//CByteDataをソケットへデータを書き込む。
	int SockWrite(const SOCKET Socket, const CBinaryString* pByteData)
	{
		int rlen = send(Socket, (char*)pByteData->c_strA(), pByteData->GetDataSize(), 0);
		return rlen;
	}

}	
