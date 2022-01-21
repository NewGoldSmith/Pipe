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

class CPipeTerm:public CBStream
{
public:
	CPipeTerm();
	virtual ~CPipeTerm();
	virtual int Read(CBinaryString& BD);
	virtual int Write(const CBinaryString& BD);

	bool InitPipeWork(HANDLE hPipeIn,HANDLE hPipeOut);//ループ開始
	bool FinPipeWork();
	virtual void SetEventHandler(type_pStreamEventHandler pEvent);
	virtual void ClearEventHandler();
protected:
	struct stThreadParam
	{
		std::string INCommand = "";
		HANDLE hPipe{nullptr};
		bool bForcedTermination = false;
		CBinaryString* pBD{nullptr};
		std::queue<CBinaryString*, std::deque<CBinaryString*>> ReadQue;
		std::queue<CBinaryString*, std::deque<CBinaryString*>> PoolQue;
		std::counting_semaphore<1> MtxRead{1};
		std::counting_semaphore<1> MtxPool{1};
		std::counting_semaphore<1> MtxEvent{ 1 };
		std::thread* pThisThread{nullptr};
		int len{ 0 };
		bool *pIsConnect=nullptr;
		type_pStreamEventHandler *ppEvOnDisconnect= nullptr;
	} ;
	stThreadParam m_ThreadParam;
	void static PipeThread(stThreadParam* pParam);	//標準入力スレッド
	HANDLE m_PipeOut;
};
