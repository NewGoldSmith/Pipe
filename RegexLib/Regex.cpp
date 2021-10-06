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
	case error_collate:    return _T("error collapse\r\n式に正しくない照合要素名が存在します。");
	case error_ctype:      return _T("error ctype\r\n式に正しくない文字クラス名が存在します。");
	case error_escape:     return _T("error escape\r\n式に正しくないエスケープ シーケンスが存在します。");
	case error_backref:    return _T("error back reference\r\n式に正しくない前方参照が存在します。");
	case error_brack:      return _T("error bracket\r\n式で '[' または ']' の対応が正しくありません。");
	case error_paren:      return _T("error paren\r\n式で '(' または ')' の対応が正しくありません。");
	case error_brace:      return _T("error brace\r\n式で '{' または '}' の対応が正しくありません。");
	case error_badbrace:   return _T("error bad brace\r\n式が { } 式に正しくない回数を含んでいます。");
	case error_range:      return _T("error range\r\n式に正しくない文字範囲指定子が存在します。");
	case error_space:      return _T("error space\r\nリソース不足が原因で正規表現の解析に失敗しました。");
	case error_badrepeat:  return _T("error bad repeat\r\n繰り返し式 (一般に、'*'、''、'+'、'{' のいずれか) の前に式が指定されていません。");
	case error_complexity: return _T("error complexity\r\n検索式が複雑すぎるために失敗しました。");
	case error_stack:	   return _T("error stack\r\nメモリ不足が原因で検索に失敗しました。");
	case error_parse:	   return _T("error parse\r\n式の解析に失敗しました。");
	default:
		return _T("正規表現\r\nその他のエラー");
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
