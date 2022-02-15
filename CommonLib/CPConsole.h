//Copyright(c) 2022, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php

//
//This class is a program that creates a pseudo console
//and sends commands with a pipe.
//e.g.
//// Select the following and press Ctrl + U.
//#include <CPConsole.h>
//int main()
//{
//	CBinaryString str;
//	str.SetTextStringW(L"cmd.exe");
//	CPConsole con;
//	con.SetCommand(str);
//	con.SetAccessMode(CPConsole::emAccessMode::Write);
//	con.SetStdAccessMode(CPConsole::emAccessMode::No);
//	con.SetInterceptMode(CPConsole::emInterceptMode::Cover);
//	con.Connect();
//	con.Write("ping localhost\r\n");
//
//	while (con.IsConnect())
//	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
//	}
//}


#pragma once
#include <CBStream.h>
#include <CPipeTerm.h>
#include <CJointStream.h>
#include <CJointIntercept.h>
#include <CAsyncBStreamQue.h>
#include <CBStreamQue.h>
#include <CBStreamTerm.h>


class CPConsole :
    public CBStream
{
public:
	enum class emInterceptMode{Off,Cover,Steal};
	typedef std::function<void(void) > type_pCPConsoleEventHandler;
	CPConsole();
	CPConsole(const CPConsole& otherobj) = delete;
	CPConsole(const CPConsole&& otherobj) = delete;
	CPConsole& operator = (const CPConsole& otherobj) = delete;
	CPConsole& operator = (const CPConsole&& otherobj) = delete;
	CPConsole& operator ()(const CPConsole& otherobj) = delete;
	CPConsole& operator ()(const CPConsole&& otherobj) = delete;
	virtual ~CPConsole();
	virtual int Read(CBinaryString& BD);		
	virtual int Write(const CBinaryString& BD);
	virtual int StdRead(CBinaryString& bstr);
	virtual int StdWrite(const CBinaryString& bstr);
	virtual bool Connect();
	virtual bool Disconnect();
	virtual bool IsConnect();
	virtual int Discard();
	virtual void SetEventHandler(const type_pStreamEventHandler pEvent);
	virtual void ClearEventHandler();
	virtual void CloseConsole();
	virtual emAccessMode SetAccessMode(const emAccessMode emaccess);
	virtual emAccessMode SetStdAccessMode(const emAccessMode emaccess);
	virtual emInterceptMode SetInterceptMode(const emInterceptMode emIntcpt);
	virtual emInterceptMode GetInterceptMode();
	virtual void SetCommand(const CBinaryString& bstr);
protected:
	virtual HRESULT OpenConsole(const CBinaryString &bstr);
	CBinaryString m_strCommand;
	CBinaryString m_strLastMessageStd;
	CBinaryString m_strLastMessageP;
	CPipeTerm m_PipeTermPSide;
	CPipeTerm m_PipeTermStdSide;
	CBStreamQue m_Que1;
	CBStreamQue m_Que2;
	CBStreamQue m_Que3;
	CBStreamQue m_Que4;
	CBStreamTerm m_TermThStdSide;
	CBStreamTerm m_TermDlgStdSide;
	CBStreamTerm m_TermThPSide;
	CBStreamTerm m_TermDlgPSide;
	HANDLE m_hPipePTYDownRead;//ConHostSide
	HANDLE m_hPipePTYDownWrite;
	HANDLE m_hPipePTYUpRead;
	HANDLE m_hPipePTYUpWrite;//ConHostSide
	HANDLE m_hStdIn;
	HANDLE m_hStdOut;
	HPCON	m_hPC;
	DWORD m_ConsoleModeInOld;
	DWORD m_ConsoleModeOutOld;
	COORD m_ConsoleSize;
	STARTUPINFOEX m_si;
	PROCESS_INFORMATION m_pi;//ConHostSide
	bool m_bEnd;
	bool m_bForcedTermination;
	struct stParamDetectEnd {
		stParamDetectEnd();
		PROCESS_INFORMATION* pPI;
		std::thread* pThisThread;
		int rVal;
		bool *pbConnected;
		type_pStreamEventHandler* ppEvOnDisconnect;
		type_pCPConsoleEventHandler pEvOnInternalDetectEnd;
	};
	stParamDetectEnd m_ParamDetectEnd;
	void OnEvInternalDetectEnd();
	static void ThreadProcDetectEnd(stParamDetectEnd* pParam);
	struct stParamForwarder {
		stParamForwarder();
		CBStream* pStdSide;
		CBStream* pPSide;
		CBStream* pInterStdSide;
		CBStream* pInterPSide;
		bool* pbConnected;
		CBinaryString bstr;
		emInterceptMode m_emPInterceptMode;
		emInterceptMode m_emStdInterceptMode;
		bool m_bEnd;
		std::counting_semaphore<1> Mtx;
		std::thread* pThisThread;
	};
	stParamForwarder m_ParamForward;
	static void ThreadProcForward(stParamForwarder* pParam);
};

