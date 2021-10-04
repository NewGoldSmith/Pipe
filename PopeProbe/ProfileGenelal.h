#pragma once
#include "Profile.h"
class ProfileGenelal :
    public Profile
{
public:
    void LoadProfile();
    CString m_strChildPathName;
    CString m_strPipeProbeDistinationPathName;
    CString m_strPipeprobeSourcePathname;
//    int m_iStartupWindow;
//    int m_iStartupIcon;
//    int m_iStartupTasktray;
    int m_iMainWindowShowStatus;
};

