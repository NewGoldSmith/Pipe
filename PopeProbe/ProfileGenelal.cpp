#include "pch.h"
#include "ProfileGenelal.h"


void ProfileGenelal::LoadProfile()
{
	CWinApp* pApp;
	pApp = AfxGetApp();
	CString strSec = _T("GENERAL");
	m_strChildPathName = pApp->GetProfileString(strSec, _T("CHILD_PATHNAME"), _T(""));
	m_strPipeProbeDistinationPathName = pApp->GetProfileString(strSec, _T("PIPEPROBE_PLACEMENT_PATHNAME"), _T(""));
	m_strPipeprobeSourcePathname = pApp->GetProfileString(strSec, _T("PIPEPROBE_SOURCE_PATHNAME"), _T(""));
	m_iMainWindowShowStatus = pApp->GetProfileInt(strSec, _T("SHOW_STATUS"), 0);
}
