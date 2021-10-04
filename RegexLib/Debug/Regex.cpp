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
	case error_collate:    return "error collapse\r\n式に正しくない照合要素名が存在します。";
	case error_ctype:      return "error ctype\r\n式に正しくない文字クラス名が存在します。";
	case error_escape:     return "error escape\r\n式に正しくないエスケープ シーケンスが存在します。";
	case error_backref:    return "error back reference\r\n式に正しくない前方参照が存在します。";
	case error_brack:      return "error bracket\r\n式で '[' または ']' の対応が正しくありません。";
	case error_paren:      return "error paren\r\n式で '(' または ')' の対応が正しくありません。";
	case error_brace:      return "error brace\r\n式で '{' または '}' の対応が正しくありません。";
	case error_badbrace:   return "error bad brace\r\n式が { } 式に正しくない回数を含んでいます。";
	case error_range:      return "error range\r\n式に正しくない文字範囲指定子が存在します。";
	case error_space:      return "error space\r\nリソース不足が原因で正規表現の解析に失敗しました。";
	case error_badrepeat:  return "error bad repeat\r\n繰り返し式 (一般に、'*'、''、'+'、'{' のいずれか) の前に式が指定されていません。";
	case error_complexity: return "error complexity\r\n検索式が複雑すぎるために失敗しました。";
	case error_stack:	   return "error stack\r\nメモリ不足が原因で検索に失敗しました。";
	case error_parse:	   return "error parse\r\n式の解析に失敗しました。";
	default:
		return "正規表現\r\nその他のエラー";
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
