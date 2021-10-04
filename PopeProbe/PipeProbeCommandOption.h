#pragma once
#include <afxwin.h>
class PipeProbeCommandOption :
    public CCommandLineInfo
{
public:
    PipeProbeCommandOption();
    virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
    BOOL m_bDebug;
    BOOL m_bSW_SHOWMINIMIZED;
};

