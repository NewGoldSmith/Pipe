//Copyright (c) 2020 2021, Gold Smith
//Released under the MIT license
//https@://opensource.org/licenses/mit-license.php

#include "CBinaryString.h"



CBinaryString& CBinaryString::operator=(const CBinaryString &OtherCByteData)
{
	OtherCByteData.m_pBDA->AddRef();
	m_pBDA->Release();
	m_pBDA = OtherCByteData.m_pBDA;
	return *this;
}

CBinaryString& CBinaryString::operator=(const char* strSz)
{
	SetTextStringA(strSz);
	return *this;
}

CBinaryString& CBinaryString::operator=(const std::string str)
{
	SetTextStringA(str.c_str());
	return *this;
}

CBinaryString& CBinaryString::operator()(const CBinaryString& OtherCByteData)
{
	OtherCByteData.m_pBDA->AddRef();
	m_pBDA->Release();
	m_pBDA = OtherCByteData.m_pBDA;
	return *this;
}

CBinaryString& CBinaryString::operator()(const char* pData)
{
	SetBinaryString8((char8_t*)pData, strlen(pData));
	return *this;
}

CBinaryString& CBinaryString::operator+=(const CBinaryString& OtherCByteData)
{
	*this =  *this + OtherCByteData;
	return *this;
}

CBinaryString& CBinaryString::operator+=(const char* str)
{
	CBinaryString bstr(str);
	*this += bstr;
	return *this;
}

CBinaryString CBinaryString::operator+(const char* str2)const
{
	CBinaryString bstr2(str2);
	CBinaryString tmp = *this + bstr2;
	return tmp;
}

bool CBinaryString::operator==(const CBinaryString& OtherObj)const
{
	size_t size1 = GetDataSize();
	size_t size2 = OtherObj.GetDataSize();
	if (size1 != size2)
	{
		return false;
	}
	char* pChar1 = (char *)m_pBDA->m_pData;
	char* pChar2 = (char *)OtherObj.m_pBDA->m_pData;
	int rVal = memcmp(pChar1, pChar2,size1);
	return rVal == 0 ? true : false;
}

bool CBinaryString::operator==(const char* str)const
{
	CBinaryString bstr(str);
	bool rVal = (*this == bstr);
	return rVal;
}

bool CBinaryString::operator!=(const CBinaryString& OtherObj)const
{
	bool rVal = (*this == OtherObj);
	rVal = !rVal;
	return rVal;
}

bool CBinaryString::operator<(const CBinaryString& OtherObj) const
{
	size_t minDataSize = __min(m_pBDA->m_DataSize, OtherObj.m_pBDA->m_DataSize);
	int rVal = memcmp((char*)m_pBDA->m_pData, (char*)OtherObj.m_pBDA->m_pData, minDataSize);
	if (rVal == 0)
	{
		rVal = (m_pBDA->m_DataSize < OtherObj.m_pBDA->m_DataSize);
		return rVal;
	}
	if (rVal < 0)
	{
		return true;
	}
	else {
		return false;
	}
}

CBinaryString::operator const char8_t* ()  noexcept
{
	return m_pBDA->m_pData;
}

CBinaryString::operator const char* () noexcept
{
	return (char *)m_pBDA->m_pData;
}

CBinaryString::operator const wchar_t* () noexcept
{
	return (wchar_t*)m_pBDA->m_pData;
}

CBinaryString::operator std::string()
{
	std::string str(c_strA());
	return str;
}


CBinaryString::CBinaryString() :
	m_pBDA(nullptr)
{
	m_pBDA = new CBinaryStringCore(DefAlloc);
}

std::vector<std::string> CBinaryString::Split(const std::string str, const std::string separator)
{
	using namespace std;
	if (separator == "") return { str };
	vector<string> result;
	string tstr = str + separator;
	size_t len = tstr.length(), splen = separator.length();
	string::size_type pos = 0, prev = 0;

	for (; pos < len && (pos = tstr.find(separator, pos)) != string::npos; prev = (pos += splen)) {
		if (len != 0)
		{
			if (pos - prev <= 0)
			{
				break;
			}
			result.emplace_back(tstr, prev, pos - prev);
		}
		else {
			break;
		}
	}
	return result;
}

