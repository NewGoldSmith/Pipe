#pragma once
#include <CBStream.h>
#include <thread>
class CJointStream
{
public:
	CJointStream();
	~CJointStream();
	CJointStream(CJointStream& OtheObj) = delete;
	void SetStream(CBStream* pstream1, CBStream* pstream2);
	void StartWork();
	void StopWork();
protected:
	struct stThreadParam {
		CBStream* pStream1;
		CBStream* pStream2;
		CBinaryString bstr;
		bool* pEnd;
		std::thread* pThisThread;
	};
	bool m_bEnd;
	stThreadParam m_ThreadParam;
	static void ThreadProc(stThreadParam* pThreadParam);
};

