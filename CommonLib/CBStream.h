//Copyright(c) 2022, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php
// 
// READ, WRITE return value.
//In the case of abnormal termination, the return value is -1.
//In the case of normal termination, the return value is the 
// processed data size.
// SetEventHandler Description
// e.g. SetEventHandler(std::bind(&oneclass::func,this));
//
#pragma once
#include <functional>
#include <CBinaryString.h>
class CBStream
{
public:
	typedef std::function<void(void) > type_pStreamEventHandler;
	enum class emAccessMode { No, Read, Write, Both };
	CBStream();
	virtual ~CBStream();
	virtual int Read(CBinaryString& BD);		
	virtual int Write(const CBinaryString& BD);
	virtual bool Connect();
	virtual bool Disconnect();
	virtual bool IsConnect();
	virtual int Discard();
	virtual void SetEventHandler(const type_pStreamEventHandler pEvent);
	virtual void ClearEventHandler();
	virtual emAccessMode SetAccessMode(const emAccessMode emaccess);
protected:
	bool m_bConnected;
	type_pStreamEventHandler m_pEvOnDisconnect;
	emAccessMode m_emAccessMode;
};