std::vector<std::wstring> CBinaryString::SplitW(const std::wstring wstr, const std::wstring wseparator)
{
	using namespace std;
	if (wseparator == L"") return { wstr };
	vector<wstring> result;
	wstring twstr = wstr + wseparator;
	size_t len = twstr.length(), splen = wseparator.length();
	wstring::size_type pos = 0, prev = 0;

	for (; pos < len && (pos = twstr.find(wseparator, pos)) != wstring::npos; prev = (pos += splen)) {
		if (len != 0)
		{
			if (pos - prev <= 0)
			{
				break;
			}
			result.emplace_back(twstr, prev, pos - prev);
		}
		else {
			break;
		}
	}
	return result;
}

void CBinaryString::RemoveSpaceHexTextA()
{
	if (m_pBDA->m_DataSize <= 0)
	{
		return;
	}
	char8_t* pDest = new char8_t[m_pBDA->m_DataSize+2];
	char8_t* pSource = m_pBDA->m_pData;
	int SourIndex = 0;
	int DestIndex = 0;
	while (SourIndex <= m_pBDA->m_DataSize)
	{
		memcpy(pDest + DestIndex, pSource+SourIndex, 2);
		DestIndex += 2;
		SourIndex += 3;
	}
	memcpy(m_pBDA->m_pData, pDest, DestIndex);
	m_pBDA->m_pData[DestIndex] = '\0';
	m_pBDA->m_DataSize = DestIndex;
	delete [] pDest;
}

void CBinaryString::InsertSpaceHexTextA()
{
	size_t BufSize = m_pBDA->m_DataSize * 3 / 2;
	BufSize = __max(BufSize, m_pBDA->m_BufSize);
	CBinaryStringCore* pBDADest = new CBinaryStringCore(BufSize);
	char8_t* pDest= pBDADest->m_pData;
	char8_t* pSour = m_pBDA->m_pData;
	int SourIndex = 0;
	int DestIndex = 0;
	while (SourIndex < m_pBDA->m_DataSize)
	{
		memcpy(pDest + DestIndex, pSour + SourIndex, 2);
		pDest[DestIndex+2] = ' ';
		DestIndex += 3;
		SourIndex += 2;
	}
	pBDADest->m_pData[DestIndex-1] = '\0';
	pBDADest->m_DataSize = DestIndex-1;
	m_pBDA->Release();
	m_pBDA = pBDADest;
}

CBinaryString::CBinaryString( const CBinaryString  &OtherCByteData)
{
	OtherCByteData.m_pBDA->AddRef();
	m_pBDA = OtherCByteData.m_pBDA;
}

CBinaryString::CBinaryString(size_t Size)
{
	m_pBDA = new CBinaryStringCore(Size);
}

CBinaryString::CBinaryString(const char DataSZ[])
{
	size_t len = strlen(DataSZ);
	size_t maxbuf = __max(len, DefAlloc);
	m_pBDA = new CBinaryStringCore(maxbuf);
	memcpy_s(m_pBDA->m_pData, maxbuf, DataSZ, len);
	m_pBDA->m_DataSize = len;
}

CBinaryString::CBinaryString(const wchar_t DataSZ[])
{
	size_t len = wcslen(DataSZ);
	size_t maxbuf = __max(len*sizeof(wchar_t), DefAlloc);
	m_pBDA = new CBinaryStringCore(maxbuf);
	wmemcpy((wchar_t*)m_pBDA->m_pData, DataSZ, len);
	m_pBDA->m_DataSize = len*sizeof(wchar_t);
}

CBinaryString::CBinaryString(const char8_t strSZ[])
{
	size_t len = strlen((char *)strSZ);
	size_t maxbuf = __max(len, DefAlloc);
	m_pBDA = new CBinaryStringCore(maxbuf);
	memcpy_s(m_pBDA->m_pData, maxbuf, strSZ, len);
	m_pBDA->m_DataSize = len;
}

