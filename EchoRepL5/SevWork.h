#pragma once
#include <SocketHelper.h>
#include "EchoRepL5.h"

namespace MainSevWork
{
	bool SevWork();
	bool SevArrayWork(std::vector<SOCKET>& SocketArray);
}