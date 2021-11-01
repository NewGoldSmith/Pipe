#pragma once

#include <afxsock.h>


// CAsyncSocketDX コマンド ターゲット
#ifndef BUF_SIZE
#define BUF_SIZE 8192
#endif
class CAsyncSocketDX : public CAsyncSocket
{
public:
	const enum class Attri{Undef,SevListen,Sev,Cli};
	const enum class ConText{Undef,New,Create,Listen,Close,OnClose,Accept,OnAccept,OnConnect,Connect,Send,OnSend,Receive,OnReceive, OnOutOfBandData,ShutDown,Err};
	const enum class Event{Undef,OnAccept,OnClose,OnConnect,OnReceive,OnSend, OnOutOfBandData,Err};
	const enum class UserErrCode { Undef, HasBeenCreated,HasBeenLitened,HasBeenOnAccepted,HasBeenShutDowned,NoUserErr };
	CAsyncSocketDX();
	virtual ~CAsyncSocketDX();
	void Init(UINT dwMessage, CWnd* pWnd, LONG ID, Attri attri=Attri::Undef);
	CStringA GetString();
	virtual BOOL Accept(CAsyncSocketDX& rConnectedSocket,	SOCKADDR* lpSockAddr = NULL,int* lpSockAddrLen = NULL);
protected:
	char m_chBuf[BUF_SIZE] = {};
	CWnd* m_pWnd = NULL;
	UINT m_ID = 0;
	UINT m_uiWM_MESSAGE;
public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
protected:
	LONG m_lBufSize;
	ConText m_eConText;

	Attri m_eAttri;
public:
	void SetAttri(Attri attri);
protected:
	int m_OnAcceptCount;
public:
	UINT GetID();
	UINT GetOnAcceptCount();
	virtual BOOL Listen(int nConnectionBacklog=5);
	CString CodeToString(int code);
	void ErrMessageBox();
protected:
	int m_Err;
public:
	CAsyncSocketDX::ConText GetConText();
	CAsyncSocketDX::ConText SetConText(CAsyncSocketDX::ConText eConText);
	CString GetConTextString();
	CString GetAttributeString();
	CString GetPeerAddressString();
	CString GetPeerPortString();
	BOOL Create(UINT nSocketPort=0, int nSocketType = SOCK_STREAM, long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, LPCTSTR lpszSocketAddress = NULL);
	void Close();
	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
protected:
	UserErrCode m_eUserErr;
public:
	int Receive(void* lpBuf, int nBufLen, int nFlags);
	BOOL ShutDown(int nHow= sends);
	int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
protected:
	void ClearErrCode();
};


