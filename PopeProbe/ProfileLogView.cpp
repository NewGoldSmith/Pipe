#include "pch.h"
#include "ProfileLogView.h"

ProfileLogView::ProfileLogView():Profile()
,m_bShowLogWindow(TRUE)
,m_bFormatDown(FALSE)
,m_strSearchDown(_T(""))
,m_strReplaceDown(_T(""))
,m_bFormatUp(FALSE)
,m_strSearchUp(_T(""))
,m_strPeplaceUp(_T(""))
,m_iMaxLine(10)
{

}
void ProfileLogView::LoadProfile()
{
	CWinApp* pApp;
	pApp = AfxGetApp();
	CString strSec = _T("LOG_VIEW");
	m_bShowLogWindow = pApp->GetProfileInt(strSec, _T("SHOW"), 0);
	m_bFormatDown = pApp->GetProfileInt(strSec, _T("FORMAT_DOWN"), 0);
	m_strSearchDown = pApp->GetProfileString(strSec, _T("SEARCH_DOWN"), _T("(^)(.*)(\\r?)(\\n?)($)"));
	m_strReplaceDown = pApp->GetProfileString(strSec, _T("REPLACE_DOWN"), _T("->$1$2$3$4"));
	m_bFormatUp = pApp->GetProfileInt(strSec, _T("FORMAT_UP"), 0);
	m_strSearchUp = pApp->GetProfileString(strSec, _T("SEARCH_UP"), _T("(^)(.*)(\\r?)(\\n?)($)"));
	m_strPeplaceUp = pApp->GetProfileString(strSec, _T("REPLACE_UP"), _T("<-$1$2$3$4"));
	m_iMaxLine = pApp->GetProfileInt(strSec, _T("MAX_LINE"), 10);
}