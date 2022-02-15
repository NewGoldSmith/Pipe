//Copyright (c) 2021, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php

#include "CPConsole.h"

CPConsole::stParamDetectEnd::stParamDetectEnd() :
	pThisThread(nullptr)
	, pPI(nullptr)
	, rVal(0)
	, pbConnected(nullptr)
	, ppEvOnDisconnect(nullptr)
	, pEvOnInternalDetectEnd(nullptr)
{
}

CPConsole::stParamForwarder::stParamForwarder()
	:pStdSide(nullptr)
	, pPSide(nullptr)
	, pInterStdSide(nullptr)
	, pInterPSide(nullptr)
	, pbConnected(nullptr)
	, bstr()
	, m_emPInterceptMode(emInterceptMode::Off)
	, m_emStdInterceptMode(emInterceptMode::Off)
	, m_bEnd(true)
	, Mtx{ 1 }
	, pThisThread(nullptr)
{
}

CPConsole::CPConsole()
	:CBStream()
	, m_PipeTermPSide(30, 2028)
	, m_PipeTermStdSide(8, 2028)
	, m_hPipePTYDownRead(nullptr)
	, m_hPipePTYDownWrite(nullptr)
	, m_hPipePTYUpRead(nullptr)
	, m_hPipePTYUpWrite(nullptr)
	, m_hStdIn(nullptr)
	, m_hStdOut(nullptr)
	, m_hPC(nullptr)
	, m_ConsoleModeInOld(0)
	, m_ConsoleModeOutOld(0)
	, m_ConsoleSize{ 640,480 }
	, m_si{}
	, m_pi{}
	, m_bForcedTermination(false)
	, m_ParamDetectEnd()
	, m_ParamForward()
{
	m_ParamDetectEnd.ppEvOnDisconnect = &(CPConsole::m_pEvOnDisconnect);
	m_ParamDetectEnd.pEvOnInternalDetectEnd = std::bind(&CPConsole::OnEvInternalDetectEnd, this);
	m_ParamDetectEnd.pbConnected = &m_bConnected;
	m_ParamForward.pbConnected = &m_bConnected;

	m_ParamForward.pStdSide = &m_PipeTermStdSide;
	m_ParamForward.pPSide = &m_PipeTermPSide;
	m_ParamForward.pInterStdSide = &m_TermThStdSide;
	m_ParamForward.pInterPSide = &m_TermThPSide;


	m_TermDlgStdSide.SetStream(&m_Que2, &m_Que1);
	m_TermDlgStdSide.Connect();

	m_TermThStdSide.SetStream(&m_Que1, &m_Que2);
	m_TermThStdSide.Connect();

	m_TermDlgPSide.SetStream(&m_Que3, &m_Que4);
	m_TermDlgPSide.Connect();

	m_TermThPSide.SetStream(&m_Que4, &m_Que3);
	m_TermThPSide.Connect();

	SetAccessMode( emAccessMode::No);
	SetStdAccessMode(emAccessMode::No);

	SetInterceptMode(emInterceptMode::Off);
}

CPConsole::~CPConsole()
{
	CloseConsole();
}

void CPConsole::CloseConsole()
{
	if (m_ParamDetectEnd.pThisThread == nullptr)
	{
		return;
	}
	m_TermDlgStdSide.Read(m_strLastMessageStd);
	m_TermDlgPSide.Read(m_strLastMessageP);
	m_ParamForward.m_bEnd = true;
	m_PipeTermPSide.Disconnect();
	m_PipeTermStdSide.Disconnect();
	m_ParamForward.pThisThread->join();
	delete m_ParamForward.pThisThread;
	m_ParamForward.pThisThread = nullptr;

	ClosePseudoConsole(m_hPC);
	m_hPC = nullptr;

	m_ParamDetectEnd.pThisThread->join();
	delete m_ParamDetectEnd.pThisThread;
	m_ParamDetectEnd.pThisThread = nullptr;
	CloseHandle(m_ParamDetectEnd.pPI->hThread);
	m_ParamDetectEnd.pPI->hThread = nullptr;
	CloseHandle(m_ParamDetectEnd.pPI->hProcess);
	m_ParamDetectEnd.pPI->hProcess = nullptr;

	DeleteProcThreadAttributeList(m_si.lpAttributeList);
	free(m_si.lpAttributeList);
	m_si.lpAttributeList = nullptr;
	CloseHandle(m_hPipePTYDownWrite);
	m_hPipePTYDownWrite = nullptr;
	CloseHandle(m_hPipePTYUpRead);
	m_hPipePTYUpRead = nullptr;
	m_bConnected = false;
}