CBinaryString::CBinaryString(const std::string str)
{
	size_t len = str.size();
	size_t maxbuf = __max(len, DefAlloc);
	m_pBDA = new CBinaryStringCore(maxbuf);
	memcpy_s(m_pBDA->m_pData, maxbuf, str.c_str(), len);
	m_pBDA->m_DataSize = len;
}

CBinaryString::CBinaryString(const std::wstring str)
{
	size_t len = str.size();
	size_t maxbuf = __max(len*sizeof(wchar_t), DefAlloc);
	m_pBDA = new CBinaryStringCore(maxbuf);
	memcpy_s(m_pBDA->m_pData, maxbuf, str.c_str(), len * sizeof(wchar_t));
	m_pBDA->m_DataSize = len * sizeof(wchar_t);
}

CBinaryString::~CBinaryString()
{
	m_pBDA->Release();
}

CBinaryString& CBinaryString::SetTextStringA(const char* pDataSZ)
{
	size_t len = strlen(pDataSZ);
	size_t maxsize = __max(len , m_pBDA->m_BufSize);
	CBinaryStringCore* pBDA = new CBinaryStringCore(maxsize);

	memcpy(pBDA->m_pData, pDataSZ, len);
	pBDA->m_DataSize = len;
	m_pBDA->Release();
	m_pBDA = pBDA;
	return * this;
}

CBinaryString& CBinaryString::SetTextStringW(const wchar_t* pDataSZ)
{
	size_t len = wcslen(pDataSZ);
	size_t wlen = len * sizeof(wchar_t);
	size_t maxsize = __max(wlen, m_pBDA->m_BufSize);
	CBinaryStringCore* pTemp = new CBinaryStringCore(maxsize);

	memcpy(pTemp->m_pData, pDataSZ, wlen);
	pTemp->m_DataSize = wlen;
	m_pBDA->Release();
	m_pBDA = pTemp;
	return *this;
}


CBinaryString& CBinaryString::SetBinaryString8(const char8_t*  pData,size_t size)
{
	size_t maxsize = __max(size, m_pBDA->m_BufSize);
	CBinaryStringCore* pTemp=new CBinaryStringCore(maxsize);
	
	memcpy(pTemp->m_pData, pData, size);
	pTemp->m_DataSize = size;
	m_pBDA->Release();
	m_pBDA = pTemp;
	return  * this;
}

CBinaryString& CBinaryString::SetBinaryStringW(const wchar_t* pData, size_t wsize)
{
	size_t size = wsize * sizeof(wchar_t);
	size_t maxsize = __max(size, m_pBDA->m_BufSize);
	CBinaryStringCore* pTemp = new CBinaryStringCore(maxsize);

	memcpy(pTemp->m_pData, pData, size);
	pTemp->m_DataSize = size;
	m_pBDA->Release();
	m_pBDA = pTemp;
	return  *this;
}

CBinaryString& CBinaryString::Clear()
{
	if (m_pBDA->m_RefCount > 1)
	{
		CBinaryStringCore* pBDA = new CBinaryStringCore(m_pBDA->m_BufSize);
		m_pBDA->Release();
		m_pBDA = pBDA;
	}
	else {
		m_pBDA->m_DataSize = 0;
	}

	return *this;
}


const char* CBinaryString::c_strA() const noexcept
{
	return (char *)m_pBDA->m_pData;
}

const char8_t* CBinaryString::c_str8() const noexcept
{
	return m_pBDA->m_pData;
}

const wchar_t* CBinaryString::c_strw() const noexcept
{
	return (wchar_t*)m_pBDA->m_pData;
}


CBinaryString& CBinaryString::SetBufReSize(const size_t size)
{
	CBinaryStringCore *pTmp = new CBinaryStringCore(size);
	memcpy_s(pTmp->m_pData, size, m_pBDA->m_pData, __min(size,m_pBDA->m_BufSize));
	pTmp->m_DataSize=__min(size, m_pBDA->m_DataSize);
	pTmp->m_pData[pTmp->m_DataSize] = '\0';
	m_pBDA->Release();
	m_pBDA = pTmp;
	return *this;
}

