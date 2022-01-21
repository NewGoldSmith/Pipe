#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <CBStream.h>
class CProtocolBD;
class CProtocolBD:public CBStream
{
public:
	CProtocolBD();
	virtual ~CProtocolBD();
	virtual bool Init(CBStream* pBDStream);
	virtual int Read(CBinaryString& BD);			//NonBlocked Read.
	virtual int Write(const CBinaryString& BD);	//NonBlocked Write.
protected:
	CBStream* m_pBDStream;
};

