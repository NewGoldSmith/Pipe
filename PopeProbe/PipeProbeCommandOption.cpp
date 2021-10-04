#include "pch.h"
#include "PipeProbeCommandOption.h"


PipeProbeCommandOption::PipeProbeCommandOption() :CCommandLineInfo()
, m_bDebug(FALSE)
, m_bSW_SHOWMINIMIZED(FALSE)
{
    
}

void PipeProbeCommandOption::ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast)
{
    if (!_tcsicmp(pszParam, _T("PPDebug")))
    {
        m_bDebug = TRUE;
    }
    if (!_tcsicmp(pszParam, _T("MIN")))
    {
        m_bSW_SHOWMINIMIZED = TRUE;
    }

        CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
}
