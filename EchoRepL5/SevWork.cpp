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

		//ホストバインド設定
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

		//ノンブロックに変更
		u_long flag = 1;
		Err = ioctlsocket(ListenSocket, FIONBIO, &flag);
		if (Err == SOCKET_ERROR)
		{
			Err = GetLastError();
			fprintf(stderr, "ioctlsocket FIONBIO error:Listen %d\n", Err);
			closesocket(ListenSocket);
			return false;
		}

		//オプション設定
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
		//リッスン
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

		//メインループ
		bool bExistConnected = false;//一回でも接続したらループを抜ける条件に出来る。
		bool bAccepted = false;
		while (((!SevSocketArray.empty() && bContinue) || !bExistConnected) && !(TP.bEndFlag))
		{
			//アクセプト
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
				//ノンブロックに変更
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

			//コンテナに入れる
			if (bAccepted)
			{
				bAccepted = false;
				bExistConnected = true;
				SevSocketArray.push_back(accSevSocket);
				ShowSocketStatus(cmdOp.m_ShowStatus, accSevSocket, "Accepted the connection.");
			}

			if (!SevSocketArray.empty())
			{
				//リード・ライトループ　Array要素それぞれに一回ずつワークを実施する
				bContinue = SevArrayWork(SevSocketArray);
			}
			if (TP.bEndFlag)
			{
				SockPrintMessage(cmdOp.m_ShowStatus, "You have entered the \"quit\" command from the command prompt.");
				bContinue = false;
			}
			Sleep(500);
			LoopCount++;
			//CPU使用率を下げる
		}

		SockPrintMessage(cmdOp.m_ShowStatus, "Since there are no more connection destinations,\r\n the end work is started.");

		//もしアレイにソケットが残っていたら終了処理をする。
		size_t j = SevSocketArray.size();
		for (SOCKET SevSocket : SevSocketArray)
		{
			ShowSocketStatus(cmdOp.m_ShowStatus, SevSocket, "Socket disconnection.");

			//ブロックに変更
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

		//オプション設定リセット
		val = 0;
		if (setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val)) == SOCKET_ERROR)
		{
			fprintf(stderr, "setsockopt error:Listen end %d\n", WSAGetLastError());
			return false;
		}

		//シャットダウン
		Err = shutdown(ListenSocket, SD_BOTH);
		Err = closesocket(ListenSocket);
		SockPrintMessage(cmdOp.m_ShowStatus, "Server work is finished.");
		return false;
	}



	bool SevArrayWork(std::vector<SOCKET>& SocketArray)
	{
		//サーバー接続済みソケット　それぞれのソケットにリード・ライトする。
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
			//コンソールにより終了コマンドが入力されていた場合、接続ソケットは全て閉じる。
			if (TP.bEndFlag)
			{
				SockPrintMessage(cmdOp.m_ShowStatus, "You have entered the \"quit\" command from the command prompt.");
				ShowSocketStatus(cmdOp.m_ShowStatus, SevSocket, "Socket disconnection.");
				Err = shutdown(SevSocket, SD_BOTH);
				//ブロックに変更
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
				//ブロックに変更
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

			//読み込み長さ０ならば切断されたとみなす。
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
				//ブロックに変更
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
			//接続先ネットワークからquit\r\nが入力されたら終了
			if (_stricmp((const char*)CByteData.c_str(), (const char*)u8"quit\r\n") == 0)
			{
				SockPrintMessage(cmdOp.m_ShowStatus, "The peer has entered the \"quit\" command.");
				ShowSocketStatus(cmdOp.m_ShowStatus, SevSocket, "Socket disconnection.");
				Err = shutdown(SevSocket, SD_BOTH);
				//ブロックに変更
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
