#include "CBinaryStringCore.h"


CBinaryStringCore::CBinaryStringCore(unsigned int len):
	m_pData(nullptr)
	,m_RefCount(0)
	,m_DataSize(0)
	,m_BufSize(0)
	,m_pDebugMark(nullptr)
{
	m_pDebugMark = (char8_t*) new char8_t[len + 2+8];
	memset(m_pDebugMark, 0, len + 2+8);
	m_pData = m_pDebugMark + 8*sizeof(char8_t);
	m_RefCount = 1;
	m_BufSize = len;
	m_DataSize = 0;
}

CBinaryStringCore::~CBinaryStringCore()
{
	delete[] m_pDebugMark;
}

unsigned int CBinaryStringCore::AddRef() noexcept
{
	m_RefCount++;
	return m_RefCount;
}


unsigned int CBinaryStringCore::Release()
{
	m_RefCount--;
	if (m_RefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_RefCount;
}
