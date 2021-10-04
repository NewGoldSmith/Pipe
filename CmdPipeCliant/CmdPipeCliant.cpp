#pragma once
#include <afxwin.h>
#include <stdio.h>
#include <tchar.h>
#include <AFXPRIV.H>
#include <strsafe.h>
#include "CCommandOption.h"

#define BUFSIZE 4096 

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	TRACE("\r\nCmdPipeCliant.exe----------------------------------------STRAT\r\n");
	USES_CONVERSION;
	char chBuf[BUFSIZE] = { 0 };
	HANDLE hStdin, hStdout;
	BOOL bSuccess = TRUE;
    BOOL bContinue=TRUE;
	DWORD dwRead, dwWritten;
	CCommandOption CCmdOp;
	setvbuf(stdout, NULL, _IONBF, 0);
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (
		(hStdout == INVALID_HANDLE_VALUE) ||
		(hStdin == INVALID_HANDLE_VALUE)
		)
		ExitProcess(1);
	CWinApp* pApp = AfxGetApp();
	pApp->ParseCommandLine(CCmdOp);
	if (CCmdOp.m_bEchoCommand == TRUE)
	{
		CString str = _T("CmdPipeCliant.exe");
		str += _T(" ");
		str += lpCmdLine;
		str += _T("\r\n");
		bSuccess = WriteFile(hStdout, str, str.GetLength(), &dwWritten, NULL);
	}
    // Send something to this process's stdout using printf.

    // This simple algorithm uses the existence of the pipes to control execution.
    // It relies on the pipe buffers to ensure that no data is lost.
    // Larger applications would use more advanced process control.
	DWORD dwVal;
	int i = 0;
	while (bContinue)
	{
		ZeroMemory(chBuf, BUFSIZE);
		bSuccess = ReadFile(hStdin, chBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0)
		{
			bContinue = FALSE;
			dwVal=GetLastError();
			switch (dwVal)
			{
			case ERROR_HANDLE_EOF:
				i = dwVal;
				TRACE("\r\nCmdPipeCliant.exe--------ERROR_HANDLE_EOF---------------ERROR\r\n");
				break;
			case ERROR_BROKEN_PIPE:
				i = dwVal;
				TRACE("\r\nCmdPipeCliant.exe--------ERROR_BROKEN_PIPE--------------ERROR\r\n");
				break;
			default:
				break;
			}
			break;
		}
		int rVal = 0;
		rVal = _stricmp(chBuf, "quit\r\n");
		if (!rVal)
		{
			TRACE("\r\nCmdPipeCliant.exe--------QUIT HAS BEEN ENTERED--------------ERROR\r\n");

			bContinue = FALSE;
		}

        bSuccess = WriteFile(hStdout, chBuf, dwRead, &dwWritten, NULL);
        if (!bSuccess /* || (dwRead != dwWritten) */ )
        {
            bContinue = FALSE;
			dwVal = GetLastError();
			switch (dwVal)
			{
			case ERROR_HANDLE_EOF:
				i = dwVal;
				break;
			case ERROR_BROKEN_PIPE:
				i = dwVal;
				break;
			default:
				break;
			}
            break;
        }

	}
	TRACE("\r\nCmdPipeCliant.exe----------------------------------------END\r\n");
    return 0;
}

