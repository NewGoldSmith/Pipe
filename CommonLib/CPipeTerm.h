//Copyright (c) 2021, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php
#pragma once
#include <Windows.h>
#include <ioapiset.h>
#include <queue>
#include <semaphore>
#include <thread>
#include <stdio.h>
#include <CBinaryString.h>
#include <PipeHelper.h>
#include <fcntl.h>
#include <CBStream.h>
#include <vector>

class CPipeTerm:public CBStream
{
public:
	//Specify the number and size of the container of the in-buffer. 
	CPipeTerm(unsigned int NumCont=4,unsigned int ContSize=8192);
	virtual ~CPipeTerm();
	virtual int Read(CBinaryString& BD);
	virtual int Write(const CBinaryString& BD);

	virtual bool Connect();
	virtual void SetHandle(HANDLE hPipeIn, HANDLE hPipeOut);
	virtual bool Disconnect();
	virtual void SetEventHandler(type_pStreamEventHandler pEvent);
	virtual void ClearEventHandler();
protected:
	struct stThreadParam
	{
		stThreadParam();
		HANDLE hPipeIn{nullptr};
		bool bForcedTermination;
		CBinaryString* pBD;
		std::queue<CBinaryString*, std::deque<CBinaryString*>> ReadQue;
		std::queue<CBinaryString*, std::deque<CBinaryString*>> PoolQue;
		std::counting_semaphore<1> MtxRead;
		std::counting_semaphore<1> MtxPool;
		std::counting_semaphore<1> MtxEvent;
		std::thread* pThisThread;
		int len;
		bool *pIsConnect;
		type_pStreamEventHandler *ppEvOnDisconnect;
	} ;
	stThreadParam m_ThreadParam;
	void static PipeThread(stThreadParam* pParam);
	HANDLE m_PipeOut;
	unsigned int m_NumInCont;
	unsigned int m_SizeInCont;
	std::vector<CBinaryString*> m_StringPointerWarehouse;
};
