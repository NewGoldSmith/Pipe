#include <CAsyncBStreamQue.h>

BStreamQue::BStreamQue(unsigned int numCont, unsigned int sizeCnt)
	:m_pBStreamA(nullptr)
	, m_pBStreamB(nullptr)
	, m_hAtoBRead(nullptr)
	, m_hAtoBWrite(nullptr)
	, m_hBtoARead(nullptr)
	, m_hBtoAWrite(nullptr)
{
	SECURITY_ATTRIBUTES saAttr = {};

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = FALSE;
	saAttr.lpSecurityDescriptor = NULL;
	if (!CreatePipe(&m_hAtoBRead, &m_hAtoBWrite, &saAttr, 0))
	{
		HRESULT_FROM_WIN32(GetLastError());
	}
	if (!CreatePipe(&m_hBtoARead, &m_hBtoAWrite, &saAttr, 0))
	{
		HRESULT_FROM_WIN32(GetLastError());
	}
	m_pBStreamA = (CPipeTerm*)new CPipeTerm(16);
	m_pBStreamB = (CPipeTerm*)new CPipeTerm(16);
}

BStreamQue::~BStreamQue()
{
	m_JointStream.StopWork();
	m_pBStreamA->Disconnect();
	m_pBStreamB->Disconnect();
	delete m_pBStreamA;
	delete m_pBStreamB;
	CloseHandle(m_hAtoBRead);
	CloseHandle(m_hAtoBWrite);
	CloseHandle(m_hBtoARead);
	CloseHandle(m_hBtoAWrite);
}

CBStream* BStreamQue::GetStreamEndA()
{
	return m_pBStreamA;
}

CBStream* BStreamQue::GetStreamEndB()
{
	return m_pBStreamB;
}

void BStreamQue::StartWork()
{
	m_pBStreamA->SetHandle(m_hBtoARead, m_hAtoBWrite);
	m_pBStreamB->SetHandle(m_hAtoBRead, m_hBtoAWrite);
	m_JointStream.SetStream(m_pBStreamA, m_pBStreamB);
	m_JointStream.StartWork();
}

void BStreamQue::StopWork()
{
	m_JointStream.StopWork();
	m_pBStreamA->Disconnect();
	m_pBStreamB->Disconnect();
}
