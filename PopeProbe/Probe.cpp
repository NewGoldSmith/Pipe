#include "pch.h"
#include "Probe.h"
Probe::Probe()
{
	m_hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
}
Probe::~Probe()
{
	TRACE(_T("\r\nProbe::~Probe ID=%d\r\n"),m_TParam.ID_THREAD);
	WaitForSingleObject(m_TParam.hThread, INFINITE);
	CloseHandle(m_hEvent);
}
int Probe::Connect(HANDLE hPipeIn, HANDLE hPipeOut, char * pBuf, size_t Bufsize, DWORD dwID, DWORD dwMessage, CWnd* pWnd)
{
	m_TParam = {};
	m_TParam.pWnd=pWnd;
	m_TParam.hInput=hPipeIn;
	m_TParam.hOutput=hPipeOut;
	m_TParam.hEvent=m_hEvent;
	m_TParam.WM_MESSAGE=dwMessage;
	m_TParam.ID_THREAD=dwID;
	m_TParam.chBuf=pBuf;
	m_TParam.Bufsize=Bufsize;
	CWinThread* pThread = AfxBeginThread(ThreadProcConnecting, &m_TParam);
	m_TParam.hThread = pThread->m_hThread;
	return 0;
}


UINT Probe::ThreadProcConnecting(LPVOID lpParam)
{
	THREAD_PARAM_PIPE* pTP = (THREAD_PARAM_PIPE*)lpParam;
	TRACE("\r\nThreadProcConnecting---------->STRAT ID=%d\r\n", pTP->ID_THREAD);
	pTP->bContinue = TRUE;
	while (pTP->bContinue)
	{
		ZeroMemory(pTP->chBuf, pTP->Bufsize);
		pTP->bSuccess = ReadFile(pTP->hInput, pTP->chBuf, pTP->Bufsize, &(pTP->dwRead), NULL);
		if (!pTP->bSuccess || pTP->dwRead == 0)
		{
			DWORD dwVal;
			dwVal = GetLastError();
			pTP->pWnd->PostMessage(pTP->WM_MESSAGE, dwVal, 0);
			break;
		}
		TRACE("\r\nThreadProcConnecting---- ID=%d Received Message:\r\n%s\r\n", pTP->ID_THREAD, pTP->chBuf);
		ResetEvent(pTP->hEvent);
		pTP->pWnd->PostMessage(pTP->WM_MESSAGE, 0, 0);
		WaitForSingleObject(pTP->hEvent, INFINITE);
		if (pTP->bContinue == FALSE)
		{
			break;
		}

		pTP->bSuccess = WriteFile(pTP->hOutput, pTP->chBuf, pTP->dwRead, &(pTP->dwWritten), NULL);
		if (!pTP->bSuccess /* || (dwRead != dwWritten) */)
		{
			DWORD dwVal;
			dwVal = GetLastError();
			pTP->pWnd->PostMessage(pTP->WM_MESSAGE, dwVal, 0);
			break;
		}
	}

	pTP->pWnd->PostMessage(pTP->WM_MESSAGE, 0, -1);
	TRACE("\r\nThreadProcConnecting----------------------->END ID=%d\r\n", pTP->ID_THREAD);
	AfxEndThread(123, TRUE);
	return 123;
}


CString Probe::GetString()
{
	CString str(m_TParam.chBuf);
	SetEvent(m_hEvent);
	return str;
}


void Probe::DisConnect()
{
	m_TParam.bContinue = FALSE;
	CancelIoEx(m_TParam.hInput,NULL);
	CancelIoEx(m_TParam.hOutput,NULL);
}

