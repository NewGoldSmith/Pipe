#include "pch.h"
#include "StringHelper.h"

//NULL終端文字列std::wstringをstd::u8stringに変換
std::u8string StringHelper::WideSZtoUtf8SZ(std::wstring const& src)
{
	auto const dest_size = ::WideCharToMultiByte(CP_UTF8, 0U, src.data(), -1, nullptr, 0, nullptr, nullptr);
	std::vector<char8_t> dest(dest_size, '\0');
	if (::WideCharToMultiByte(CP_UTF8, 0U, src.data(), -1, (char *)dest.data(), dest.size(), nullptr, nullptr) == 0) {
		throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
	}
	dest.resize(std::char_traits<char8_t>::length(dest.data()));
	dest.shrink_to_fit();
	return std::u8string(dest.begin(), dest.end());
}

//NULL終端文字列std::u8stringをstd::wstringに変換
std::wstring StringHelper::Utf8SZtoWideSZ(std::u8string const& src)
{
	auto const dest_size = ::MultiByteToWideChar(CP_UTF8, 0U, (LPCCH)src.data(),-1,nullptr, 0U);
	std::vector<wchar_t> dest(dest_size, L'\0');
	if (::MultiByteToWideChar(CP_UTF8, 0U, (LPCCH)src.data(), -1, dest.data(), dest.size()) == 0) {
		throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
	}
	dest.resize(std::char_traits<wchar_t>::length(dest.data()));
	dest.shrink_to_fit();
	return std::wstring(dest.begin(), dest.end());
}

//「0D 0A」の様なCString（マルチバイトかワイドキャラ)をバイト配列に変換
int StringHelper::TextToByteDATA(CString strData, BYTE* pByte, int BytesSize)
{
	CString resToken;
	int curPos = 0;
	int Index = 0;
	resToken = strData.Tokenize(_T("% #"), curPos);
	while (resToken != _T(""))
	{
		if (BytesSize <= Index)
		{
			m_Err = ErrCode::NotEnoughBuffer;
			return -1;
		}
		pByte[Index] = _tcstol(resToken, NULL, 16);
		resToken = strData.Tokenize(_T("% #"), curPos);
		Index++;
	}
	return Index;
}


void StringHelper::ErrMessageBox()
{
	CString str;
	str.Format(_T("%s"), CodeToString(m_Err));
	AfxMessageBox(str, MB_OK | MB_ICONASTERISK);
}

//エラーコードをCStringに変換
CString StringHelper::CodeToString(int ErrCode)
{
	CString str;
	switch (ErrCode) {
	case ErrCode::NotEnoughBuffer:      return _T("StringHelper\r\n変換後格納バッファーが足りません。");
	case ErrCode::DetectedNULL:           return _T("StringHelper\r\nData配列にNULLを検出しました。");
	default:
		str.Format(_T("その他のエラー　%d"), ErrCode);
		return str;
	}
	return CString();
}

//Byteデータ配列を「0D 0A」の様なスペースで区切った文字列に変換
int StringHelper::ByteDataToHexText(char8_t* byteData, int byteLen,CString &destStr)
{
	int i = 0;
	CString str;
	CString retStr;
	str.Format(_T("%02X"), byteData[i]);
	retStr.Append(str);
	i++;
	while (i < byteLen)
	{
	   retStr += _T(" ");
		str.Format(_T("%02X"), byteData[i]);
		retStr.Append(str);
		i++;
	}
	destStr = retStr;
	return i;
}

//utf-8から、ビルド設定の文字コードのCStirngに変換
CString StringHelper::Utf8ToCString(std::u8string const& src)
{
	std::basic_string<wchar_t> stdWstr = Utf8SZtoWideSZ(src);
	CStringW cstrW(stdWstr.c_str());
	return CString(cstrW);
}

//utf-8からCStirngWに変換
CStringW StringHelper::Utf8ToCStringW(std::u8string const& src)
{
	std::basic_string<wchar_t> stdWstr = Utf8SZtoWideSZ(src);
	return CStringW(stdWstr.c_str());
}

//CStringWから、Utf8に変換
std::u8string StringHelper::CStringWToUtf8(CStringW& src)
{
	std::basic_string<wchar_t> stdWstr(src);
	std::basic_string<char8_t> stdU8str = WideSZtoUtf8SZ(stdWstr);
	return stdU8str;
}

//"\\r"等の文字列をエスケープシーケンスに変換
CString StringHelper::TranslateEscape(CString strSource)
{
	strSource.Replace(_T("\\\\"), _T("\\"));
	strSource.Replace(_T("\\r"), _T("\r"));
	strSource.Replace(_T("\\n"), _T("\n"));
	strSource.Replace(_T("\\t"), _T("\t"));
	return strSource;
}


//エスケープシーケンスから「\\r」等の文字に変換
CString StringHelper::InvTranslateEscape(CString strSource)
{
	strSource.Replace(_T("\r"), _T("\\r"));
	strSource.Replace(_T("\n"), _T("\\n"));
	strSource.Replace(_T("\t"), _T("\\t"));
	strSource.Replace(_T("\\"), _T("\\\\"));
	return strSource;
}

//途中に'\0'の入ったByte配列から'\0'を取り除く。
int StringHelper::RemoveNullChar(char8_t* pSrc, int numSrc, char8_t* pDst, int sz_Dst)
{
	int IndexSrc = 0;
	int IndexDst = 0;
	while (IndexSrc< numSrc)
	{
		if (pSrc[IndexSrc] == '\0')
		{
			IndexSrc++;
			continue;
		}
		if (sz_Dst <=IndexSrc)
		{
			return -1;
		}
		pDst[IndexDst] = pSrc[IndexSrc];

		IndexSrc++;
		IndexDst++;
	}
	return IndexDst;
}
