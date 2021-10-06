#include "pch.h"
#include "Regex.h"
#include <afxwin.h>
#include <tchar.h>

CString Regex::TranslateEscape(CString strSource)
{
	strSource.Replace(_T("\\\\"), _T("\\"));
	strSource.Replace(_T("\\r"), _T("\r"));
	strSource.Replace(_T("\\n"), _T("\n"));
	strSource.Replace(_T("\\t"), _T("\t"));
	return strSource;
}
CString Regex::ExecuteRegex(CString strSource, CString strRegex, CString strReplace)
{
	CString tmpSearch, tmpReplace, strResult;
	tmpSearch = TranslateEscape(strRegex);
	tmpReplace = TranslateEscape(strReplace);
	try
	{
		std::basic_regex<TCHAR> rx(tmpSearch);
		std::basic_string<TCHAR> std_strSource(strSource);
		std::basic_string<TCHAR> std_strFmt(tmpReplace);
		std::basic_string<TCHAR> result;
		result = std::regex_replace(std_strSource, rx, std_strFmt);
		strResult = result.c_str();
	}
	catch (std::regex_error& e)
	{
		std::basic_string<TCHAR> e_str = code_to_string(e.code());
		strResult = e_str.c_str();
		AfxMessageBox(strResult, MB_OK | MB_ICONEXCLAMATION);
	}

	return strResult;
}

CString Regex::InvTranslateEscape(CString strSource)
{
	strSource.Replace(_T("\r"), _T("\\r"));
	strSource.Replace(_T("\n"), _T("\\n"));
	strSource.Replace(_T("\t"), _T("\\t"));
	strSource.Replace(_T("\\"), _T("\\\\"));
	return strSource;
}


std::basic_string<TCHAR> Regex::code_to_string(std::regex_constants::error_type e)
{
	using namespace std::regex_constants;
	switch (e) {
	case error_collate:    return _T("error collapse\r\n���ɐ������Ȃ��ƍ��v�f�������݂��܂��B");
	case error_ctype:      return _T("error ctype\r\n���ɐ������Ȃ������N���X�������݂��܂��B");
	case error_escape:     return _T("error escape\r\n���ɐ������Ȃ��G�X�P�[�v �V�[�P���X�����݂��܂��B");
	case error_backref:    return _T("error back reference\r\n���ɐ������Ȃ��O���Q�Ƃ����݂��܂��B");
	case error_brack:      return _T("error bracket\r\n���� '[' �܂��� ']' �̑Ή�������������܂���B");
	case error_paren:      return _T("error paren\r\n���� '(' �܂��� ')' �̑Ή�������������܂���B");
	case error_brace:      return _T("error brace\r\n���� '{' �܂��� '}' �̑Ή�������������܂���B");
	case error_badbrace:   return _T("error bad brace\r\n���� { } ���ɐ������Ȃ��񐔂��܂�ł��܂��B");
	case error_range:      return _T("error range\r\n���ɐ������Ȃ������͈͎w��q�����݂��܂��B");
	case error_space:      return _T("error space\r\n���\�[�X�s���������Ő��K�\���̉�͂Ɏ��s���܂����B");
	case error_badrepeat:  return _T("error bad repeat\r\n�J��Ԃ��� (��ʂɁA'*'�A''�A'+'�A'{' �̂����ꂩ) �̑O�Ɏ����w�肳��Ă��܂���B");
	case error_complexity: return _T("error complexity\r\n�����������G�����邽�߂Ɏ��s���܂����B");
	case error_stack:	   return _T("error stack\r\n�������s���������Ō����Ɏ��s���܂����B");
	case error_parse:	   return _T("error parse\r\n���̉�͂Ɏ��s���܂����B");
	default:
		return _T("���K�\��\r\n���̑��̃G���[");
		//throw std::invalid_argument("invalid error code");
	}
}


BOOL Regex::Verify(CString strSource, CString strRegex, CString strReplace)
{
	CString tmpSearch, tmpReplace;
	BOOL bResult=TRUE;
	tmpSearch = TranslateEscape(strRegex);
	tmpReplace = TranslateEscape(strReplace);
	try
	{
		std::basic_regex<TCHAR> rx(tmpSearch);
		std::basic_string<TCHAR> std_strSource(strSource);
		std::basic_string<TCHAR> std_strFmt(tmpReplace);
		std::basic_string<TCHAR> result;
		result = std::regex_replace(std_strSource, rx, std_strFmt);
	}
	catch (std::regex_error& e)
	{
		bResult = FALSE;
	}
	return bResult;
}
