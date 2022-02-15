//Copyright (c) 2021, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php
#include "CJointIntercept.h"

CJointIntercept::stThreadParam::stThreadParam()
	: pStCon(nullptr)
	, pStIntercept(nullptr)
	, pStPsd(nullptr)
	, bstr(8192)
	, pEnd(nullptr)
	, pThisThread(nullptr)
	, MtxEvent{ 1 }
	, ppEvOnDisconnect(nullptr)
	, pIsInterceptMode(nullptr)
	, pIsInterceptWriteOnlyMode(nullptr)
{ }

CJointIntercept::CJointIntercept()
	:m_bEnd(true)
	, m_ThreadParam()
	, m_pEvOnDisconnect(nullptr)
	, m_bIsInterceptMode(true)
	, m_bIsInterceptWriteOnlyMode(true)
{
	m_ThreadParam.pEnd = &m_bEnd;
	m_ThreadParam.ppEvOnDisconnect = &(this->m_pEvOnDisconnect);
	m_ThreadParam.pIsInterceptMode = &m_bIsInterceptMode;
	m_ThreadParam.pIsInterceptWriteOnlyMode = &m_bIsInterceptWriteOnlyMode;
}

CJointIntercept::~CJointIntercept()
{
	if (m_ThreadParam.pThisThread!=nullptr)
	{
		StopWork();
	}
}

void CJointIntercept::SetStream(CBStream* pStCon, CBStream* pStIntercept, CBStream* pStPsd)
{
	m_ThreadParam.pStCon = pStCon;
	m_ThreadParam.pStIntercept = pStIntercept;
	m_ThreadParam.pStPsd = pStPsd;
}

void CJointIntercept::SetInterceptMode(bool onoff)
{
	m_bIsInterceptMode = onoff;
}

void CJointIntercept::SetInterceptWriteOnlyMode(bool onoff)
{
}

void CJointIntercept::StartWork()
{
	if (m_ThreadParam.pThisThread!=nullptr)
	{
		StopWork();
	}
	m_bEnd = false;
	m_ThreadParam.pThisThread = new std::thread(&ThreadProc, &m_ThreadParam);
}

void CJointIntercept::StopWork()
{
	if (m_ThreadParam.pThisThread==nullptr)
	{
		return;
	}
	m_bEnd = true;
	m_ThreadParam.pThisThread->join();
	delete m_ThreadParam.pThisThread;
	m_ThreadParam.pThisThread = nullptr;
}

bool CJointIntercept::IsRunning()const
{
	return !m_bEnd;
}

void CJointIntercept::SetEventHandler(const type_pJointEventHandler pEvent)
{
	m_ThreadParam.MtxEvent.acquire();
	m_pEvOnDisconnect = pEvent;
	m_ThreadParam.MtxEvent.release();
}

void CJointIntercept::ClearEventHandler()
{
	m_ThreadParam.MtxEvent.acquire();
	m_pEvOnDisconnect = nullptr;
	m_ThreadParam.MtxEvent.release();
}

void CJointIntercept::ThreadProc(stThreadParam* pThreadParam)
{
	for (pThreadParam->MtxEvent.acquire(); !*pThreadParam->pEnd; pThreadParam->MtxEvent.acquire())
	{

		if (!pThreadParam->pStCon->IsConnect() ||
			!pThreadParam->pStIntercept->IsConnect() ||
			!pThreadParam->pStPsd->IsConnect())
		{
			*pThreadParam->pEnd = true;
			if (*(pThreadParam->ppEvOnDisconnect) != nullptr)
			{
				(*(pThreadParam->ppEvOnDisconnect))();
			}
			break;
		}
		pThreadParam->pStCon->Read(pThreadParam->bstr);
		pThreadParam->pStPsd->Write(pThreadParam->bstr);

		if (*pThreadParam->pIsInterceptMode)
		{
			pThreadParam->pStIntercept->Read(pThreadParam->bstr);
			pThreadParam->pStPsd->Write(pThreadParam->bstr);
		}

		pThreadParam->pStPsd->Read(pThreadParam->bstr);
		pThreadParam->pStCon->Write(pThreadParam->bstr);
		if (*pThreadParam->pIsInterceptMode && !*pThreadParam->pIsInterceptWriteOnlyMode)
		{
			pThreadParam->pStIntercept->Write(pThreadParam->bstr);
		}

		pThreadParam->MtxEvent.release();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	pThreadParam->MtxEvent.release();
}

