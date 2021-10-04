#include "pch.h"
#include "Regex.h"

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
	CString tmpSource, tmpSearch, tmpReplace,strResult;
	tmpSearch = TranslateEscape(strRegex);
	tmpReplace = TranslateEscape(strReplace);

	try
	{
		std::regex rx(tmpSearch);
		std::string str(strSource);
		std::string fmt(tmpReplace);
		std::string result;
		result = std::regex_replace(str, rx, fmt);
		strResult = result.c_str();
	}
	catch (std::regex_error& e)
	{
		std::string e_str = code_to_string(e.code());
		CString strResult = e_str.c_str();
		AfxMessageBox(strResult.GetBuffer(), MB_OK | MB_ICONEXCLAMATION);
		strResult.ReleaseBuffer();
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


std::string Regex::code_to_string(std::regex_constants::error_type e)
{
	using namespace std::regex_constants;
	switch (e) {
	case error_collate:    return "error collapse\r\n���ɐ������Ȃ��ƍ��v�f�������݂��܂��B";
	case error_ctype:      return "error ctype\r\n���ɐ������Ȃ������N���X�������݂��܂��B";
	case error_escape:     return "error escape\r\n���ɐ������Ȃ��G�X�P�[�v �V�[�P���X�����݂��܂��B";
	case error_backref:    return "error back reference\r\n���ɐ������Ȃ��O���Q�Ƃ����݂��܂��B";
	case error_brack:      return "error bracket\r\n���� '[' �܂��� ']' �̑Ή�������������܂���B";
	case error_paren:      return "error paren\r\n���� '(' �܂��� ')' �̑Ή�������������܂���B";
	case error_brace:      return "error brace\r\n���� '{' �܂��� '}' �̑Ή�������������܂���B";
	case error_badbrace:   return "error bad brace\r\n���� { } ���ɐ������Ȃ��񐔂��܂�ł��܂��B";
	case error_range:      return "error range\r\n���ɐ������Ȃ������͈͎w��q�����݂��܂��B";
	case error_space:      return "error space\r\n���\�[�X�s���������Ő��K�\���̉�͂Ɏ��s���܂����B";
	case error_badrepeat:  return "error bad repeat\r\n�J��Ԃ��� (��ʂɁA'*'�A''�A'+'�A'{' �̂����ꂩ) �̑O�Ɏ����w�肳��Ă��܂���B";
	case error_complexity: return "error complexity\r\n�����������G�����邽�߂Ɏ��s���܂����B";
	case error_stack:	   return "error stack\r\n�������s���������Ō����Ɏ��s���܂����B";
	case error_parse:	   return "error parse\r\n���̉�͂Ɏ��s���܂����B";
	default:
		return "���K�\��\r\n���̑��̃G���[";
		//throw std::invalid_argument("invalid error code");
	}
}


BOOL Regex::Verify(CString strSource, CString strRegex, CString strReplace)
{
	CString tmpSource, tmpSearch, tmpReplace, strResult;
	BOOL bResult=TRUE;
	tmpSearch = TranslateEscape(strRegex);
	tmpReplace = TranslateEscape(strReplace);

	try
	{
		std::regex rx(tmpSearch);
		std::string str(strSource);
		std::string fmt(tmpReplace);
		std::string result;
		result = std::regex_replace(str, rx, fmt);
		strResult = result.c_str();
	}
	catch (std::regex_error& e)
	{
		bResult = FALSE;
	}
	return bResult;
}
