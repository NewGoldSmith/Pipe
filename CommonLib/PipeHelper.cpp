#include <PipeHelper.h>

int PipeHelper::StdRead(CBinaryString&bd)
{
	int len =_read(_fileno(stdin),bd.GetBuffer8(), bd.GetBufSize());
	bd.SetDataSize(len);
	return len;
}

int PipeHelper::StdWrite(const CBinaryString& bd)
{
	int len = _write(_fileno(stdout), bd.c_strA(), bd.GetDataSize());
	return len;
}

int PipeHelper::HRead(HANDLE hPipeIn, CBinaryString&bd)
{
	int len = _read(_open_osfhandle((intptr_t)hPipeIn, _O_RDONLY), bd.GetBuffer8(), bd.GetBufSize());
	if (len == -1)
	{
		bd.SetDataSize(0);
	}
	else {
		bd.SetDataSize(len);
	}
	return len;
}

int PipeHelper::HWrite(HANDLE hPipeOut,const CBinaryString&bd)
{
	int len = _write(_open_osfhandle((intptr_t)hPipeOut,  0), bd.c_strA(), bd.GetDataSize());
	return len;
}
