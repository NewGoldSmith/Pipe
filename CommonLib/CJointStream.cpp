#include <CJointStream.h>

CJointStream::stThreadParam::stThreadParam():
	pStream1(nullptr)
	,pStream2(nullptr)
	,bstr(1024)
	,pEnd(nullptr)
	,MtxEvent{1}
	, ppEvOnDisconnect(nullptr)
	,pThisThread(nullptr)
{
}

CJointStream::CJointStream() :
	m_ThreadParam()
	,m_bEnd{false}
{
	m_ThreadParam.pEnd = &m_bEnd;
	m_ThreadParam.ppEvOnDisconnect = &(this->m_pEvOnDisconnect);
}
CJointStream::~CJointStream()
{
	StopWork();
}

void CJointStream::SetStream(CBStream* pstream1, CBStream* pstream2)
{
	m_ThreadParam.pStream1 = pstream1;
	m_ThreadParam.pStream2 = pstream2;
}

void CJointStream::StartWork()
{
	if (m_ThreadParam.pThisThread != nullptr)
	{
		StopWork();
	}
	m_bEnd = false;
	m_ThreadParam.pThisThread = new std::thread(&ThreadProc, &m_ThreadParam);
}

void CJointStream::StopWork()
{
	if (m_ThreadParam.pThisThread == nullptr)
		return;
	m_bEnd = true;
	m_ThreadParam.pThisThread->join();
	delete m_ThreadParam.pThisThread;
	m_ThreadParam.pThisThread = nullptr;
}

void CJointStream::SetEventHandler(type_pJointEventHandler pEvent)
{
	m_ThreadParam.MtxEvent.acquire();
	m_pEvOnDisconnect = pEvent;
	m_ThreadParam.MtxEvent.release();
}

void CJointStream::ClearEventHandler()
{
	m_ThreadParam.MtxEvent.acquire();
	m_pEvOnDisconnect = nullptr;
	m_ThreadParam.MtxEvent.release();
}

bool CJointStream::IsConnect()
{
	return !*m_ThreadParam.pEnd;
}

void CJointStream::ThreadProc(stThreadParam* pThreadParam)
{
	for (;!*pThreadParam->pEnd;)
	{
		if (!pThreadParam->pStream1->IsConnect() || !pThreadParam->pStream2->IsConnect())
		{
			*pThreadParam->pEnd = true;
			break;
		}
		pThreadParam->pStream1->Read(pThreadParam->bstr);
		pThreadParam->pStream2->Write(pThreadParam->bstr);
		pThreadParam->pStream2->Read(pThreadParam->bstr);
		pThreadParam->pStream1->Write(pThreadParam->bstr);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	pThreadParam->MtxEvent.acquire();
	if (*(pThreadParam->ppEvOnDisconnect) != nullptr)
	{
		(*(pThreadParam->ppEvOnDisconnect))();
	}
	pThreadParam->MtxEvent.release();
}
