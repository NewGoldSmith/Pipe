#pragma once
#include "CommandLine.h"
class CommandLine;
class CommandLineInfo
{
public:
	void virtual ParseParam(char* p_command, bool flag,bool last);
};

