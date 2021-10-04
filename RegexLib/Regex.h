#pragma once
#include <atlstr.h>
#include <string>
#include <regex>

class Regex
{
public:
	CString ExecuteRegex(CString strSource, CString strRegex, CString strReplace);
	CString TranslateEscape(CString strSource);
	CString InvTranslateEscape(CString strSource);
protected:
	std::string code_to_string(std::regex_constants::error_type e);
public:
	BOOL Verify(CString strSource, CString strRegex, CString strPeplace);
};

