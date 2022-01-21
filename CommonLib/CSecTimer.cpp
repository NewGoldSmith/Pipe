#include "CSecTimer.h"



CSecTimer::CSecTimer()
{
}

CSecTimer::~CSecTimer()
{
	Stop();
}

void CSecTimer::SetParam(bool* pTrigger, time_t sec)
{
	m_pbTrigger = pTrigger;
	m_Timer = sec;
}

void CSecTimer::Start()
{
	if (m_ThreadParam.pThisThread != nullptr)
	{
		Stop();
	}
	m_ThreadParam={};
	m_ThreadParam.pbTrigger = m_pbTrigger;
	*m_ThreadParam.pbTrigger = false;
	m_ThreadParam.Timer = m_Timer;

	m_ThreadParam.pThisThread = new std::thread(&CSecThread, &m_ThreadParam);
}

void CSecTimer::Stop()
{
	if (m_ThreadParam.pThisThread != nullptr)
	{
		m_ThreadParam.bStop = true;
		m_ThreadParam.pThisThread->join();
		delete m_ThreadParam.pThisThread;
		m_ThreadParam.pThisThread = nullptr;
	}
}

void CSecTimer::Pause()
{
	m_ThreadParam.bPause = true;
}

void CSecTimer::Continue()
{
	m_ThreadParam.bPause = false;
}

void CSecTimer::CSecThread(stThreadParam* pParam)
{
	*(pParam->pbTrigger) = false;
	for (pParam->count = 0; pParam->count < pParam->Timer; pParam->count++)
	{
		if (pParam->bStop)
		{
			return;
		}
		if (pParam->bPause)
		{
			pParam->count--;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	*pParam->pbTrigger = true;
}
