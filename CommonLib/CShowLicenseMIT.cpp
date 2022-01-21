#include "CShowLicenseMIT.h"

CShowLicenseMIT::CShowLicenseMIT():CShowInfoPrompt()
,m_strYearly{"2021,2022"}
,m_strAppName{""}
{
}

void CShowLicenseMIT::ShowPrompt()
{
	std::string str=m_strAppName+"\r\n"+m_strCopyright +m_strYearly+" " + m_strDescription;
	m_TextSize = str.size();
	m_Text = str;
	CShowInfoPrompt::ShowPrompt();
}

std::string CShowLicenseMIT::SetAppName(std::string strApp)
{
	std::string strOld = m_strAppName;
	m_strAppName = strApp;
	return strOld;
}

std::string CShowLicenseMIT::SetYearly(std::string strYearly)
{
	std::string strOld = m_strYearly;
	m_strYearly = strYearly;
	return strOld;
}

