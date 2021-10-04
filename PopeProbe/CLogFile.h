#pragma once
#include <afx.h>
#include "ProfileLogFile.h"
class CLogFile :
    protected CFile
{
public:
    CLogFile(ProfileLogFile *pprofLogFile);
    ~CLogFile();
protected:
    ProfileLogFile* m_pProfileLogFile;
public:
    BOOL Open();
};

