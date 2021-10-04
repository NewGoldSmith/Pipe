#pragma once
#include <afxwin.h>
class CCommandOption :
    public CCommandLineInfo
{
    public:
    virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
    BOOL m_bEchoCommand;
};

