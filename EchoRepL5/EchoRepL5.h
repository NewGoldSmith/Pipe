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

//���[�h�E���C�g���[�v�B
//�T�[�o�[�͑S�Ă̐ڑ��ς݃\�P�b�g�Ɉ�񂸂B
bool SevArrayWork(std::vector<SOCKET>& SocketArray);
//�N���C�A���g�͐ؒf����܂Ń��[�v�B
bool CliReadWriteWork(SOCKET clisoc);

