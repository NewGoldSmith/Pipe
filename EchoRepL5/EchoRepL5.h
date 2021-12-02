#pragma once
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include "CommandLine.h"
#include "CommandOpEchoRepL5.h"
#include "CShowLicenseMIT.h"
#include "CShowHelp.h"
#include <SocketHelper.h>
#include "stdPipeDetectEnd.h"
#include "CliWork.h"
#include "SevWork.h"

//リード・ライトループ。
//サーバーは全ての接続済みソケットに一回ずつ。
bool SevArrayWork(std::vector<SOCKET>& SocketArray);
//クライアントは切断するまでループ。
bool CliReadWriteWork(SOCKET clisoc);

