#pragma once
#include <Windows.h>
#include <ioapiset.h>
#include <queue>
#include <semaphore>
#include <thread>
#include <stdio.h>
#include <CBinaryString.h>
#include <PipeHelper.h>

class CStdPipe
{
public:
	CStdPipe();
	~CStdPipe();
	size_t Read(CBinaryString& BD);
	size_t Write(const CBinaryString& BD);

	struct stThreadParam
	{
		std::string INCommand = "";
		bool bForcedTermination = false;
		bool bEndFlag = false;
		int iRes;
		CBinaryString* pBD;
		std::queue<CBinaryString*, std::deque<CBinaryString*>> ReadQue;
		std::queue<CBinaryString*, std::deque<CBinaryString*>> PoolQue;
		std::counting_semaphore<1> MtxRead{ 1 };
		std::counting_semaphore<1> MtxPool{ 1 };
		std::thread* pThisThread;
	} ;
	stThreadParam m_ThreadParam;
	bool InitPipeWork();//ループ開始
	bool FinPipeWork();

protected:
	void static PipeThread(stThreadParam* pParam);	//標準入力スレッド
};
