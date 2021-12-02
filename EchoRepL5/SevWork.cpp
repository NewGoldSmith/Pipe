#include "SevWork.h"

extern CommandOpEchoRepL5 cmdOp;
extern DetectEnd_EchoL5::ThreadParam TP;

namespace MainSevWork
{
	bool SevWork()
	{
		using namespace SocketHelper;

		SockPrintMessage(cmdOp.m_ShowStatus, "Server Work Start");
		CByteData CByteData(1024);
		bool bContinue = true;
		int LoopCount = 0;
		SOCKET ListenSocket;
		int Err = 0;
		std::vector<SOCKET> SevSocketArray;
		ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (ListenSocket == INVALID_SOCKET)
		{
			fprintf(stderr, "socket error:%d\n", WSAGetLastError());
			return false;
		}

		//�z�X�g�o�C���h�ݒ�
		struct sockaddr_in addr = { };
		addr.sin_family = AF_INET;
		addr.sin_port = htons(cmdOp.m_HostPort);
		Err = inet_pton(AF_INET, cmdOp.m_strHostAddress.c_str(), &(addr.sin_addr));
		if (Err != 1)
		{
			if (Err == 0)
			{
				fprintf(stderr, "socket error:Listen inet_pton return val 0\n");
				return false;
			}
			else if (Err == -1)
			{
				fprintf(stderr, "socket error:Listen inet_pton %d\n", WSAGetLastError());
				return false;
			}
		}
		Err = bind(ListenSocket, (struct sockaddr*)&(addr), sizeof(addr));
		if (Err == SOCKET_ERROR)
		{
			printf("bind err\r\n");
			SockPrintErr();
			return false;
		}

		//�m���u���b�N�ɕύX
		u_long flag = 1;
		Err = ioctlsocket(ListenSocket, FIONBIO, &flag);
		if (Err == SOCKET_ERROR)
		{
			Err = GetLastError();
			fprintf(stderr, "ioctlsocket FIONBIO error:Listen %d\n", Err);
			closesocket(ListenSocket);
			return false;
		}

		//�I�v�V�����ݒ�
		int val = 1;
		if (setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val)) == SOCKET_ERROR)
		{
			Err = GetLastError();
			switch (Err)
			{
			case WSAEWOULDBLOCK:
				break;
			default:
				fprintf_s(stderr, "setsockopt SO_REUSEADDR error\r\n");
				SockPrintErr();
				closesocket(ListenSocket);
				return false;
			}
		}
		//���b�X��
		if (listen(ListenSocket, cmdOp.m_BackLog))
		{
			Err = GetLastError();
			switch (Err)
			{
			case WSAEWOULDBLOCK:
				break;
			default:
				fprintf_s(stderr, "listen error\r\n");
				SockPrintErr();
				closesocket(ListenSocket);
				return false;
			}
		}
		ShowSocketStatus(cmdOp.m_ShowStatus, ListenSocket, "Listen Start. ");

		//���C�����[�v
		bool bExistConnected = false;//���ł��ڑ������烋�[�v�𔲂�������ɏo����B
		bool bAccepted = false;
		while (((!SevSocketArray.empty() && bContinue) || !bExistConnected) && !(TP.bEndFlag))
		{
			//�A�N�Z�v�g
			sockaddr  SevSocAddr;
			int Len = sizeof(sockaddr);
			SOCKET accSevSocket = accept(ListenSocket, &(SevSocAddr), &Len);
			if (accSevSocket == INVALID_SOCKET)
			{
				int Err = GetLastError();
				switch (Err)
				{
				case WSAEWOULDBLOCK:
					break;
				default:
					SockPrintErr();
					break;
				}
			}
			else
			{
				//�m���u���b�N�ɕύX
				u_long flag = 1;
				Err = ioctlsocket(accSevSocket, FIONBIO, &flag);
				if (Err == SOCKET_ERROR)
				{
					printf("ioctlsocket FIONBIO err\r\n");
					SockPrintErr();
					closesocket(accSevSocket);
					return false;
				}
				bAccepted = true;
			}

			//�R���e�i�ɓ����
			if (bAccepted)
			{
				bAccepted = false;
				bExistConnected = true;
				SevSocketArray.push_back(accSevSocket);
				ShowSocketStatus(cmdOp.m_ShowStatus, accSevSocket, "Accepted the connection.");
			}

			if (!SevSocketArray.empty())
			{
				//���[�h�E���C�g���[�v�@Array�v�f���ꂼ��Ɉ�񂸂��[�N�����{����
				bContinue = SevArrayWork(SevSocketArray);
			}
			if (TP.bEndFlag)
			{
				SockPrintMessage(cmdOp.m_ShowStatus, "You have entered the \"quit\" command from the command prompt.");
				bContinue = false;
			}
			Sleep(500);
			LoopCount++;
			//CPU�g�p����������
		}

		SockPrintMessage(cmdOp.m_ShowStatus, "Since there are no more connection destinations,\r\n the end work is started.");

		//�����A���C�Ƀ\�P�b�g���c���Ă�����I������������B
		size_t j = SevSocketArray.size();
		for (SOCKET SevSocket : SevSocketArray)
		{
			ShowSocketStatus(cmdOp.m_ShowStatus, SevSocket, "Socket disconnection.");

			//�u���b�N�ɕύX
			u_long flag = 0;
			Err = ioctlsocket(SevSocket, FIONBIO, &flag);
			if (Err == SOCKET_ERROR)
			{
				SockPrintErr();
				break;
			}
			Err = shutdown(SevSocket, SD_BOTH);
			Err = closesocket(SevSocket);
		}

		//�I�v�V�����ݒ胊�Z�b�g
		val = 0;
		if (setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val)) == SOCKET_ERROR)
		{
			fprintf(stderr, "setsockopt error:Listen end %d\n", WSAGetLastError());
			return false;
		}

		//�V���b�g�_�E��
		Err = shutdown(ListenSocket, SD_BOTH);
		Err = closesocket(ListenSocket);
		SockPrintMessage(cmdOp.m_ShowStatus, "Server work is finished.");
		return false;
	}



	bool SevArrayWork(std::vector<SOCKET>& SocketArray)
	{
		//�T�[�o�[�ڑ��ς݃\�P�b�g�@���ꂼ��̃\�P�b�g�Ƀ��[�h�E���C�g����B
		using namespace std;
		using namespace SocketHelper;

		CByteData CByteData(1024);
		int Err = 0;
		SOCKET SevSocket = NULL;
		size_t nSize = SocketArray.size();
		vector<SOCKET>::iterator it = SocketArray.begin();
		bool bRWLoopContinue = true;

		for (it; it < SocketArray.end(); it++)
		{
			SevSocket = *it;
			//�R���\�[���ɂ��I���R�}���h�����͂���Ă����ꍇ�A�ڑ��\�P�b�g�͑S�ĕ���B
			if (TP.bEndFlag)
			{
				SockPrintMessage(cmdOp.m_ShowStatus, "You have entered the \"quit\" command from the command prompt.");
				ShowSocketStatus(cmdOp.m_ShowStatus, SevSocket, "Socket disconnection.");
				Err = shutdown(SevSocket, SD_BOTH);
				//�u���b�N�ɕύX
				u_long flag = 0;
				Err = ioctlsocket(SevSocket, FIONBIO, &flag);
				if (Err == SOCKET_ERROR)
				{
					Err = GetLastError();
					fprintf(stderr, "ioctlsocket error:SevArrayWork %d\n", Err);
					return false;
				}
				Err = closesocket(SevSocket);
				SevSocket = -1;
				SocketArray.erase(it, it + 1);
				size_t j = SocketArray.size();
				break;
			}

			SevSocket = *it;
			int len = SockRead(SevSocket, &CByteData);
			if (len == SOCKET_ERROR)
			{
				int Err = GetLastError();
				switch (Err)
				{
				case WSAEWOULDBLOCK:
					break;
				default:
					bRWLoopContinue = false;
					SockPrintErr();
					ShowSocketStatus(cmdOp.m_ShowStatus, SevSocket, "Socket disconnection.");
					break;
				}
			}

			if (!bRWLoopContinue)
			{
				bRWLoopContinue = true;
				Err = shutdown(SevSocket, SD_BOTH);
				//�u���b�N�ɕύX
				u_long flag = 0;
				Err = ioctlsocket(SevSocket, FIONBIO, &flag);
				if (Err == SOCKET_ERROR)
				{
					Err = GetLastError();
					fprintf(stderr, "ioctlsocket error:SevArrayWork After Read %d\n", Err);
					return false;
				}

				Err = closesocket(SevSocket);
				SevSocket = -1;
				SocketArray.erase(it, it + 1);
				size_t j = SocketArray.size();
				break;
			}

			//�ǂݍ��ݒ����O�Ȃ�ΐؒf���ꂽ�Ƃ݂Ȃ��B
			if (len == 0)
			{
				bRWLoopContinue = false;
				SockPrintMessage(cmdOp.m_ShowStatus, "Disconnected from the peer.");
				ShowSocketStatus(cmdOp.m_ShowStatus, SevSocket, "Socket disconnection.");
			}

			len = SockWrite(SevSocket, &CByteData);
			if (len == SOCKET_ERROR)
			{
				int Err = GetLastError();
				switch (Err)
				{
				case WSAEWOULDBLOCK:
					break;
				default:
					SockPrintErr();
					ShowSocketStatus(cmdOp.m_ShowStatus, SevSocket, "Socket disconnection.");
					bRWLoopContinue = false;
					break;
				}
			}

			if (!bRWLoopContinue)
			{
				bRWLoopContinue = true;
				Err = shutdown(SevSocket, SD_BOTH);
				//�u���b�N�ɕύX
				u_long flag = 0;
				Err = ioctlsocket(SevSocket, FIONBIO, &flag);
				if (Err == SOCKET_ERROR)
				{
					Err = GetLastError();
					fprintf(stderr, "ioctlsocket error:SevArrayWork after write %d\n", Err);
					return false;
				}
				Err = closesocket(SevSocket);
				SevSocket = -1;
				SocketArray.erase(it, it + 1);
				size_t j = SocketArray.size();
				break;
			}
			//�ڑ���l�b�g���[�N����quit\r\n�����͂��ꂽ��I��
			if (_stricmp((const char*)CByteData.c_str(), (const char*)u8"quit\r\n") == 0)
			{
				SockPrintMessage(cmdOp.m_ShowStatus, "The peer has entered the \"quit\" command.");
				ShowSocketStatus(cmdOp.m_ShowStatus, SevSocket, "Socket disconnection.");
				Err = shutdown(SevSocket, SD_BOTH);
				//�u���b�N�ɕύX
				u_long flag = 0;
				Err = ioctlsocket(SevSocket, FIONBIO, &flag);
				if (Err == SOCKET_ERROR)
				{
					Err = GetLastError();
					fprintf(stderr, "ioctlsocket error:SevArrayWork detect quit %d\n", Err);
					return false;
				}
				Err = closesocket(SevSocket);
				SevSocket = -1;
				SocketArray.erase(it, it + 1);
				size_t j = SocketArray.size();
				break;
			}
		}
		return !(SocketArray.size() == 0);

	}
}
