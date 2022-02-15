//Copyright (c) 2021, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php

#include "CPipeTerm.h"
#include <strsafe.h>
#include <signal.h>
#include <iostream>
using namespace std;

CPipeTerm::stThreadParam::stThreadParam()
	: hPipeIn(nullptr)
	, bForcedTermination(false)
	, pBD( nullptr)
	, ReadQue()
	, PoolQue()
	, MtxRead{ 1 }
	, MtxPool{ 1 }
	, MtxEvent{ 1 }
	, pThisThread(nullptr)
	, len( 0 )
	, pIsConnect(nullptr)
	, ppEvOnDisconnect(nullptr)
{
}

CPipeTerm::CPipeTerm(unsigned int NumCont, unsigned int ContSize)
	:CBStream()
	, m_ThreadParam()
	, m_PipeOut(nullptr)
{
	m_NumInCont = NumCont;
	if (NumCont < 1)
	{
		m_NumInCont = 1;
	}
	m_SizeInCont = ContSize;
	if (ContSize<14)
	{
		m_SizeInCont = 14;
	}
	m_ThreadParam.ppEvOnDisconnect = &(this->m_pEvOnDisconnect);
}

CPipeTerm::~CPipeTerm()
{
	if (m_ThreadParam.pThisThread != nullptr)
	{
		Disconnect();
	}
}


bool CPipeTerm::Connect()
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

	m_ThreadParam.pIsConnect = &m_bConnected;
	for (unsigned int i = 0; i <= m_NumInCont; i++)
	{
		CBinaryString * pBstr=(CBinaryString *) new CBinaryString(m_SizeInCont);
		m_ThreadParam.PoolQue.push(pBstr);
		m_StringPointerWarehouse.push_back(pBstr);
	}
	m_bConnected = true;

	m_ThreadParam.pThisThread = new std::thread(&PipeThread, &m_ThreadParam);
	return true;
}


void CPipeTerm::SetHandle(HANDLE hPipeIn, HANDLE hPipeOut)
{
	m_ThreadParam.hPipeIn = hPipeIn;
	m_PipeOut = hPipeOut;
}

bool CPipeTerm::Disconnect()
{
	if (m_ThreadParam.pThisThread == nullptr)
	{
		return true;
	}

	m_ThreadParam.bForcedTermination = true;
	HANDLE hThread=m_ThreadParam.pThisThread->native_handle();
	int rVal = CancelSynchronousIo(hThread);

	m_ThreadParam.pThisThread->join();
	delete m_ThreadParam.pThisThread;
	m_ThreadParam.pThisThread = nullptr;

	m_ThreadParam.MtxPool.acquire();
	while (!m_ThreadParam.PoolQue.empty())
	{
		CBinaryString* pBD = m_ThreadParam.PoolQue.front();
		m_ThreadParam.PoolQue.pop();
	}
	m_ThreadParam.MtxPool.release();
	m_ThreadParam.MtxRead.acquire();
	while (!m_ThreadParam.ReadQue.empty())
	{
		CBinaryString*pBD = m_ThreadParam.ReadQue.front();
		m_ThreadParam.ReadQue.pop();
	}
	m_ThreadParam.MtxRead.release();

	m_bConnected = false;

	for (int i = m_StringPointerWarehouse.size(); 0 < i; --i)
	{
		CBinaryString* pBS;
		pBS = m_StringPointerWarehouse.front();
		delete pBS;
		m_StringPointerWarehouse.erase(m_StringPointerWarehouse.begin());
	}
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
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}
		pParam->pBD = pParam->PoolQue.front();
		pParam->PoolQue.pop();
		pParam->MtxPool.release();

		pParam->len = HRead(pParam->hPipeIn,*pParam->pBD);
		if (pParam->len != -1)
		{
			if (pParam->len != 0)
			{
				pParam->pBD->SetDataSize(pParam->len);
				pParam->MtxRead.acquire();
				pParam->ReadQue.push(pParam->pBD);
				pParam->pBD = nullptr;
				pParam->MtxRead.release();
			}
			else {
				pParam->bForcedTermination = true;
				pParam->MtxPool.acquire();
				pParam->PoolQue.push(pParam->pBD);
				pParam->pBD = nullptr;
				pParam->MtxPool.release();
				break;
			}
		}
		else {
			pParam->bForcedTermination = true;
			pParam->MtxPool.acquire();
			pParam->PoolQue.push(pParam->pBD);
			pParam->pBD = nullptr;
			pParam->MtxPool.release();
			break;
		}

//		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}

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
	CBinaryString* pStr(nullptr);
	m_ThreadParam.MtxRead.acquire();

	if (CBStream::Read(outBD))
	{
		m_ThreadParam.MtxRead.release();
		return -1;
	}

	if (m_ThreadParam.ReadQue.empty())
	{
		m_ThreadParam.MtxRead.release();
		return 0;
	}

	while (!m_ThreadParam.ReadQue.empty())
	{
		pStr = m_ThreadParam.ReadQue.front();

		outBD += *pStr;
		*pStr = "";
		m_ThreadParam.ReadQue.pop();

		m_ThreadParam.MtxPool.acquire();
		m_ThreadParam.PoolQue.push(pStr);
		pStr = nullptr;
		m_ThreadParam.MtxPool.release();
	}

	m_ThreadParam.MtxRead.release();
	return outBD.GetDataSize();
}

int CPipeTerm::Write(const CBinaryString& BD)
{
	if (CBStream::Write(BD))
	{
		return -1;
	}

	int res= PipeHelper::HWrite(m_PipeOut,BD);
	FlushFileBuffers(m_PipeOut);
	return res;
}

