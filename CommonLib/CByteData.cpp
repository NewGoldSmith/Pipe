#include "CByteData.h"


CByteData& CByteData::operator=(const CByteData& OtherCByteData)
{
	CByteDataAtom* ptmp;
	ptmp = OtherCByteData.m_pBDA;
	ptmp->AddRef();
	m_pBDA->Release();
	m_pBDA = ptmp;
	return *this;
}

CByteData CByteData::operator+( CByteData ObjectCByteData)const
{
	size_t TotalBufSize = m_pBDA->m_BufSize + ObjectCByteData.m_pBDA->m_BufSize;
	size_t TotalDataSize = m_pBDA->m_DataSize + ObjectCByteData.m_pBDA->m_DataSize;
	CByteDataAtom* pTmpBDA = new CByteDataAtom(TotalBufSize);
	memcpy_s(pTmpBDA->m_pData, TotalBufSize, m_pBDA->m_pData, m_pBDA->m_DataSize);
	memcpy_s(pTmpBDA->m_pData + m_pBDA->m_DataSize, TotalBufSize- m_pBDA->m_DataSize, ObjectCByteData.m_pBDA->m_pData, ObjectCByteData.m_pBDA->m_DataSize);
	pTmpBDA->m_pData[TotalDataSize ] = '\0';
	pTmpBDA->m_DataSize = TotalDataSize;
	CByteData tmpByteData;
	tmpByteData.m_pBDA->Release();
	tmpByteData.m_pBDA = pTmpBDA;
	return tmpByteData;
}

CByteData::operator const char8_t* ()  noexcept
{
	return m_pBDA->m_pData;
}


CByteData::CByteData(const CByteData & OtherCByteData)
{
	m_pBDA = OtherCByteData.m_pBDA;
	m_pBDA->AddRef();
}

CByteData::CByteData(size_t Size)

{
	m_pBDA = new CByteDataAtom(Size);
	m_pBDA->m_BufSize = Size;
	memset(m_pBDA->m_pData, 0,Size+1);
	m_pBDA->m_DataSize = 0;
}

CByteData::CByteData(const char8_t* const psrcData, size_t nSize)
{
	m_pBDA = new CByteDataAtom(nSize+1);
	memcpy_s(m_pBDA->m_pData, nSize, psrcData, nSize);
	m_pBDA->m_pData[nSize] = '\0';
	m_pBDA->m_DataSize = nSize;
}

CByteData::~CByteData()
{
	m_pBDA->Release();
	m_pBDA = nullptr;
}


CByteData CByteData::SetByteData(const char8_t* const pData,size_t size)
{
	CByteDataAtom *pTempBDA = new CByteDataAtom(__max(size,m_pBDA->m_BufSize));
	memcpy(pTempBDA->m_pData, pData, size);
	pTempBDA->m_pData[size] = '\0';
	pTempBDA->m_DataSize = size;
	m_pBDA->Release();
	m_pBDA = pTempBDA;
	return *this;
}


const char8_t* CByteData::c_str() const
{
	return m_pBDA->m_pData;
}


CByteData& CByteData::SetBufReSize(const size_t size)
{
	CByteDataAtom *pTmp = new CByteDataAtom(size);
	memcpy_s(pTmp->m_pData, size, m_pBDA->m_pData, __min(size,m_pBDA->m_BufSize));
	pTmp->m_BufSize = size;
	pTmp->m_DataSize = __min(size, m_pBDA->m_DataSize);
	pTmp->m_pData[pTmp->m_DataSize] = '\0';
	m_pBDA->Release();
	m_pBDA = pTmp;
	return *this;
}


size_t CByteData::GetBufSize()const
{
	return m_pBDA->m_BufSize;
}


CByteData& CByteData::SetDataSize(const size_t size)
{
	m_pBDA->m_DataSize=size;
	return *this;
}

size_t CByteData::GetDataSize()const
{
	return m_pBDA->m_DataSize;
}

char8_t*  CByteData::GetBuffer()
{
	return m_pBDA->m_pData;
}
