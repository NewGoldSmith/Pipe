#include "CShowLicenseMIT.h"

CShowLicenseMIT::CShowLicenseMIT():CShowInfoPrompt()
{
	const char p[] = R"(

Echo layer 5 

Copyright (c) 2021 Gold Smith.
Released under the MIT license.
https://opensource.org/licenses/mit-license.php
)";
	m_TextSize = strnlen(p, 1024);
	m_pText = new char[m_TextSize+1];
	int err =strcpy_s(m_pText, m_TextSize+1, p);

}

