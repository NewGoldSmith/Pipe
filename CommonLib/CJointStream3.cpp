#include <CJointStream3.h>

CJointStream3::CJointStream3():
	m_ThreadParam3{}
	, m_bEnd{ false }
{
	m_ThreadParam3.pEnd = &m_bEnd;
	m_ThreadParam3.ppEvOnDisconnect = &(this->m_pEvOnDisconnect);
}

CJointStream3::~CJointStream3()
{
	if (m_ThreadParam3.pThisThread != nullptr)
	{
		StopWork3();
	}
}

void CJointStream3::SetStream3(CBStream* pstream1, CBStream* pstream2,CBStream* pstream3)
{
	m_ThreadParam3.pStream1 = pstream1;
	m_ThreadParam3.pStream2 = pstream2;
	m_ThreadParam3.pStream3 = pstream3;
}

void CJointStream3::StartWork3()
{
	if (m_ThreadParam3.pThisThread != nullptr)
	{
		StopWork3();
	}
	m_bEnd = false;
	m_ThreadParam3.pThisThread = new std::thread(&ThreadProc3, &m_ThreadParam3);
}

void CJointStream3::StopWork3()
{
	m_bEnd = true;
	m_ThreadParam3.pThisThread->join();
	delete m_ThreadParam3.pThisThread;
	m_ThreadParam3.pThisThread = nullptr;
}

bool CJointStream3::IsRunning()
{
	return !m_bEnd;
}

void CJointStream3::SetEventHandler(type_pJointEventHandler pEvent)
{
	m_ThreadParam3.MtxEvent.acquire();
	m_pEvOnDisconnect = pEvent;
	m_ThreadParam3.MtxEvent.release();
}

void CJointStream3::ClearEventHandler()
{
	m_ThreadParam3.MtxEvent.acquire();
	m_pEvOnDisconnect = nullptr;
	m_ThreadParam3.MtxEvent.release();
}

void CJointStream3::ThreadProc3(stThreadParam3* pThreadParam3)
{

	for (; !*pThreadParam3->pEnd;)
	{
		if (!pThreadParam3->pStream1->IsConnect() ||
			!pThreadParam3->pStream2->IsConnect() ||
			!pThreadParam3->pStream3->IsConnect())
		{
			*pThreadParam3->pEnd = true;
			pThreadParam3->MtxEvent.acquire();
			if (*(pThreadParam3->ppEvOnDisconnect) != nullptr)
			{
				(*(pThreadParam3->ppEvOnDisconnect))();
			}
			pThreadParam3->MtxEvent.release();
			break;
		}
		pThreadParam3->pStream1->Read(pThreadParam3->bstr);
		pThreadParam3->pStream2->Write(pThreadParam3->bstr);
		pThreadParam3->pStream3->Write(pThreadParam3->bstr);

		pThreadParam3->pStream2->Read(pThreadParam3->bstr);
		pThreadParam3->pStream1->Write(pThreadParam3->bstr);
		pThreadParam3->pStream3->Write(pThreadParam3->bstr);

		pThreadParam3->pStream3->Read(pThreadParam3->bstr);
		pThreadParam3->pStream1->Write(pThreadParam3->bstr);
		pThreadParam3->pStream2->Write(pThreadParam3->bstr);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
