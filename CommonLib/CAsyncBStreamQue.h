//Copyright(c) 2021 Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php
#pragma once
#include <CPipeTerm.h>
#include <CJointStream.h>
class BStreamQue
{
public:
	BStreamQue(unsigned int numCont=10,unsigned int sizeCnt=8194);
	virtual ~BStreamQue();
	CBStream* GetStreamEndA();
	CBStream* GetStreamEndB();
	void StartWork();
	void StopWork();
protected:
	CPipeTerm* m_pBStreamA;
	CPipeTerm* m_pBStreamB;
	CJointStream m_JointStream;
	HANDLE m_hAtoBRead;
	HANDLE m_hAtoBWrite;
	HANDLE m_hBtoARead;
	HANDLE m_hBtoAWrite;
};

