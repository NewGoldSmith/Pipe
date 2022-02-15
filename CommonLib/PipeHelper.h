#pragma once
#include <Windows.h>
#include <stdio.h>
#include <io.h>
#include <CBinaryString.h>
#include <fcntl.h>
#include <wtypes.h>

namespace PipeHelper 
{

	int StdRead(CBinaryString& bd);
	int StdWrite(const CBinaryString& bd);
	int HRead(HANDLE hPipeIn, CBinaryString&bd);
	int HWrite(HANDLE hPipeOut,const CBinaryString&bd);
}
