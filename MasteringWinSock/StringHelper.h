#pragma once
#include <string>
#include <xlocbuf>
#include <vector>

class StringHelper
{
public:
enum ErrCode { Undef,NotEnoughBuffer,DetectedNULL };
	std::u8string WideSZtoUtf8SZ(std::wstring const& src);
	std::wstring Utf8SZtoWideSZ(std::u8string const& src);
	int TextToByteDATA(CString strData, BYTE* pByte, int ByteSize);
protected:
	UINT m_Err;
public:
	void ErrMessageBox();
protected:
	CString CodeToString(int Err);
public:
	int ByteDataToHexText(char8_t* byteData, int byteLen, CString& destStr);//Byteデータ配列を「0D 0A」の様なスペースで区切った文字列に変換
	CString Utf8ToCString(std::u8string const& src);
	CStringW Utf8ToCStringW(std::u8string const& src);
	std::u8string CStringWToUtf8(CStringW& src);
	CString TranslateEscape(CString strSource);
	CString InvTranslateEscape(CString strSource);
	int RemoveNullChar(char8_t* pSrc, int numSrc, char8_t* pDst, int sz_Dst);//途中に'\0'の入った配列から'\0'を取り除く。
};

