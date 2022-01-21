#pragma once
#include <thread>

class CSecTimer
{
public:
	CSecTimer();
	~CSecTimer();
	void SetParam(bool* pTrigger, time_t sec);
	void Start();
	void Stop();
	void Pause();
	void Continue();
protected:
	struct stThreadParam
	{
		time_t Timer;
		bool* pbTrigger;
		unsigned int count;
		bool bPause;
		bool bStop;
		std::thread* pThisThread;
	};
	stThreadParam m_ThreadParam{};
	time_t m_Timer;
	bool* m_pbTrigger;
	void static CSecThread(stThreadParam* pParam);

};

