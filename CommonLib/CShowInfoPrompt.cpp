
#include "CShowInfoPrompt.h"
#include <atlchecked.h>



CShowInfoPrompt::CShowInfoPrompt():
	m_Text("")
	,m_TextSize(0)
{
}

CShowInfoPrompt::~CShowInfoPrompt()
{
}

void CShowInfoPrompt::ShowPrompt()
{
	if (!m_Text.empty())
	{
		std::cout << m_Text;
	}
}


int CShowInfoPrompt::GetText(char* pDst, size_t DstSize)
{
	if (m_TextSize > DstSize)
	{
		return -1;
	}
	if (m_Text.empty() )
	{
		return -1;
	}
	strcpy_s(pDst, m_TextSize, m_Text.c_str());
	return 0;
}
