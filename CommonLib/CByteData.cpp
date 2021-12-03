#include "CByteData.h"


CByteData& CByteData::operator=(const CByteData& OtherCByteData)
{
	char8_t *pTemp = new char8_t[OtherCByteData.m_BufSize+1];
	m_BufSize = OtherCByteData.m_BufSize;
	memcpy_s(pTemp,m_BufSize+1, OtherCByteData.m_pData,OtherCByteData.m_DataSize+1);
	m_DataSize = OtherCByteData.m_DataSize;
	delete [] m_pData;
	m_pData = pTemp;
	return *this;
}

CByteData& CByteData::operator+(const CByteData& ObjectCByteData)
{
	int TotalBufSize = m_BufSize + ObjectCByteData.m_BufSize;
	char8_t * pTmpBuf = new char8_t[TotalBufSize + 1];
	memcpy_s(pTmpBuf, TotalBufSize, m_pData, m_DataSize);
	memcpy_s(pTmpBuf + m_DataSize, TotalBufSize-m_DataSize, ObjectCByteData.m_pData, ObjectCByteData.m_DataSize);
	delete[] m_pData;
	m_pData = pTmpBuf;
	m_BufSize = TotalBufSize;
	m_DataSize += ObjectCByteData.m_DataSize;
	m_pData[m_DataSize] = '\0';
	return *this;
}

CByteData::operator const char8_t* ()  noexcept
{
	m_pData[m_DataSize];
	return m_pData;
}

CByteData::CByteData():
	m_BufSize(0)
	, m_pData(NULL)
	,m_DataSize(0)
{
	m_pData = new char8_t[1];
	memset(m_pData, 0, 1);
	m_pData[m_DataSize] = '\0';
}

CByteData::CByteData(const CByteData & OtherCByteData):
	m_pData(NULL)
	,m_BufSize(0)
{
	m_pData = new char8_t[OtherCByteData.m_BufSize + 1];
	m_BufSize = OtherCByteData.m_BufSize;
	memcpy_s(m_pData, m_BufSize+1, OtherCByteData.m_pData, OtherCByteData.m_BufSize+1);
	m_DataSize = OtherCByteData.m_DataSize;
}

CByteData::CByteData(int Size):
	m_pData(NULL)
	,m_BufSize(0)
	,m_DataSize(0)
{
	m_pData = new char8_t[Size+1];
	memset(m_pData, 0,Size+1);
	m_BufSize = Size;
}

CByteData::CByteData(const char8_t* const psrcData, int nSize):
	m_pData(NULL)
	,m_BufSize(0)
{
	if (m_BufSize < nSize)
	{
		delete[] m_pData;
		m_BufSize = nSize;
		m_pData = new char8_t[nSize + 1];
	}
	memcpy_s(m_pData, nSize, psrcData, nSize);
	m_DataSize = nSize;
	m_pData[m_DataSize] = '\0';
}

CByteData::~CByteData()
{
	delete [] m_pData;
	m_pData = nullptr;
}


CByteData CByteData::SetByteData(const char8_t* const pData,int size)
{
	if (size > m_BufSize)
	{
		char8_t* pTmp = new char8_t[size + 1];
		memcpy_s(pTmp, size, pData, size);
		delete[] m_pData;
		m_pData = pTmp;
		m_BufSize = size;
		m_DataSize = size;
		m_pData[m_DataSize] = '\0';
	}
	else
	{
		memcpy_s(m_pData, size, pData, size);
		m_DataSize = size;
		m_pData[m_DataSize] = '\0';
	}
	return *this;
}


const char8_t* CByteData::c_str() const
{
	return m_pData;
}


CByteData& CByteData::SetBufReSize(const int size)
{
	char8_t *pTmp = new char8_t[size+1];
	memcpy_s(pTmp, size, m_pData, __min(size,m_BufSize));
	delete[] m_pData;
	m_pData = pTmp;
	m_BufSize = size;
	m_DataSize = __min(size, m_DataSize);
	m_pData[m_DataSize] = '\0';
	return *this;
}


int CByteData::GetBufSize()const
{
	return m_BufSize;
}


CByteData& CByteData::SetDataSize(const int size)
{
	m_DataSize = size;
	return *this;
}

int CByteData::GetDataSize()const
{
	return m_DataSize;
}

char8_t*  CByteData::GetBuffer()
{
	return m_pData;
}
