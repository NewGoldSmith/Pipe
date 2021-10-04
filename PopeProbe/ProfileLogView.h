#pragma once
#include "Profile.h"

class ProfileLogView :
    public Profile
{
public:
	ProfileLogView();
	void virtual LoadProfile();
	BOOL m_bShowLogWindow;
	BOOL m_bFormatDown;
	CString m_strSearchDown;
	CString m_strReplaceDown;
	BOOL m_bFormatUp;
	CString m_strSearchUp;
	CString m_strPeplaceUp;
	int m_iMaxLine;
};

