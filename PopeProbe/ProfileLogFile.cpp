#include "pch.h"
#include "ProfileLogFile.h"


void ProfileLogFile::LoadProfile()
{
	CWinApp* pApp;
	pApp = AfxGetApp();
	CString strSec = _T("LOG_FILE");
	m_strLogDir = pApp->GetProfileString(strSec, _T("DIR"), _T(""));
	m_bFormatDown = pApp->GetProfileInt(strSec, _T("FORMAT_DOWN"), 0);
	m_strSearchDown = pApp->GetProfileString(strSec, _T("SEARCH_DOWN"), _T("(^)(.*)(\\r?)(\\n?)($)"));
	m_strReplaceDown = pApp->GetProfileString(strSec, _T("REPLACE_DOWN"), _T("->$1$2$3$4"));
	m_bFormatUp = pApp->GetProfileInt(strSec, _T("FORMAT_UP"), 0);
	m_strSearchUp = pApp->GetProfileString(strSec, _T("SEARCH_UP"), _T("(^)(.*)(\\r?)(\\n?)($)"));
	m_strReplaceUp = pApp->GetProfileString(strSec, _T("REPLACE_UP"), _T("<-$1$2$3$4"));
	m_bWriteCommand = pApp->GetProfileInt(strSec, _T("LEAD_WRITE_COMMAND"), 0);
	m_bMakeLogFile = pApp->GetProfileInt(strSec, _T("CREATE_LOG"), 0);
	m_strNameFormat = pApp->GetProfileString(strSec, _T("NAME_FORMAT"), _T("Pipe%y-%m-%d-%S?:.log"));
}


CString ProfileLogFile::MakeFilename(CString strNameFormat)
{
	USES_CONVERSION;
	CString strName;
	time_t osBinaryTime;  // C run-time time (defined in <time.h>)
	time(&osBinaryTime);
	CTime ctime(osBinaryTime);
	strName = ctime.Format(m_strNameFormat);
	LPWSTR pwStr = T2W(strName.GetBuffer());
	strName.ReleaseBuffer();
	int i = PathCleanupSpec(NULL, pwStr);
	strName = W2T(pwStr);
	return strName;
}

