#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <CBinaryString.h>
#include <CBStream.h>
#include <CSecTimer.h>
using namespace std;
//NetPipeControllProtocolのプレースホルダ
//クライアントからの接続通知は実装。
class CNPCP11 :
	public CBStream
{
protected:
	typedef std::function<bool (const CBinaryString&) > type_pEventHandler;
	multimap<string, type_pEventHandler> mapLowFirstWords;
	multimap<string, type_pEventHandler> mapUpFirstWords;
public:
	CNPCP11();
	virtual ~CNPCP11();
	enum class emDeviceType { UNDEF, PXY, NPHUB, STUB, FORWARDER };
	virtual bool Init(CBStream* pBDStream, emDeviceType emThisType, std::string strThisID);

//Called from Upper Layer. NonBlocked Read.
	virtual int Read(CBinaryString& BD);			
	//Called from Upper Layer. NonBlocked Write. 
	virtual int Write(const CBinaryString& BD);	
	bool LowIN(const CBinaryString& bstr);
	bool UpIN(const CBinaryString& bstr);
	bool UpInternalIN(const CBinaryString& bstr);
	bool ConnectNPCPServer();
protected:
	CBStream* m_pBStream;
	std::queue<CBinaryString> m_queReadForUpper;
	std::queue<CBinaryString> m_queWriteForUpper;
	multimap<string, emDeviceType> mapDeviceType;
	emDeviceType m_ThisDeviceType;
	emDeviceType m_PeerDeviceType;
	CBinaryString m_strThisID;
	CBinaryString m_strPeerID;
	bool m_bConnected;
	virtual bool OnLowNPCP(const CBinaryString &bstr);
	virtual bool OnLowIM(const CBinaryString& bstr);
	virtual bool OnLowEXEC(const CBinaryString& bstr);
	virtual bool OnLowNOTE(const CBinaryString& bstr);
	virtual bool OnLowQUIT(const CBinaryString& bstr);
	virtual bool OnLowSEND(const CBinaryString& bstr);
	virtual bool OnLowREP(const CBinaryString& bstr);
	virtual bool OnUpNPCP(const CBinaryString& bstr);
	virtual bool OnUpEXEC(const CBinaryString& bstr);
	virtual bool OnUpNOTE(const CBinaryString& bstr);
	virtual bool OnUpQUIT(const CBinaryString& bstr);
	virtual bool OnUpSEND(const CBinaryString& bstr);
	virtual bool OnUpREP(const CBinaryString& bstr);
};

