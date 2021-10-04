#pragma once
#include "Profile.h"
class ProfileChild :
    public Profile
{
public:
    void LoadProfile();
	BOOL m_bAddtionalCommandOptionFront;
	CString m_strAddtionalCommandOptionFront;
	BOOL m_bAddtionalCommandOptionBack;
	CString m_strAddtionalCommandOptionBack;
	BOOL m_bIgnoreParentCommandOption;
	CString CreateCommandLine(CString strPathName);
};

