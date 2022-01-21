#include "CBinaryStringCore.h"



CBinaryStringCore::CBinaryStringCore(CBinaryStringCore& other):
m_pData(nullptr)
, m_RefCount(0)
, m_DataSize(0)
, m_BufSize(0)
{
	m_pData = new char8_t[other.m_BufSize + 1];
	memcpy(m_pData, other.m_pData, other.m_BufSize + 1);
	m_RefCount = 1;
	m_BufSize = other.m_BufSize;
	m_DataSize = other.m_DataSize;
}

CBinaryStringCore::CBinaryStringCore(size_t len):
	m_pData(nullptr)
	,m_RefCount(0)
	,m_DataSize(0)
	,m_BufSize(0)
{
	m_pData = (char8_t*) new char8_t[len+1];
	memset(m_pData, 0, len + 1);
	m_RefCount = 1;
	m_BufSize = len;
	m_DataSize = 0;
}

CBinaryStringCore::~CBinaryStringCore()
{
	delete[] m_pData;
}

unsigned long CBinaryStringCore::AddRef() noexcept
{
	m_RefCount++;
	return m_RefCount;
}


size_t CBinaryStringCore::SetDataSize(size_t len)
{
	size_t tmp=m_DataSize;
	m_DataSize = len;
	return tmp;
}


size_t CBinaryStringCore::GetDataSize()
{
	return m_DataSize;
}


size_t CBinaryStringCore::GetBufSize()
{
	return m_BufSize;
}


size_t CBinaryStringCore::SetBufSize(size_t len)
{
	size_t tmp = m_BufSize;
	m_BufSize = len;
	return tmp;
}



unsigned long CBinaryStringCore::Release()
{
	m_RefCount--;
	if (m_RefCount == 0)
	{
		delete this;
		return 0;
	}
	return m_RefCount;
}
