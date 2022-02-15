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
		stThreadParam() :
			Timer(0)
			, pbTrigger(nullptr)
			, count(0)
			, bPause(0)
			, bStop(0)
			,pThisThread(nullptr) {};
		time_t Timer;
		bool* pbTrigger;
		unsigned int count;
		bool bPause;
		bool bStop;
		std::thread* pThisThread;
	};
	stThreadParam m_ThreadParam;
	time_t m_Timer;
	bool* m_pbTrigger;
	void static CSecThread(stThreadParam* pParam);

};

