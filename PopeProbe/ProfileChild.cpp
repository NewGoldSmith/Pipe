#include "pch.h"
#include "ProfileChild.h"


void ProfileChild::LoadProfile()
{
	CWinApp* pApp;
	pApp = AfxGetApp();
	CString strSec = _T("CHILD");
	m_bAddtionalCommandOptionFront = pApp->GetProfileInt(strSec, _T("USE_ADDTIONAL_COMMAND_FRONT"), FALSE);
	m_strAddtionalCommandOptionFront = pApp->GetProfileString(strSec, _T("COMMAND_FRONT"), _T(""));
	m_bAddtionalCommandOptionBack = pApp->GetProfileInt(strSec, _T("USE_ADDTIONAL_COMMAND_BACK"), FALSE);
	m_strAddtionalCommandOptionBack = pApp->GetProfileString(strSec, _T("COMMAND_BACK"), _T(""));
	m_bIgnoreParentCommandOption = pApp->GetProfileInt(strSec, _T("IGNORE_PARENT_COMMAND"), FALSE);
}


CString ProfileChild::CreateCommandLine(CString strPathName)
{
	CWinApp* pApp = AfxGetApp();
	if (m_bAddtionalCommandOptionFront != FALSE)
	{
		strPathName += _T(" ") + m_strAddtionalCommandOptionFront;
	}
	if (m_bIgnoreParentCommandOption == FALSE)
	{
		CString str = pApp->m_lpCmdLine;
		str.Replace(_T("\/PPDebug"),_T(""));
		strPathName += _T(" ") + str;
	}
	if (m_bAddtionalCommandOptionBack != FALSE)
	{
		strPathName += _T(" ") + m_strAddtionalCommandOptionBack;
	}
	return strPathName;
}
