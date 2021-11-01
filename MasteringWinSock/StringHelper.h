#pragma once
#include <string>
#include <xlocbuf>
#include <vector>

class StringHelper
{
public:
enum ErrCode { Undef,NotEnoughBuffer,DetectedNULL };
	std::u8string wide_to_utf8(std::wstring const& src);
	std::wstring utf8_to_wide(std::u8string const& src);
	int TextToByteDATA(CString strData, BYTE* pByte, int ByteSize);
protected:
	UINT m_Err;
public:
	void ErrMessageBox();
protected:
	CString CodeToString(int Err);
public:
	int ByteDataToText(char8_t* byteData, int byteLen, CString& destStr);
	CString Utf8ToCString(std::u8string const& src);
	CStringW Utf8ToCStringW(std::u8string const& src);
	std::u8string CStringWToUtf8(CStringW& src);
	CString TranslateEscape(CString strSource);
	CString InvTranslateEscape(CString strSource);
};