CPConsole::emAccessMode CPConsole::SetAccessMode(const emAccessMode emaccess)
{
	emAccessMode tmp = CBStream::SetAccessMode(emaccess);
	m_TermDlgPSide.SetAccessMode(m_emAccessMode);
	return tmp;
}

CPConsole::emAccessMode CPConsole::SetStdAccessMode(const emAccessMode emaccess)
{
	emAccessMode tmpOld = m_TermDlgStdSide.SetAccessMode(emaccess);
	return tmpOld;
}

CPConsole::emInterceptMode CPConsole::SetInterceptMode(const emInterceptMode emIntcpt)
{
	emInterceptMode tmpOld = m_ParamForward.m_emPInterceptMode;
	m_ParamForward.m_emPInterceptMode = emIntcpt;
	return tmpOld;
}

CPConsole::emInterceptMode CPConsole::GetInterceptMode()
{
	return m_ParamForward.m_emPInterceptMode;
}


int CPConsole::Read(CBinaryString& BD)
{
	if (CBStream::Read(BD))
	{
		return -1;
	}
	if (IsConnect())
	{
		return m_TermDlgPSide.Read(BD);
	}
	else
	{
		BD = m_strLastMessageP;
		return BD.GetDataSize();
	}
}

int CPConsole::Write(const CBinaryString& BD)
{
	if (CBStream::Write(BD))
	{
		return -1;
	}
	return m_TermDlgPSide.Write(BD);
}

int CPConsole::StdRead(CBinaryString& bstr)
{
	if (IsConnect())
	{
		return m_TermDlgStdSide.Read(bstr);
	}
	else
	{
		bstr = m_strLastMessageStd;
		return bstr.GetDataSize();
	}
}

int CPConsole::StdWrite(const CBinaryString& bstr)
{
	return m_TermDlgStdSide.Write(bstr);
}

bool CPConsole::Connect()
{
	HRESULT rVal= OpenConsole(m_strCommand);
	return (rVal == S_OK)? true:false;
}

bool CPConsole::Disconnect()
{
	CloseConsole();
	return true;
}

bool CPConsole::IsConnect()
{
	return m_PipeTermPSide.IsConnect() && m_PipeTermStdSide.IsConnect() && m_ParamDetectEnd.pPI->hThread;
}

int CPConsole::Discard()
{
	m_Que1.Discard();
	m_Que2.Discard();
	m_Que3.Discard();
	m_Que4.Discard();
	return 0;
}

void CPConsole::SetEventHandler(type_pStreamEventHandler pEvent)
{
	CBStream::SetEventHandler(pEvent);
}

void CPConsole::SetCommand(const CBinaryString& bstr)
{
	m_strCommand = bstr;
}

void CPConsole::ClearEventHandler()
{
	CBStream::ClearEventHandler();
}

