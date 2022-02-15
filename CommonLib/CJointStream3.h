#pragma once
#include <CBStream.h>
#include <thread>
#include <semaphore>
class CJointStream3
{
public:
	typedef std::function<void(void) > type_pJointEventHandler;
	CJointStream3();
	~CJointStream3();
	CJointStream3(CJointStream3& OtheObj) = delete;
	void SetStream3(CBStream* pstream1, CBStream* pstream2,CBStream* pstream3);
	void StartWork3();
	void StopWork3();
	bool IsRunning();
	virtual void SetEventHandler(type_pJointEventHandler pEvent);
	virtual void ClearEventHandler();
protected:
	struct stThreadParam3 {
		CBStream* pStream1;
		CBStream* pStream2;
		CBStream* pStream3;
		CBinaryString bstr;
		bool* pEnd;
		std::thread* pThisThread;
		std::counting_semaphore<1> MtxEvent{ 1 };
		type_pJointEventHandler* ppEvOnDisconnect = nullptr;
	};
	bool m_bEnd;
	stThreadParam3 m_ThreadParam3;
	static void ThreadProc3(stThreadParam3* pThreadParam3);
	type_pJointEventHandler m_pEvOnDisconnect;

};