CBinaryString& CBinaryString::SetBufReSizeW(const size_t wsize)
{
	CBinaryStringCore* pTmp = new CBinaryStringCore(wsize*sizeof(wchar_t));
	memcpy_s(pTmp->m_pData, wsize*sizeof(wchar_t), m_pBDA->m_pData, __min(wsize*sizeof(wchar_t), m_pBDA->m_BufSize));
	pTmp->m_DataSize = __min(wsize*sizeof(wchar_t), m_pBDA->m_DataSize);
	((wchar_t*)pTmp->m_pData)[pTmp->m_DataSize / sizeof(wchar_t)] = L'\0';
	m_pBDA->Release();
	m_pBDA = pTmp;
	return *this;
}

CBinaryString& CBinaryString::Discard()
{
	SetBufReSize();
	Clear();
	return *this;
}


unsigned int CBinaryString::GetBufSize()const noexcept
{
	return m_pBDA->m_BufSize;
}

unsigned int CBinaryString::GetBufSizeW() const noexcept
{
	return m_pBDA->m_BufSize/sizeof(wchar_t);
}


CBinaryString& CBinaryString::SetDataSize(const unsigned int size)
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}
	if (m_pBDA->m_BufSize < size)
	{
		SetBufReSize(size);
	}
	m_pBDA->m_DataSize = size;

	m_pBDA->m_pData[size] = '\0';
	return *this;
}

CBinaryString& CBinaryString::SetDataSizeW(const unsigned int wsize)
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach(*this);
	}
	if (m_pBDA->m_BufSize < wsize * sizeof(wchar_t))
	{
		SetBufReSizeW(wsize * sizeof(wchar_t));
	}
	m_pBDA->m_DataSize = wsize*sizeof(wchar_t);
	((wchar_t *)m_pBDA->m_pData)[wsize] = L'\0';
	return *this;
}

unsigned int CBinaryString::GetDataSize()const noexcept
{
	return m_pBDA->m_DataSize;
}

unsigned int CBinaryString::GetDataSizeW() const noexcept
{
	return m_pBDA->m_DataSize/sizeof(wchar_t);
}

char8_t*  CBinaryString::GetBuffer8()const noexcept
{
	return m_pBDA->m_pData;
}

wchar_t* CBinaryString::GetBufferW() const noexcept
{
	return (wchar_t*)m_pBDA->m_pData;
}

char* CBinaryString::GetBufferA() const noexcept
{
	return (char*)(m_pBDA->m_pData);
}

CBinaryString& CBinaryString::Detach(const CBinaryString& OtherCByteData)
{
	CBinaryStringCore* ptempBSC = new CBinaryStringCore(OtherCByteData.m_pBDA->m_BufSize);
	memcpy(ptempBSC->m_pData, OtherCByteData.m_pBDA->m_pData, OtherCByteData.m_pBDA->m_DataSize);
	ptempBSC->m_DataSize = OtherCByteData.m_pBDA->m_DataSize;
	m_pBDA->Release();
	m_pBDA = ptempBSC;
	return  *this;
}

CBinaryString& CBinaryString::Detach()
{
	if (m_pBDA->m_RefCount == 1)
	{
		return *this;
	}
	CBinaryStringCore* ptempBSC = new CBinaryStringCore(m_pBDA->m_BufSize);
	memcpy(ptempBSC->m_pData, m_pBDA->m_pData, m_pBDA->m_DataSize);
	ptempBSC->m_DataSize = m_pBDA->m_DataSize;
	m_pBDA->Release();
	m_pBDA = ptempBSC;
	return  *this;
}

