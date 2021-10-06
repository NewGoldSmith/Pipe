#pragma once
#include <afxwin.h>
class WCmdCLiantOption :
	public CCommandLineInfo
{
public:
	WCmdCLiantOption();
	virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
	BOOL m_bEchoCommand;
	BOOL m_bDebug;
};

