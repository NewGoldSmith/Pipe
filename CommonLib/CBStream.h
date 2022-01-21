#pragma once
#include <functional>
#include <CBinaryString.h>

class CBStream
{
public:
	typedef std::function<void(void) > type_pStreamEventHandler;
	CBStream();
	virtual ~CBStream();
	virtual int Read(CBinaryString& BD) = 0;		//ASync
	virtual int Write(const CBinaryString& BD) = 0;//ASync
	virtual bool IsConnect();
	virtual void SetEventHandler(type_pStreamEventHandler pEvent);
	virtual void ClearEventHandler();

protected:
	bool m_bConnected;
	type_pStreamEventHandler m_pEvOnDisconnect;
};