CBinaryString CBinaryString::BinaryDataToSpaceHexTextA()
{
	if (m_pBDA->m_RefCount == 1)
	{
		Detach();
	}
	size_t SourceLen = m_pBDA->m_DataSize ;
	size_t BufSize = m_pBDA->m_BufSize * 3 + 1;
	char8_t *pstrSource = m_pBDA->m_pData;
	CBinaryStringCore* pBDAtom = new CBinaryStringCore(BufSize);
	size_t szDest = pBDAtom->m_BufSize;
	char8_t* pstrDest = pBDAtom->m_pData;
	char str[3]{};
	size_t TotalLen = 0;
	size_t LenWriten = 0;
	unsigned int i = 0;
	do {

		LenWriten = sprintf_s(str, 3, "%02X", (unsigned char)pstrSource[i]);
		_memccpy(pstrDest + TotalLen, str, sizeof(char), LenWriten);
		TotalLen += LenWriten;
		pstrDest[TotalLen] = ' ';
		TotalLen++;
		i++;
	} while (i < SourceLen );
	pstrDest[TotalLen - 1] = '\0';
	pBDAtom->m_DataSize=TotalLen - 1;
	m_pBDA->Release();
	m_pBDA = pBDAtom;
	return *this;
}

CBinaryString CBinaryString::BinaryDataToSpaceHexTextW()
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}
	size_t numbSourceLen = m_pBDA->m_DataSize;
//	size_t numwSourceLen = m_pBDA->m_DataSize/2;
	size_t numbBufSize = (numbSourceLen * 2 )*sizeof(wchar_t);
	if (m_pBDA->m_BufSize > numbBufSize)
	{
		numbBufSize = m_pBDA->m_BufSize;
	}
	char * pSourceData = (char*)m_pBDA->m_pData;
	CBinaryStringCore* ptempBD = new CBinaryStringCore(numbBufSize);
	wchar_t* pwstrDest = (wchar_t*)ptempBD->m_pData;
	char str[12]{};
	size_t numwTotal = 0;
	size_t wLenWriten = 0;
	unsigned int i = 0;
	do {

		wLenWriten = swprintf_s((wchar_t*)str, 6, L"%02X", pSourceData[i]);
		wmemcpy(pwstrDest + numwTotal, (wchar_t*)str, wLenWriten);
		numwTotal += wLenWriten;
		pwstrDest[numwTotal] = L' ';
		numwTotal +=1;
		i++;
	} while (i < numbSourceLen);
	pwstrDest[numwTotal - 1] = L'\0';
	ptempBD->m_DataSize = (numwTotal - 1)*sizeof(wchar_t);
	m_pBDA->Release();
	m_pBDA = ptempBD;
	return *this;
}

CBinaryString CBinaryString::SpaceHexTextToBinaryDataA()
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}
	using namespace std;
	vector<string> Parts = Split(string(c_strA()), string(" "));
	string str;
	int i = 0;
	char* pcData=new char[m_pBDA->m_DataSize+1]{};
	while (!Parts.empty())
	{
		str = Parts.front();
		pcData[i] = stol(str, NULL, 16);
		i++;
		Parts.erase(Parts.begin());
	}
	memcpy(m_pBDA->m_pData, pcData, i + 2);
	m_pBDA->m_DataSize=i;
	delete [] pcData;
	return *this;
}

CBinaryString CBinaryString::SpaceHexTextToBinaryDataW()
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}
	using namespace std;
	vector<wstring> Parts = SplitW(wstring(c_strw()), wstring(L" "));
	wstring str;
	int i = 0;
	char* pcData = new char[(m_pBDA->m_DataSize + 3)*sizeof(wchar_t)]{};
	while (!Parts.empty())
	{
		str = Parts.front();
		pcData[i] = wcstoul(str.c_str(), NULL, 16);
		i++;
		Parts.erase(Parts.begin());
	}
	memcpy(m_pBDA->m_pData, pcData, i+3 );
	m_pBDA->m_DataSize = i;
	delete[] pcData;
	return *this;
}

CBinaryString CBinaryString::HexTextToBinaryDataA()
{
	InsertSpaceHexTextA();
	SpaceHexTextToBinaryDataA();
	return *this;
}

