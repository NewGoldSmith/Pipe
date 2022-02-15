#include <PipeHelper.h>

int PipeHelper::StdRead(CBinaryString&bd)
{
	CBinaryString bstr(bd.GetBufSize());
	int len =_read(_fileno(stdin), bstr.GetBuffer8(), bstr.GetBufSize());
	bd.SetDataSize(len);
	bd = bstr;
	return len;
}

int PipeHelper::StdWrite(const CBinaryString& bd)
{
	int len = _write(_fileno(stdout), bd.c_strA(), bd.GetDataSize());
	return len;
}

int PipeHelper::HRead(HANDLE hPipeIn, CBinaryString& bd)
{
	DWORD dwNumberOfBytesRead;
	bd.Clear();
	BOOL rVal = ReadFile(hPipeIn, bd.GetBuffer8(), bd.GetBufSize(), &dwNumberOfBytesRead, NULL);
	if (rVal)
	{
		bd.SetDataSize(dwNumberOfBytesRead);
	}
	else {
		bd.SetDataSize(0);
		DWORD code = GetLastError();
		switch (code)
		{
		case ERROR_IO_PENDING:
			return 0;
		default:
			break;
			return -1;
		}
	}
	return dwNumberOfBytesRead;
}

int PipeHelper::HWrite(HANDLE hPipeOut, const CBinaryString& bd)
{
	DWORD dwWriten;
	BOOL rVal = WriteFile(hPipeOut,bd.GetBuffer8(), bd.GetDataSize(),&dwWriten,NULL);
	if (rVal)
	{
		return dwWriten;
	}
	else {
		DWORD code = GetLastError();
		switch (code)
		{
		case ERROR_IO_PENDING:
			return 0;
		default:
			break;
			return -1;
		}
	}
}
