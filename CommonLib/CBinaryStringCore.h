#pragma once
#include <string.h>
#include <CBinaryString.h>
class CBinaryString;
class CBinaryStringCore
{
private:
	CBinaryStringCore& operator = (CBinaryStringCore&& obj)  noexcept = default;
protected:
	friend class CBinaryString;
	virtual ~CBinaryStringCore();
	CBinaryStringCore(CBinaryStringCore& other);

public:
	CBinaryStringCore(size_t len);
	unsigned long AddRef() noexcept;
	unsigned long  Release();

protected:
	char8_t* m_pData;
	unsigned long m_RefCount;
	size_t m_DataSize;
	size_t m_BufSize;
	size_t SetDataSize(size_t len);
	size_t GetDataSize();
	size_t GetBufSize();
	size_t SetBufSize(size_t len);

};
