#pragma once
#include <string.h>

class CByteDataAtom
{
public:
	CByteDataAtom(size_t len);
	virtual ~CByteDataAtom();
	char8_t* m_pData;
	size_t m_BufSize;
	size_t m_DataSize;
	unsigned long m_RefCount;
	unsigned long AddRef();
	unsigned long Release();
};

