#include "CStdPipe.h"
#include <strsafe.h>
#include <signal.h>
#include <iostream>
using namespace std;




CStdPipe::CStdPipe()
{
	m_ThreadParam.pBD = nullptr;
	m_ThreadParam.bEndFlag = false;
	m_ThreadParam.bForcedTermination = false;
	m_ThreadParam.INCommand = "";
	m_ThreadParam.iRes = 0;
	m_ThreadParam.pThisThread=nullptr;
}

CStdPipe::~CStdPipe()
{
}


bool CStdPipe::InitPipeWork()
{
	CTRL_C_EVENT;
//	int rval=	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_PROCESSED_INPUT);
	//SetConsoleCtrlHandler(NULL, TRUE);
	//signal(SIGINT, CallbackSingnalControl);
	for (int i = 0; i <= 2; i++)
	{
		m_ThreadParam.PoolQue.push(new CBinaryString(1024));
	}
	//コンソール入出力待ちスレッド起動
	m_ThreadParam.pThisThread = new std::thread(&PipeThread, &m_ThreadParam);
	return true;
}

bool CStdPipe::FinPipeWork()
{

	//キューの中身を削除
	m_ThreadParam.MtxPool.acquire();
	while (!m_ThreadParam.PoolQue.empty())
	{
		delete m_ThreadParam.PoolQue.front();
		m_ThreadParam.PoolQue.pop();
	}
	m_ThreadParam.MtxPool.release();
	m_ThreadParam.MtxRead.acquire();
	while (!m_ThreadParam.ReadQue.empty())
	{
		delete m_ThreadParam.ReadQue.front();
		m_ThreadParam.ReadQue.pop();
	}
	m_ThreadParam.MtxRead.release();
	//コンソール入力待ちのスレッドを終了。
	m_ThreadParam.bForcedTermination = true;
	HANDLE hThread=m_ThreadParam.pThisThread->native_handle();
	int rVal = CancelSynchronousIo(hThread);
	//スレッドが終了するまで待ち。
	m_ThreadParam.pThisThread->join();

	return true;
}


void CStdPipe::PipeThread(stThreadParam* pParam)
{
	using namespace PipeHelper;

	while (!pParam->bForcedTermination)
	{
		pParam->MtxPool.acquire();
		if (pParam->PoolQue.empty())
		{
			pParam->MtxPool.release();
			break;
		}
		pParam->pBD = pParam->PoolQue.front();
		pParam->PoolQue.pop();
		pParam->MtxPool.release();
		int len= StdRead(*(pParam->pBD));
		if (len == 0)
		{
			pParam->bEndFlag = true;
		}
		pParam->MtxRead.acquire();
		pParam->ReadQue.push(pParam->pBD);
		pParam->MtxRead.release();
		Sleep(100);
	}
	_flushall();
}

size_t CStdPipe::Read(CBinaryString& outBD)
{
	outBD.SetDataSize(0);
	m_ThreadParam.MtxRead.acquire();
	if (m_ThreadParam.ReadQue.empty())
	{
		m_ThreadParam.MtxRead.release();
	}
	else 
	{
		CBinaryString* pBD;
		while (!m_ThreadParam.ReadQue.empty())
		{
			pBD = m_ThreadParam.ReadQue.front();
			
			outBD += *pBD;
			m_ThreadParam.ReadQue.pop();

			m_ThreadParam.MtxPool.acquire();
			m_ThreadParam.PoolQue.push(pBD);
			m_ThreadParam.MtxPool.release();
		}
		m_ThreadParam.MtxRead.release();

	}
	return outBD.GetDataSize();
}

size_t CStdPipe::Write(const CBinaryString& BD)
{
	return PipeHelper::StdWrite(BD);
	//return true;
}

