#pragma once
#include <afxwin.h>
namespace WCmdCLiant {
	class WCmdCLiantOption :
		public CCommandLineInfo
	{
	public:
		WCmdCLiantOption();
		virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
		BOOL m_bEchoCommand;
		BOOL m_bDebug;
	};

}