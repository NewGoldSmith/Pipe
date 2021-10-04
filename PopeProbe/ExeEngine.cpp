#include "pch.h"
#include "ExeEngine.h"


int ExeEngine::Execute(CString strCommand, HANDLE hPipeIn, HANDLE hPipeOut, HANDLE hPipeErr, DWORD creationflags, CWnd* pWnd, DWORD dwWindowMessage)
{
	m_tParam.WM_MESSAGE = dwWindowMessage;
	CWinApp* pApp = AfxGetApp();
	SECURITY_ATTRIBUTES saAttr = {};
	BOOL bSuccess = FALSE;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	STARTUPINFO siStartInfo = {};
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = hPipeErr;
	siStartInfo.hStdOutput = hPipeOut;
	siStartInfo.hStdInput = hPipeIn;
	siStartInfo.wShowWindow = SW_HIDE;
	siStartInfo.dwFlags  |= creationflags;

	bSuccess = CreateProcess(NULL,
		strCommand.GetBuffer(),     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&m_PI);  // receives PROCESS_INFORMATION 
	strCommand.ReleaseBuffer();
	if (bSuccess == FALSE)
	{
		AfxMessageBox(_T("子プロセス起動に失敗しました。"), MB_ICONSTOP | MB_OK);
		ExitProcess(0);
	}
	// ChildProcessの起動状態監視用スレッド。
	m_tParam.pPI = &m_PI;
	m_tParam.hWnd = pWnd->m_hWnd;
	CWinThread* m_pThreadDetect = AfxBeginThread(ThreadProcDetectEnd, &m_tParam);
	m_tParam.hThisThread = m_pThreadDetect->m_hThread;

	return 0;
}


UINT ExeEngine::ThreadProcDetectEnd(LPVOID pvParam)
{
	TRACE("\r\nThreadProcDetectEnd-------------->START\r\n");
	THREAD_PARAM_DETECT* pTPD = (THREAD_PARAM_DETECT*)pvParam;
	DWORD rVal;
	PostMessage(pTPD->hWnd, pTPD->WM_MESSAGE, EXE_ENGINE_START, 0);
	rVal = WaitForSingleObject(pTPD->pPI->hProcess, INFINITE);
	PostMessage(pTPD->hWnd, pTPD->WM_MESSAGE, EXE_ENGINE_END, 0);
	CloseHandle(pTPD->pPI->hProcess);
	CloseHandle(pTPD->pPI->hThread);
	*pTPD->pPI = { 0 };
	TRACE("\r\nThreadProcDetectEnd--------------->END\r\n");
	AfxEndThread(456, TRUE);
	return 456;
}

ExeEngine::~ExeEngine()
{
	TRACE("\r\nExeEngine::~ExeEngine()\r\n");
	WaitForSingleObject(m_tParam.hThisThread,INFINITE);
}
//強制終了の為のルーチン
int ExeEngine::ForcedTermination()
{
	if (m_tParam.pPI->hProcess != NULL)
	{
		TerminateProcess(m_tParam.pPI->hProcess,3);
		CloseHandle(m_tParam.pPI->hProcess);
		CloseHandle(m_tParam.pPI->hThread);
	}
	return 0;
}
