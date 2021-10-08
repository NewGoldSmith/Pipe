#include "pch.h"
#include "CLogFile.h"
CLogFile::CLogFile(ProfileLogFile *pprofLogFile)
{
	m_pProfileLogFile=pprofLogFile;
}
CLogFile::~CLogFile()
{
	CFile::~CFile();
}

BOOL CLogFile::Open()
{
	m_pProfileLogFile.
	return 0;
}
