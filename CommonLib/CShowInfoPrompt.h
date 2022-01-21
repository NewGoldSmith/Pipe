#pragma once

#include <string>
#include <iostream>

class CShowInfoPrompt
{
public:
	CShowInfoPrompt();
	virtual ~CShowInfoPrompt();
	virtual void ShowPrompt();
	virtual int GetText(char* pDst, size_t DstSize);
protected:
	std::string m_Text;
	size_t m_TextSize;
};