CBinaryString CBinaryString::TrimFirstCodeA(char demilita)
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}
	CBinaryString tmpBD(GetBufSize());
	if (GetDataSize() == 0)
	{
		return tmpBD;
	}
	size_t datasize = GetDataSize();
	char* pSource = (char *)m_pBDA->m_pData;
	bool bContinue = false;
	int count = 0;
	while (true)
	{
		if (memcmp(pSource+count, &demilita, 1))
		{
			count++;
			if (count  == datasize)
			{
				return tmpBD;
			}
			continue;
		}

		break;
	}
	char* pTmpDest = (char*)tmpBD.m_pBDA->m_pData;
	memcpy(pTmpDest, pSource, count);
	pTmpDest[count] = '\0';
	tmpBD.SetDataSize(count);
	count++;

	SetBinaryString8(m_pBDA->m_pData + count , datasize - count );
	return tmpBD;
}

CBinaryString CBinaryString::TrimFirstCodesA(const char* strdemilita)
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}

	CBinaryString rVal;
	CBinaryString dem(strdemilita);
	size_t demlen = dem.GetDataSize();
	int pos = FindFirst(dem);
	if (pos !=-1)
	{
		rVal = TrimPosition(0, pos - 1);
		TrimPosition(0, demlen - 1);
	}
	return rVal;
}

CBinaryString CBinaryString::TrimFirstBFormat()
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}

	CBinaryString strBF = this->TrimFirstCodeA(' ');
	CBinaryString strLen = this->TrimFirstCodeA(' ');
	size_t sizeLen = atol(strLen.c_strA());
	CBinaryString tmp;
	CBinaryString tmp2;
	tmp = this->TrimPosition(0,sizeLen-1);
	tmp2 = strBF + " " + strLen + " " + tmp;
	return tmp2;
}

CBinaryString CBinaryString::TrimPosition(unsigned long begin, unsigned long end)
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}

	CBinaryString tmpBD(GetBufSize());
	if (!(end <= GetDataSize()) || !(begin<=end))
	{
		return tmpBD;
	}

	char* pDest = (char *)tmpBD.m_pBDA->m_pData;
	char* pSource = (char *)m_pBDA->m_pData;
	memcpy(pDest, pSource+begin, end - begin+1);
	tmpBD.SetDataSize(end - begin+1);

	size_t datasize = GetDataSize();
	CBinaryString tmpNewSource(GetBufSize());
	char* pNewSource = (char*)tmpNewSource.m_pBDA->m_pData;
	memcpy(pNewSource, pSource, begin);
	memcpy(pNewSource+begin, pSource+end+1, GetDataSize()-1);
	pNewSource[GetDataSize()-(end-begin+ 1)] = '\0';
	SetBinaryString8((char8_t*)pNewSource,datasize-(end-begin+1));
	return tmpBD;
}

int CBinaryString::FindFirst(const CBinaryString BS, unsigned int InitPos)const
{
	size_t SizeSrc = GetDataSize();
	size_t SizeFnd = BS.GetDataSize();

	char* pstrFnd = (char *)BS.m_pBDA->m_pData;
	char* pstrSrc = (char *)m_pBDA->m_pData;
	int cntSrc = InitPos;
	int rVal = 0;
	for (; ; cntSrc++)
	{
		if (SizeSrc - cntSrc < SizeFnd)
		{
			rVal = -1;
			break;
		}
		rVal=memcmp(pstrSrc+cntSrc, pstrFnd, SizeFnd);
		if (rVal == 0)
		{
			rVal = cntSrc;
			break;
		}
	}
	return rVal;
}

int CBinaryString::FindFirstA(const char* strFnd, unsigned int InitPos)const
{
	CBinaryString bstrFnd(strFnd);
	int rVal = FindFirst(bstrFnd,InitPos);
	return rVal;
}

