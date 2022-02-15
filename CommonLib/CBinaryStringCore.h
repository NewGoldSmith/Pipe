#pragma once
#include <string.h>
#include <CBinaryString.h>
//#include <CBWinConv.h>
//#include <CBWinConv.h>
class CBinaryString;

class CBinaryStringCore
{
	friend class CBinaryString;
protected:

	virtual ~CBinaryStringCore();
	CBinaryStringCore(const CBinaryStringCore& other) = delete;
	CBinaryStringCore(const CBinaryStringCore&& other) = delete;
	CBinaryStringCore& operator=(const CBinaryStringCore& otherobj) = delete;
	CBinaryStringCore& operator = (const CBinaryStringCore&& obj) = delete;
	CBinaryStringCore& operator()(const CBinaryStringCore& otherobj) = delete;
	CBinaryStringCore& operator()(const CBinaryStringCore&& otherobj) = delete;

public:
	CBinaryStringCore() = delete;
	CBinaryStringCore(unsigned int len);
	unsigned int AddRef() noexcept;
	unsigned int  Release();

protected:
	char8_t* m_pData;
	char8_t* m_pDebugMark;
	unsigned int m_RefCount;
	unsigned int m_DataSize;
	unsigned int m_BufSize;
//	unsigned int SetDataSize(unsigned int len);
//	unsigned int GetDataSize();
//	unsigned int GetBufSize();
//	unsigned int SetBufSize(unsigned int len);

};
