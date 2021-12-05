#include "CByteDataAtom.h"


CByteDataAtom::CByteDataAtom(size_t len)
{
	m_pData = (char8_t*) new char8_t[len+1];
	memset(m_pData, 0, len + 1);
	m_RefCount = 1;
	m_BufSize = len;
	m_DataSize = 0;
}

CByteDataAtom::~CByteDataAtom()
{
	delete[] m_pData;
}

unsigned long CByteDataAtom::AddRef()
{
	m_RefCount++;
	return m_RefCount;
}

unsigned long CByteDataAtom::Release()
{
	m_RefCount--;
	if (m_RefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_RefCount;
}