int CBinaryString::FindLast(const CBinaryString BS, unsigned int InitPos)const
{
	size_t SizeSrc = GetDataSize();
	size_t SizeFnd = BS.GetDataSize();

	if (InitPos == -1)
	{
		InitPos = SizeSrc - SizeFnd;
	}

	if (InitPos > (SizeSrc - SizeFnd) || InitPos<0)
	{
		return -1;
	}
	char* pstrFnd = (char*)BS.m_pBDA->m_pData;
	char* pstrSrc = (char*)m_pBDA->m_pData;
	int cntSrc = InitPos;
	int rVal = 0;
	for (; ; cntSrc--)
	{
		if (cntSrc < 0)
		{
			rVal = -1;
			break;
		}
		rVal = memcmp(pstrSrc + cntSrc, pstrFnd, SizeFnd);
		if (rVal == 0)
		{
			rVal = cntSrc;
			break;
		}
	}
	return rVal;
}

int CBinaryString::FindLastA(const char* strFnd, unsigned int InitPos)const
{
	CBinaryString bstrFnd(strFnd);
	int rVal = FindLast(bstrFnd, InitPos);
	return rVal;
}

CBinaryString CBinaryString::MakeBFormat(emBFormat bFormat)
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}
	CBinaryString bstr(DefAlloc);
	CBinaryString thistmp(*this);
	char strLen[64]{};
	unsigned long len{0};
	if (bFormat == emBFormat::BS)
	{
		len = thistmp.GetDataSize();
		_ultoa_s(len, strLen, 64, 10);
		bstr = "BS " + CBinaryString(strLen) + " " + thistmp;
		(*this)(bstr);
	}
	if (bFormat == emBFormat::HTS)
	{
		thistmp.BinaryDataToSpaceHexTextA();
		thistmp.RemoveSpaceHexTextA();
		len = thistmp.GetDataSize();
		_ultoa_s(len, strLen, 64, 10);
		bstr = "HTS " + CBinaryString(strLen) + " " + thistmp;
		(*this)(bstr);
	}
	return *this;
}

CBinaryString CBinaryString::UnMakeBFormat()
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}

	CBinaryString tmp = *this;
	CBinaryString strFormat = tmp.TrimFirstCodeA(' ');
	CBinaryString strLen = tmp.TrimFirstCodeA(' ');
	size_t len = atol(strLen.c_strA());
	CBinaryString tmpData = tmp.TrimPosition(0, len-1);
	if (strFormat == "BS")
	{
		(*this)(tmpData);
	}
	else if (strFormat == "HTS")
	{
		tmpData.InsertSpaceHexTextA();
		tmpData.SpaceHexTextToBinaryDataA();
		(*this)(tmpData);
	}
	else
	{
		return CBinaryString("");
	}
	return tmp;
}

bool CBinaryString::empty()const noexcept
{
	return (m_pBDA->m_DataSize == 0) ? true:false;
}

void CBinaryString::SetDebugMark(const char* pstr,int size)
{
	if (m_pBDA->m_RefCount > 1)
	{
		Detach();
	}
	int maxsize = __max(8, size);
	memcpy(m_pBDA->m_pDebugMark, pstr,maxsize);
}

CBinaryString operator+(const CBinaryString obj1,const CBinaryString obj2)
{
	size_t obj1Size = obj1.GetDataSize();
	size_t obj2Size = obj2.GetDataSize();
	size_t TotalSize = obj1Size + obj2Size;
	CBinaryString tmp(TotalSize);
	char8_t *pStrTmp = tmp.GetBuffer8();
	memcpy(pStrTmp, obj1.GetBuffer8(),obj1Size );
	memcpy(pStrTmp + obj1Size, obj2.GetBuffer8(), obj2Size);
	tmp.SetDataSize(TotalSize);
	return CBinaryString(tmp);
}

CBinaryString operator+(const char* str1, const CBinaryString str2)
{
	CBinaryString bstr1(str1);
	CBinaryString tmp = bstr1 + str2;
	return tmp;
}

CBinaryString operator+(const std::string str1, const CBinaryString str2)
{
	CBinaryString bstr1(str1.c_str());
	CBinaryString tmp = bstr1 + str2;
	return tmp;
}