HRESULT CPConsole::OpenConsole(const CBinaryString& strCommand)
{
	//This string requires rewritable attributes.
	CBinaryString tempArg = strCommand;

	HRESULT hr= E_UNEXPECTED;
	m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	m_hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	// Enable Console VT Processing

	if (!GetConsoleMode(m_hStdOut, &m_ConsoleModeOutOld))
		return  HRESULT_FROM_WIN32(GetLastError());

	DWORD ConModeOut =
		0
		|ENABLE_PROCESSED_OUTPUT           
		|ENABLE_WRAP_AT_EOL_OUTPUT         
		|ENABLE_VIRTUAL_TERMINAL_PROCESSING
//		|DISABLE_NEWLINE_AUTO_RETURN       
//		|ENABLE_LVB_GRID_WORLDWIDE         
		;

	if (!SetConsoleMode(m_hStdOut, ConModeOut))
		return HRESULT_FROM_WIN32(GetLastError());

	if (!GetConsoleMode(m_hStdIn, &m_ConsoleModeInOld))
		return HRESULT_FROM_WIN32(GetLastError());

	DWORD ConModeIn =
		0
//		|ENABLE_WINDOW_INPUT
//		|ENABLE_MOUSE_INPUT
//		|ENABLE_PROCESSED_INPUT
//		|ENABLE_LINE_INPUT
//		|ENABLE_ECHO_INPUT
//		|ENABLE_INSERT_MODE
//		|ENABLE_QUICK_EDIT_MODE
//		|ENABLE_EXTENDED_FLAGS
//		|ENABLE_AUTO_POSITION
		|ENABLE_VIRTUAL_TERMINAL_INPUT
		;

	if (!SetConsoleMode(m_hStdIn, ConModeIn))
		return HRESULT_FROM_WIN32(GetLastError());

	if (!SetConsoleOutputCP(65001))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	m_hPC= INVALID_HANDLE_VALUE;

	m_hPipePTYDownRead = INVALID_HANDLE_VALUE;
	m_hPipePTYUpWrite= INVALID_HANDLE_VALUE;

	SECURITY_ATTRIBUTES saAttr = {};

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = FALSE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create the pipes to which the ConPTY will connect
	if (!CreatePipe(&m_hPipePTYDownRead, &m_hPipePTYDownWrite, &saAttr, 0))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	if(!CreatePipe(&m_hPipePTYUpRead, &m_hPipePTYUpWrite, &saAttr, 0))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// Determine required size of Pseudo Console
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	if (!GetConsoleScreenBufferInfo(m_hStdOut, &csbi))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	m_ConsoleSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	m_ConsoleSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	// Create the Pseudo Console of the required size, attached to the PTY-end of the pipes
	hr = CreatePseudoConsole(m_ConsoleSize, m_hPipePTYDownRead, m_hPipePTYUpWrite, PSEUDOCONSOLE_INHERIT_CURSOR, &m_hPC);
	if (hr != S_OK)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	// Note: We can close the handles to the PTY-end of the pipes here
	// because the handles are dup'ed into the ConHost and will be released
	// when the ConPTY is destroyed.
	CloseHandle(m_hPipePTYUpWrite);
	m_hPipePTYUpWrite = nullptr;
	CloseHandle(m_hPipePTYDownRead);
	m_hPipePTYDownRead = nullptr;

	// Initialize the necessary startup info struct        
	m_si = {};
	size_t attrListSize{};
	m_si.StartupInfo.cb = sizeof(STARTUPINFOEX);

	// Get the size of the thread attribute list.
	InitializeProcThreadAttributeList(NULL, 1, 0, &attrListSize);

	// Allocate a thread attribute list of the correct size
	m_si.lpAttributeList =
		reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(malloc(attrListSize));

	// Initialize thread attribute list
	if (!m_si.lpAttributeList)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(!InitializeProcThreadAttributeList(m_si.lpAttributeList, 1, 0, &attrListSize))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	// Set Pseudo Console attribute

	if (!UpdateProcThreadAttribute(
		m_si.lpAttributeList,
		0,
		PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
		m_hPC,
		sizeof(HPCON),
		NULL,
		NULL))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (!CreateProcess(
		NULL,                           // No module name - use Command Line
		tempArg.GetBufferW(),                      // Command Line
		NULL,                           // Process handle not inheritable
		NULL,                           // Thread handle not inheritable
		FALSE,                          // Inherit handles
		EXTENDED_STARTUPINFO_PRESENT,   // Creation flags
		NULL,                           // Use parent's environment block
		NULL,                           // Use parent's starting directory 
		&m_si.StartupInfo,       // Pointer to STARTUPINFO
		&m_pi))                      // Pointer to PROCESS_INFORMATION
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	m_ParamDetectEnd.pPI = &(this->m_pi);
	m_bConnected = true;
	m_ParamDetectEnd.pThisThread = new std::thread(&ThreadProcDetectEnd, &m_ParamDetectEnd);

	m_PipeTermPSide.SetHandle(m_hPipePTYUpRead, m_hPipePTYDownWrite);
	m_PipeTermPSide.Connect();
	m_PipeTermStdSide.SetHandle(m_hStdIn, m_hStdOut);
	m_PipeTermStdSide.Connect();
	m_ParamForward.m_bEnd = false;
	m_ParamForward.pThisThread = new std::thread(&ThreadProcForward, &m_ParamForward);

	return S_OK;
}


