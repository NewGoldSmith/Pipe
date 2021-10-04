#pragma once
#include "Profile.h"
class ProfileLogFile :
    public Profile
{
public:
    void LoadProfile();
	CString m_strLogDir;
	BOOL m_bFormatDown;
	CString m_strSearchDown;
	CString m_strReplaceDown;
	BOOL m_bFormatUp;
	CString m_strSearchUp;
	CString m_strReplaceUp;
	BOOL m_bWriteCommand;
	BOOL m_bMakeLogFile;
	CString m_strNameFormat;
	CString MakeFilename(CString strNameFormat);
//	HANDLE CReateLogFile();
};

