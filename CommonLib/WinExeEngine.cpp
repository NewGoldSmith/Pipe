#include "WinExeEngine.h"

void  WinExeEngine::ThreadProcDetectEnd(void* pvParam)
{
	stTHREAD_PARAM_DETECT_END* pThreadDetectEndParam = (stTHREAD_PARAM_DETECT_END*)pvParam;
	pThreadDetectEndParam->rVal = WaitForSingleObject(pThreadDetectEndParam->pPI->hProcess, INFINITE);
	pThreadDetectEndParam->Alive = false;
	if (pThreadDetectEndParam->pEvOnEnd != nullptr)
	{
		pThreadDetectEndParam->pEvOnEnd();
	}
	return;
}


WinExeEngine::WinExeEngine():
	m_ThreadProcDetectEndParam{}
	, m_PI{}
{
}

WinExeEngine::~WinExeEngine()
{
//	ForcedTermination();
}

bool WinExeEngine::Execute(std::wstring strCommand, HANDLE hPipeIn, HANDLE hPipeOut, HANDLE hPipeErr, DWORD creationflags)
{
	if (m_ThreadProcDetectEndParam.pThisThread != nullptr)
	{
		//前回起動したプロセスは終了させる
		ForcedTermination();
	}
	HANDLE h = GetModuleHandle(0);
	SECURITY_ATTRIBUTES saAttr = {};
	BOOL bSuccess = FALSE;
	m_ThreadProcDetectEndParam.Alive = true;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	STARTUPINFO siStartInfo = {};
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = hPipeErr;
	siStartInfo.hStdOutput = hPipeOut;
	siStartInfo.hStdInput = hPipeIn;
	siStartInfo.wShowWindow = SW_NORMAL;
	siStartInfo.dwFlags |= creationflags;

	bSuccess = CreateProcessW(NULL,
		(LPWSTR)strCommand.c_str(),     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&m_PI);  // receives PROCESS_INFORMATION 
	if (bSuccess == FALSE)
	{
		return false;
	}

		m_ThreadProcDetectEndParam.pPI = &m_PI;
		m_ThreadProcDetectEndParam.pThisThread = new std::thread(&ThreadProcDetectEnd, &m_ThreadProcDetectEndParam);

	return true;
}

bool WinExeEngine::ForcedTermination(unsigned int iExitCode)
{
	////コンソール入力待ちのスレッドを終了。
//	m_ThreadProcDetectEndParam.bForcedTermination = true;
	HANDLE hThread = m_ThreadProcDetectEndParam.pThisThread->native_handle();
	int rVal = CancelSynchronousIo(hThread);
//	int rVal = 0;
	if (m_ThreadProcDetectEndParam.Alive )
	{
		HANDLE hp = m_ThreadProcDetectEndParam.pPI->hProcess;
		rVal = TerminateProcess(m_ThreadProcDetectEndParam.pPI->hProcess, iExitCode);
		m_ThreadProcDetectEndParam.pThisThread->join();
		CloseHandle(m_ThreadProcDetectEndParam.pPI->hProcess);
		CloseHandle(m_ThreadProcDetectEndParam.pPI->hThread);
		delete m_ThreadProcDetectEndParam.pThisThread;
		m_ThreadProcDetectEndParam.pThisThread = nullptr;
		m_ThreadProcDetectEndParam.Alive = false;
		return true;
	}
	return true;
}


bool WinExeEngine::IsRun()
{
	return m_ThreadProcDetectEndParam.Alive;
}

void WinExeEngine::SetEventHandler(type_pExeEventHandler pEvent)
{
	m_ThreadProcDetectEndParam.pEvOnEnd=pEvent;
}
void WinExeEngine::ClearEventHandler()
{
	m_ThreadProcDetectEndParam.pEvOnEnd = nullptr;
}
