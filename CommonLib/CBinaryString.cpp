#include "CBinaryString.h"



CBinaryString& CBinaryString::operator=(const CBinaryString &OtherCByteData)
{
	SetBinaryString8(OtherCByteData.c_str8(), OtherCByteData.GetDataSize());
	SetBufReSize(OtherCByteData.GetBufSize());
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

CBinaryString& CBinaryString::operator()(CBinaryString&& OtherObj)
{
	m_pBDA = OtherObj.m_pBDA;
	OtherObj.m_pBDA = new CBinaryStringCore(1024);
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

CBinaryString::operator std::string()
{
	std::string str(c_strA());
	return str;
}


CBinaryString::CBinaryString() :
	m_pBDA(nullptr)
{
	m_pBDA = new CBinaryStringCore(1024);
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

void CBinaryString::RemoveSpaceHexText()
{
	if (m_pBDA->m_DataSize <= 0)
	{
		return;
	}
	char8_t* pDest = new char8_t[m_pBDA->m_DataSize];
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

void CBinaryString::InsertSpaceHexText()
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
	size_t maxbuf = __max(len, 1024);
	m_pBDA = new CBinaryStringCore(maxbuf);
	memcpy_s(m_pBDA->m_pData, len, DataSZ, len);
	m_pBDA->m_pData[len] = '\0';
	m_pBDA->m_DataSize = len;

}

CBinaryString::CBinaryString(CBinaryString&& OtherObj)noexcept:
	m_pBDA(OtherObj.m_pBDA)
{
	OtherObj.m_pBDA = new CBinaryStringCore(1023);
}

CBinaryString::CBinaryString(const char8_t*  psrcData, size_t nSize)
{
	size_t size = __max(nSize, 1024);
	m_pBDA = new CBinaryStringCore(size);
	memcpy_s(m_pBDA->m_pData, nSize, psrcData, nSize);
	m_pBDA->m_pData[nSize] = '\0';
	m_pBDA->m_DataSize= nSize;
}

CBinaryString::CBinaryString(const std::string str)
{
	size_t len = str.size();
	size_t maxbuf = __max(len, 1024);
	m_pBDA = new CBinaryStringCore(maxbuf);
	memcpy_s(m_pBDA->m_pData, len, str.c_str(), len);
	m_pBDA->m_pData[len] = '\0';
	m_pBDA->m_DataSize = len;
}

CBinaryString& CBinaryString::operator=(CBinaryString&& OtherObj)noexcept
{
	m_pBDA = OtherObj.m_pBDA;
	OtherObj.m_pBDA = new CBinaryStringCore(1024);
	return *this;
}

CBinaryString::~CBinaryString()
{
	m_pBDA->Release();
}

CBinaryString& CBinaryString::SetTextStringA(const char* pDataSZ)
{
	size_t len = strlen(pDataSZ);
	size_t maxsize = __max(len, m_pBDA->m_BufSize);
	CBinaryStringCore *pBDA = new CBinaryStringCore(maxsize);
	memcpy(pBDA->m_pData, pDataSZ, len);
//	pBDA->m_pData[len] = '\0';
	pBDA->m_DataSize = len;
	m_pBDA->Release();
	m_pBDA = pBDA;
	return * this;
}


CBinaryString& CBinaryString::SetBinaryString8(const char8_t*  pData,size_t size)
{
	size_t maxsize = __max(size, m_pBDA->m_BufSize);
	CBinaryStringCore* pBDA = new CBinaryStringCore(maxsize);
	memcpy(pBDA->m_pData, pData, size);
//	pBDA->m_pData[size] = '\0';
	pBDA->m_DataSize = size;
	m_pBDA->Release();
	m_pBDA = pBDA;
	return  * this;
}

CBinaryString& CBinaryString::SetBinaryStringW(const wchar_t* pData, size_t sizebyte)
{
	size_t maxsize = __max(sizebyte+2, m_pBDA->m_BufSize-2);
	CBinaryStringCore* pBDA = new CBinaryStringCore(maxsize);
	memcpy(pBDA->m_pData, pData, sizebyte);
	//pBDA->m_pData[sizebyte] = '\0';
	//pBDA->m_pData[sizebyte + 1] = '\0';
	pBDA->m_DataSize = sizebyte;
	m_pBDA->Release();
	m_pBDA = pBDA;
	return  *this;
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
	pTmp->m_BufSize= size;
	pTmp->m_DataSize=__min(size, m_pBDA->m_DataSize);
	pTmp->m_pData[pTmp->m_DataSize] = '\0';
	m_pBDA->Release();
	m_pBDA = pTmp;
	return *this;
}


size_t CBinaryString::GetBufSize()const noexcept
{
	return m_pBDA->m_BufSize;
}


CBinaryString& CBinaryString::SetDataSize(const size_t size)
{
	m_pBDA->m_DataSize=size;
	m_pBDA->m_pData[size] = '\0';
	return *this;
}

size_t CBinaryString::GetDataSize()const noexcept
{
	return m_pBDA->m_DataSize;
}

char8_t*  CBinaryString::GetBuffer8()const noexcept
{
	return m_pBDA->m_pData;
}

char* CBinaryString::GetBufferA() const noexcept
{
	return (char*)(m_pBDA->m_pData);
}

CBinaryString& CBinaryString::Detach(const CBinaryString& OtherCByteData)
{
	this->SetBinaryString8(OtherCByteData.c_str8(), OtherCByteData.GetDataSize());
	this->SetBufReSize(OtherCByteData.GetBufSize());
	return *this;
}

CBinaryString CBinaryString::BinaryDataToSpaceHexTextA()
{
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
	pBDAtom->SetDataSize(TotalLen - 1);
	m_pBDA->Release();
	m_pBDA = pBDAtom;
	return *this;
}

CBinaryString CBinaryString::SpaceHexTextToBinaryDataA()
{
	using namespace std;
	vector<string> Parts = Split(string(c_strA()), string(" "));
	string str;
	int i = 0;
	char* pcData=new char[m_pBDA->m_DataSize]{};
	while (!Parts.empty())
	{
		str = Parts.front();
		pcData[i] = stol(str, NULL, 16);
		i++;
		Parts.erase(Parts.begin());
	}
	memcpy(m_pBDA->m_pData, pcData, i + 1);
	m_pBDA->SetDataSize(i);
	delete [] pcData;
	return *this;
}

CBinaryString CBinaryString::TrimFirstCodeA(char demilita)
{
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

	SetBinaryString8(c_str8() + count , datasize - count );
	return tmpBD;
}

CBinaryString CBinaryString::TrimFirstCodesA(const char* strdemilita)
{
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
	CBinaryString bstr(1024);
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
		thistmp.RemoveSpaceHexText();
		len = thistmp.GetDataSize();
		_ultoa_s(len, strLen, 64, 10);
		bstr = "HTS " + CBinaryString(strLen) + " " + thistmp;
		(*this)(bstr);
	}
	return *this;
}

CBinaryString CBinaryString::UnMakeBFormat()
{
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
		tmpData.InsertSpaceHexText();
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

CBinaryString operator+(const CBinaryString obj1,const CBinaryString obj2)
{
	size_t obj1Size = obj1.GetDataSize();
	size_t obj2Size = obj2.GetDataSize();
	size_t TotalSize = obj1Size + obj2Size;
	CBinaryString tmp(TotalSize);
	char8_t *pStrTmp = tmp.GetBuffer8();
	memcpy(pStrTmp, obj1.GetBuffer8(),obj1Size );
	memcpy(pStrTmp + obj1Size, obj2.GetBuffer8(), obj2Size);

	return CBinaryString(pStrTmp,TotalSize);
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
