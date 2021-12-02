#pragma once

#include <tchar.h>
#include <string.h>
#include <stdio.h>

class CShowInfoPrompt
{
public:
	CShowInfoPrompt();
	virtual ~CShowInfoPrompt();
	virtual void ShowPrompt();
	virtual int GetText(char* pDst, size_t DstSize);
protected:
	char *m_pText;
	size_t m_TextSize;
};
