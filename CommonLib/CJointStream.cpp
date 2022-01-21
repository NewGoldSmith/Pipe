#include <CJointStream.h>

CJointStream::CJointStream() :
	m_ThreadParam{}
	,m_bEnd{false}
{
	m_ThreadParam.pEnd = &m_bEnd;
}
CJointStream::~CJointStream()
{
	if (m_ThreadParam.pThisThread != nullptr)
	{
		StopWork();
	}
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
	m_bEnd = true;
	m_ThreadParam.pThisThread->join();
	delete m_ThreadParam.pThisThread;
	m_ThreadParam.pThisThread = nullptr;
}

void CJointStream::ThreadProc(stThreadParam* pThreadParam)
{
	for (;!*pThreadParam->pEnd;)
	{
		pThreadParam->pStream1->Read(pThreadParam->bstr);
		pThreadParam->pStream2->Write(pThreadParam->bstr);
		pThreadParam->pStream2->Read(pThreadParam->bstr);
		pThreadParam->pStream1->Write(pThreadParam->bstr);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
