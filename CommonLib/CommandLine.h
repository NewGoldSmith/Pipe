#pragma once
#include <string>
#include <CommandLineInfo.h>
class CommandLineInfo;
class CommandLine
{
public:
	CommandLine(int argc, char** argv);
	void ParseCommand(CommandLineInfo* pCmdInfo);
protected:
	int m_argc;
	char** m_argv;
};

