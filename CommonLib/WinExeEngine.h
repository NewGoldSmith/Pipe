#pragma once
#include <string>
#include <functional>
#include <WinSock2.h>
#include <semaphore>
#include <thread>

class WinExeEngine
{
private:
	WinExeEngine& operator = (WinExeEngine &obj) =delete;
	WinExeEngine(WinExeEngine& other) =delete;
public:
	typedef std::function<void(void) > type_pExeEventHandler;
	WinExeEngine();
	~WinExeEngine();
	bool Execute(std::wstring strCommand, HANDLE hPipeIn, HANDLE hPipeOut, HANDLE hPipeErr, DWORD creationflags);
	bool ForcedTermination(unsigned int iExitCode = 1);
	void SetEventHandler(type_pExeEventHandler pEvent);
	void ClearEventHandler();
protected:
	PROCESS_INFORMATION m_PI;
	static void ThreadProcDetectEnd(void * pvParam);
	struct stTHREAD_PARAM_DETECT_END
	{
		bool Alive;
		DWORD rVal;
		PROCESS_INFORMATION* pPI;
		type_pExeEventHandler pEvOnEnd;
		std::thread* pThisThread;
	};
	stTHREAD_PARAM_DETECT_END m_ThreadProcDetectEndParam;
public:
	bool IsRun();
};

