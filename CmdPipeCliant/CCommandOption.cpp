#include "CCommandOption.h"
void CCommandOption::ParseParam( const TCHAR* pszParam,  BOOL bFlag,  BOOL bLast)
{
	if (pszParam == CString("EchoCommand"))
	{
		m_bEchoCommand = TRUE;
	}
	CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
}