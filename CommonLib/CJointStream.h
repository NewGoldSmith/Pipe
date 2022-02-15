#pragma once
#include <semaphore>
#include <CBStream.h>
#include <thread>
class CJointStream
{
public:
	typedef std::function<void(void) > type_pJointEventHandler;
	CJointStream();
	~CJointStream();
	CJointStream(CJointStream& OtheObj) = delete;
	void SetStream(CBStream* pstream1, CBStream* pstream2);
	void StartWork();
	void StopWork();
	virtual void SetEventHandler(type_pJointEventHandler pEvent);
	virtual void ClearEventHandler();
	virtual bool IsConnect();
protected:
	struct stThreadParam {
		stThreadParam();
		CBStream* pStream1;
		CBStream* pStream2;
		CBinaryString bstr;
		bool* pEnd;
		std::counting_semaphore<1> MtxEvent;
		type_pJointEventHandler *ppEvOnDisconnect;
		std::thread* pThisThread;
	};
	bool m_bEnd;
	stThreadParam m_ThreadParam;
	static void ThreadProc(stThreadParam* pThreadParam);
	type_pJointEventHandler m_pEvOnDisconnect;
};

