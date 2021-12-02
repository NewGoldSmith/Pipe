
#include "CShowInfoPrompt.h"
#include <atlchecked.h>


CShowInfoPrompt::CShowInfoPrompt():
	m_pText(nullptr)
	,m_TextSize(0)
{
}

CShowInfoPrompt::~CShowInfoPrompt()
{
	delete [] m_pText;
}

void CShowInfoPrompt::ShowPrompt()
{
	if (m_pText != nullptr)
	{
		printf_s("%s", m_pText);
	}
}


int CShowInfoPrompt::GetText(char* pDst, size_t DstSize)
{
	if (m_TextSize > DstSize)
	{
		return -1;
	}
	if (m_pText == nullptr)
	{
		return -1;
	}
	strcpy_s(pDst, m_TextSize, m_pText);
	return 0;
}
