#include "CByteDataHelper.h"

namespace CByteDataHelper {
	//�\�P�b�g����CByteData�փf�[�^��ǂݍ��ށB
	int SockRead(const SOCKET Socket, CByteData* pByteData)
	{
		int len = recv(Socket, (char*)pByteData->GetBuffer(), pByteData->GetBufSize(), 0);
		if (len == SOCKET_ERROR)
		{
			pByteData->SetDataSize(0);
			return len;
		}
		pByteData->SetDataSize(len);
		return len;
	}

	//CByteData���\�P�b�g�փf�[�^���������ށB
	int SockWrite(const SOCKET Socket, const CByteData* pByteData)
	{
		int rlen = send(Socket, (const char *)pByteData->GetBuffer(), pByteData->GetDataSize(), 0);
		return rlen;
	}
}