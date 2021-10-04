#pragma once
#define EXE_ENGINE_START 0
#define EXE_ENGINE_END 1

class ExeEngine
{
public:
	ExeEngine() { m_PI = {}; };
	~ExeEngine();
	int Execute(CString strCommand, HANDLE hPipeIn, HANDLE hPipeOut, HANDLE hPipeErr, DWORD creationflags, CWnd* pWnd, DWORD dwWindowMessage);
protected:
	PROCESS_INFORMATION m_PI;
	static UINT ThreadProcDetectEnd(LPVOID pvParam);
	struct THREAD_PARAM_DETECT
	{
		PROCESS_INFORMATION* pPI;
		HANDLE hThisThread;
		HWND hWnd;
		DWORD WM_MESSAGE;
	};
	THREAD_PARAM_DETECT m_tParam;

	DWORD m_dwWindowMessage;
public:
	int ForcedTermination();
};

