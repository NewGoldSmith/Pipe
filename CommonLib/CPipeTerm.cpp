#include "CPipeTerm.h"
#include <strsafe.h>
#include <signal.h>
#include <iostream>
using namespace std;




CPipeTerm::CPipeTerm():CBStream()
,m_PipeOut{nullptr}
{
	m_ThreadParam.ppEvOnDisconnect = &(this->m_pEvOnDisconnect);
}

CPipeTerm::~CPipeTerm()
{
	if (m_ThreadParam.pThisThread != nullptr)
	{
		FinPipeWork();
	}
}


bool CPipeTerm::InitPipeWork(HANDLE hPipeIn, HANDLE hPipeOut)
{
	m_ThreadParam.bForcedTermination=false;
	m_ThreadParam.MtxPool.acquire();
	while (!m_ThreadParam.PoolQue.empty())
	{
		CBinaryString* pBD = m_ThreadParam.PoolQue.front();
		m_ThreadParam.PoolQue.pop();
		delete pBD;
	}
	m_ThreadParam.MtxPool.release();
	m_ThreadParam.MtxRead.acquire();
	while (!m_ThreadParam.ReadQue.empty())
	{
		CBinaryString* pBD = m_ThreadParam.ReadQue.front();
		m_ThreadParam.ReadQue.pop();
		delete pBD;
	}
	m_ThreadParam.MtxRead.release();
	//	m_ThreadParam.bEndFlag = false;
	m_PipeOut = hPipeOut;
	m_ThreadParam.hPipe = hPipeIn;
	m_ThreadParam.pIsConnect = &m_bConnected;
	for (int i = 0; i <= 2; i++)
	{
		m_ThreadParam.PoolQue.push(new CBinaryString(1024));
	}
	m_bConnected = true;
	//コンソール入出力待ちスレッド起動
	m_ThreadParam.pThisThread = new std::thread(&PipeThread, &m_ThreadParam);
	return true;
}

bool CPipeTerm::FinPipeWork()
{
	//パイプ入力待ちのスレッドを終了。
	m_ThreadParam.bForcedTermination = true;
	HANDLE hThread=m_ThreadParam.pThisThread->native_handle();
	int rVal = CancelSynchronousIo(hThread);
	//スレッドが終了するまで待ち。
	m_ThreadParam.pThisThread->join();
	delete m_ThreadParam.pThisThread;
	m_ThreadParam.pThisThread = nullptr;

	//キューの中身を削除
	m_ThreadParam.MtxPool.acquire();
	while (!m_ThreadParam.PoolQue.empty())
	{
		CBinaryString* pBD = m_ThreadParam.PoolQue.front();
		m_ThreadParam.PoolQue.pop();
		delete pBD;
	}
	m_ThreadParam.MtxPool.release();
	m_ThreadParam.MtxRead.acquire();
	while (!m_ThreadParam.ReadQue.empty())
	{
		CBinaryString*pBD = m_ThreadParam.ReadQue.front();
		m_ThreadParam.ReadQue.pop();
		delete pBD;
	}
	m_ThreadParam.MtxRead.release();
	m_bConnected = false;
	return true;
}

void CPipeTerm::SetEventHandler(type_pStreamEventHandler pEvent)
{
	m_ThreadParam.MtxEvent.acquire();
	m_pEvOnDisconnect = pEvent;
	m_ThreadParam.MtxEvent.release();
}

void CPipeTerm::ClearEventHandler()
{
	m_ThreadParam.MtxEvent.acquire();
	m_pEvOnDisconnect = nullptr;
	m_ThreadParam.MtxEvent.release();
}

void CPipeTerm::PipeThread(stThreadParam* pParam)
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
		pParam->len = _read(_open_osfhandle((intptr_t)pParam->hPipe, _O_RDONLY), pParam->pBD->GetBuffer8(), pParam->pBD->GetBufSize());
		if (pParam->len != -1)
		{
			pParam->pBD->SetDataSize(pParam->len);
		}

		pParam->MtxRead.acquire();
		pParam->ReadQue.push(pParam->pBD);
		pParam->MtxRead.release();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	//終了処理
	_flushall();
	*pParam->pIsConnect = false;
	pParam->MtxEvent.acquire();
	if (*(pParam->ppEvOnDisconnect) != nullptr)
	{
		 (* (pParam->ppEvOnDisconnect))();
	}
	pParam->MtxEvent.release();
}

int CPipeTerm::Read(CBinaryString& outBD)
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

int CPipeTerm::Write(const CBinaryString& BD)
{
	return PipeHelper::HWrite(m_PipeOut,BD);
}

