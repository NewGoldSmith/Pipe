#pragma once
class Probe
{
	
public:
	Probe();
	~Probe();
	int Connect(HANDLE hPipeIn, HANDLE hPipeOut, char *pBuf, size_t Bufsize,/* CSemaphore* pSem, HANDLE hEvent,*/ DWORD dwID, DWORD dwMessage, /*LPARAM lpEndFlag, */ CWnd * pWnd);
protected:
	struct THREAD_PARAM_PIPE
	{
		CWnd* pWnd;
		HANDLE hInput;
		HANDLE hOutput;
		HANDLE hEvent;
		HANDLE hThread;
		DWORD WM_MESSAGE;
		DWORD ID_THREAD;
		char* chBuf;
		size_t Bufsize;
		DWORD dwRead, dwWritten;
		BOOL bContinue;
		BOOL bSuccess;
	};
	THREAD_PARAM_PIPE m_TParam;
	static UINT ThreadProcConnecting(LPVOID pParam);
	HANDLE m_hEvent;
public:
	CString GetString();
	void DisConnect();
//	void Continue();
};