void CPConsole::OnEvInternalDetectEnd()
{
	;
}

void CPConsole::ThreadProcDetectEnd(stParamDetectEnd* pParam)
{
	pParam->rVal = WaitForSingleObject(pParam->pPI->hThread, INFINITE);
	CloseHandle(pParam->pPI->hThread);
	pParam->pPI->hThread = nullptr;
	CloseHandle(pParam->pPI->hProcess);
	pParam->pPI->hProcess = nullptr;
	*pParam->pbConnected = false;
	if (*(pParam->ppEvOnDisconnect) != nullptr)
	{
		(*(pParam->ppEvOnDisconnect))();
	}
	(pParam->pEvOnInternalDetectEnd)();
}

void CPConsole::ThreadProcForward(stParamForwarder* pParam)
{
	while (
		!pParam->m_bEnd
		&& pParam->pStdSide->IsConnect()
		&& pParam->pPSide->IsConnect()
		)
	{
		switch (pParam->m_emPInterceptMode)
		{
		case emInterceptMode::Off:

			if (pParam->pStdSide->Read(pParam->bstr) > 0)
			{
				pParam->pPSide->Write(pParam->bstr);
			}

			if (pParam->pPSide->Read(pParam->bstr) > 0)
			{
				pParam->pStdSide->Write(pParam->bstr);
			}
			break;
		case emInterceptMode::Cover:
			if (pParam->pStdSide->Read(pParam->bstr) > 0)
			{
				pParam->pPSide->Write(pParam->bstr);
				pParam->pInterStdSide->Write(pParam->bstr);
			}

			if (pParam->pPSide->Read(pParam->bstr) > 0)
			{
				pParam->pStdSide->Write(pParam->bstr);
				pParam->pInterPSide->Write(pParam->bstr);
			}

			if (pParam->pInterStdSide->Read(pParam->bstr) > 0)
			{
				pParam->pStdSide->Write(pParam->bstr);
			}

			if (pParam->pInterPSide->Read(pParam->bstr) > 0)
			{
				pParam->pPSide->Write(pParam->bstr);
			}
			break;
		case emInterceptMode::Steal:
			if (pParam->pStdSide->Read(pParam->bstr) > 0)
			{
				pParam->pInterStdSide->Write(pParam->bstr);
			}

			if (pParam->pInterStdSide->Read(pParam->bstr) > 0)
			{
				pParam->pStdSide->Write(pParam->bstr);
			}

			if (pParam->pPSide->Read(pParam->bstr) > 0)
			{
				pParam->pInterPSide->Write(pParam->bstr);
			}

			if (pParam->pInterPSide->Read(pParam->bstr))
			{
				pParam->pPSide->Write(pParam->bstr);
			}
			break;
		default:
			break;
		}
//				std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

