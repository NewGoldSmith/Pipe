#include "pch.h"
#include "WCmdCLiantOption.h"
namespace WCmdCLiant {
	WCmdCLiantOption::WCmdCLiantOption() :CCommandLineInfo()
	{
		m_bDebug = 0;
		m_bEchoCommand = 0;
	}
	void WCmdCLiantOption::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
	{
		if (!CString("Debug").CompareNoCase(pszParam))
		{
			m_bDebug = TRUE;
		}
		CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
	}
}