#pragma once
#include "CShowInfoPrompt.h"
#include <string>

class CShowLicenseMIT :
    public CShowInfoPrompt
{
public:
    CShowLicenseMIT();
    virtual void ShowPrompt();
    std::string SetAppName(std::string strApp);
    std::string SetYearly(std::string strYearly);
protected:
    std::string m_strYearly;
    std::string m_strAppName;
    std::string m_strCopyright=R"(Copyright (c) )" ;
    std::string m_strDescription = 
R"(Gold Smith.
Released under the MIT license.
https://opensource.org/licenses/mit-license.php
)";

};

