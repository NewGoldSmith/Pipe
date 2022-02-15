//Copyright (c) 2021, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php

#pragma once
#include <CBStream.h>
#include <thread>
#include <semaphore>

class CJointIntercept 
{
public:
	typedef std::function<void(void) > type_pJointEventHandler;
	CJointIntercept();
	CJointIntercept(CJointIntercept& OtherObj) = delete;
	CJointIntercept(CJointIntercept&& OtherObj) = delete;
	CJointIntercept& operator()(CJointIntercept& OtherObj) = delete;
	CJointIntercept& operator = (CJointIntercept& OtherObj) = delete;
	CJointIntercept& operator()(CJointIntercept&& OtherObj) = delete;
	CJointIntercept& operator = (CJointIntercept&& OtherObj) = delete;
	virtual ~CJointIntercept();
	virtual void SetStream(CBStream* pStCon, CBStream* pStIntercept, CBStream* pStPsd);
	virtual void SetInterceptMode(bool onoff);
	virtual void SetInterceptWriteOnlyMode(bool onoff);
	virtual void StartWork();
	virtual void StopWork();
	virtual bool IsRunning()const;
	virtual void SetEventHandler(const type_pJointEventHandler pEvent);
	virtual void ClearEventHandler();
protected:
	struct stThreadParam {
		stThreadParam();
		CBStream* pStCon;
		CBStream* pStIntercept;
		CBStream* pStPsd;//StreamPseudoConsole
		CBinaryString bstr;
		bool* pEnd;
		std::thread* pThisThread;
		std::counting_semaphore<1> MtxEvent;
		type_pJointEventHandler* ppEvOnDisconnect;
		bool* pIsInterceptMode;
		bool* pIsInterceptWriteOnlyMode;
	};
	bool m_bEnd;
	bool m_bIsInterceptMode;
	bool m_bIsInterceptWriteOnlyMode;
	stThreadParam m_ThreadParam;
	static void ThreadProc(stThreadParam* pThreadParam);
	type_pJointEventHandler m_pEvOnDisconnect;

};

