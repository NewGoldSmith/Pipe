#include <CBWinConv.h>


int CBWinConv::WtoUtf8(const CBinaryString& wstr, CBinaryString& utf8str)
{
	utf8str.Detach();
	int mbsize=WideCharToMultiByte(CP_UTF8, 0, wstr.GetBufferW(), wstr.GetDataSizeW(), utf8str.GetBufferA(), 0, NULL, NULL);

	if(mbsize> utf8str.GetBufSize())
	{
		utf8str.SetBufReSize(mbsize);
	}
	else
	{
		utf8str.Clear();
	}

	utf8str.SetDataSize(WideCharToMultiByte(CP_UTF8, 0, wstr.GetBufferW(), wstr.GetDataSizeW(), utf8str.GetBufferA(), utf8str.GetBufSize(), NULL, NULL));
	return utf8str.GetDataSize();
}

int CBWinConv::Utf8toW(const CBinaryString& utf8str, CBinaryString& wstr)
{
	wstr.Detach();
	int wsize = MultiByteToWideChar(CP_UTF8, 0, utf8str.GetBufferA(), utf8str.GetDataSize(), wstr.GetBufferW(), 0);

	if (wsize > wstr.GetBufSizeW())
	{
		wstr.SetBufReSizeW(wsize);
	}
	else
	{
		wstr.Clear();
	}
	wstr.SetDataSizeW(MultiByteToWideChar(CP_UTF8, 0, utf8str.GetBufferA(), utf8str.GetDataSize(), wstr.GetBufferW(), wstr.GetBufSizeW()));
	return wstr.GetDataSizeW();
}

