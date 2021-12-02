#include "CommandLine.h"


CommandLine::CommandLine(int argc, char** argv)
{
    m_argc = argc;
    m_argv = argv;
}

void CommandLine::ParseCommand(CommandLineInfo* pCmdInfo)
{
	for (int i = 1; i < m_argc; i++)
	{
		char * pszParam = m_argv[i];
		bool bFlag = false;
		bool bLast = ((i + 1) == m_argc);
		if (pszParam[0] == '-' || pszParam[0] == '/')
		{
			// remove flag specifier
			bFlag = true;
			++pszParam;
		}
		pCmdInfo->ParseParam(pszParam, bFlag, bLast);
	}
}
