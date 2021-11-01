// CAsyncSocketDX.cpp : 実装ファイル
//

#include "pch.h"
#include "CAsyncSocketDX.h"


// CAsyncSocketDX

CAsyncSocketDX::CAsyncSocketDX()
    :m_uiWM_MESSAGE(WM_NULL)
    ,m_lBufSize(BUF_SIZE)
    ,m_eConText(CAsyncSocketDX::ConText::New)
    ,m_eAttri(CAsyncSocketDX::Attri::Undef)
{
    m_OnAcceptCount = 0;
    m_Err = 0;
    m_eUserErr = UserErrCode::Undef;
}

CAsyncSocketDX::~CAsyncSocketDX()
{
}


BOOL CAsyncSocketDX::Accept(CAsyncSocketDX& rConnectedSocket, SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{
    BOOL rVal = FALSE;
    if (rConnectedSocket.m_eConText == ConText::Create)
    {
        m_eUserErr = UserErrCode::HasBeenCreated;
        return FALSE;
    }
    rVal = CAsyncSocket::Accept(rConnectedSocket, lpSockAddr, lpSockAddrLen);
    if (!rVal)
    {
        m_Err = GetLastError();
        return rVal;
    }
    rConnectedSocket.m_eConText=ConText::Accept;
    m_OnAcceptCount--;
    return rVal;
}

// CAsyncSocketDX のメンバー関数


void CAsyncSocketDX::Init(UINT dwMessage, CWnd* pWnd, LONG ID,Attri attri)
{
    m_uiWM_MESSAGE = dwMessage;
    m_pWnd = pWnd;
    m_ID = ID;
    m_eAttri = attri;
}


CStringA CAsyncSocketDX::GetString()
{
    return CStringA(m_chBuf);
}


void CAsyncSocketDX::OnAccept(int nErrorCode)
{
    m_Err = nErrorCode;
    m_OnAcceptCount++;
    TRACE(_T("\r\nCAsyncSocketDX::OnAccept(%d) Attri=%d ID=%d\r\n"), nErrorCode, m_eAttri, m_ID);
    if (m_uiWM_MESSAGE != WM_NULL)
    {
        m_pWnd->PostMessage(m_uiWM_MESSAGE, m_ID, (LPARAM)CAsyncSocketDX::Event::OnAccept);
    }
    m_eConText = ConText::OnAccept;
    CAsyncSocket::OnAccept(nErrorCode);
}


void CAsyncSocketDX::OnClose(int nErrorCode)
{
    m_Err = nErrorCode;
    TRACE(_T("\r\nCAsyncSocketDX::OnClose(%d) Attri=%d ID=%d\r\n"), nErrorCode, m_eAttri, m_ID);
    m_eConText = ConText::OnClose;
    if (m_uiWM_MESSAGE != WM_NULL)
    {
        m_pWnd->PostMessage(m_uiWM_MESSAGE, m_ID, (LPARAM)CAsyncSocketDX::Event::OnClose);
    }
    CAsyncSocket::OnClose(nErrorCode);
}


void CAsyncSocketDX::OnConnect(int nErrorCode)
{
    m_Err = nErrorCode;
    TRACE(_T("\r\nCAsyncSocketDX::OnConnect(%d) Attri=%d ID=%d\r\n"), nErrorCode, m_eAttri, m_ID);
    if (m_uiWM_MESSAGE != WM_NULL)
    {
        m_pWnd->PostMessage(m_uiWM_MESSAGE, m_ID, (LPARAM)CAsyncSocketDX::Event::OnConnect);
    }
    m_eConText = ConText::OnConnect;
    CAsyncSocket::OnConnect(nErrorCode);
}


void CAsyncSocketDX::OnOutOfBandData(int nErrorCode)
{
    m_Err = nErrorCode;
    TRACE(_T("\r\nCAsyncSocketDX::OnOutOfBandData(%d) Attri=%d ID=%d\r\n"), nErrorCode, m_eAttri,m_ID);
    int i = m_ID;
    if (m_uiWM_MESSAGE != WM_NULL)
    {
        m_pWnd->PostMessage(m_uiWM_MESSAGE, m_ID, (LPARAM)CAsyncSocketDX::Event::OnOutOfBandData);
    }
    m_eConText = ConText::OnOutOfBandData;
    CAsyncSocket::OnOutOfBandData(nErrorCode);
}


void CAsyncSocketDX::OnReceive(int nErrorCode)
{
    m_Err = nErrorCode;
    TRACE(_T("\r\nCAsyncSocketDX::OnReceive(%d) Attri=%d ID=%d\r\n"), nErrorCode,m_eAttri, m_ID);
    if (m_uiWM_MESSAGE != WM_NULL)
    {
        m_pWnd->PostMessage(m_uiWM_MESSAGE, m_ID, (LPARAM)CAsyncSocketDX::Event::OnReceive);
    }
    m_eConText = ConText::OnReceive;
    CAsyncSocket::OnReceive(nErrorCode);
}


void CAsyncSocketDX::OnSend(int nErrorCode)
{
    m_Err = nErrorCode;
    TRACE(_T("\r\nCAsyncSocketDX::OnSend(%d) Attri=%d ID=%d\r\n"), nErrorCode, m_eAttri, m_ID);
    if (m_uiWM_MESSAGE != WM_NULL)
    {
        m_pWnd->PostMessage(m_uiWM_MESSAGE, m_ID, (LPARAM)CAsyncSocketDX::Event::OnSend);
    }
    m_eConText = ConText::OnSend;
    CAsyncSocket::OnSend(nErrorCode);
}



void CAsyncSocketDX::SetAttri(Attri attri)
{
    m_eAttri = attri;
}


UINT CAsyncSocketDX::GetID()
{
    return m_ID;
}


UINT CAsyncSocketDX::GetOnAcceptCount()
{
    return m_OnAcceptCount;
}


BOOL CAsyncSocketDX::Listen(int nConnectionBacklog)
{
    TRACE(_T("\r\nCAsyncSocketDX::Listen(%d)ID=%d\r\n"), nConnectionBacklog,m_ID);
    BOOL rVal = CAsyncSocket::Listen(nConnectionBacklog);
    if (!rVal)
    {
        m_Err=GetLastError();
        return rVal;
    }
    m_eConText = ConText::Listen;
    return rVal;
}


CString CAsyncSocketDX::CodeToString(int ErrCode)
{
    CString str;
    switch (ErrCode) {
    case WSAEWOULDBLOCK:    return _T("WSAEWOULDBLOCK\r\nリソースは一時的に使用できません。");
    case WSAEISCONN:		return _T("WSAEISCONN\r\nソケットは既に接続されています。");
    case WSAEADDRINUSE:		return _T("WSAEADDRINUSE\r\nアドレスは既に使用中です。");
    case WSAEINVAL:			return _T("WSAEINVAL\r\n引数が無効です。");
    case WSAENOTSOCK:       return _T("WSAENOTSOCK\r\nソケット以外でのソケット操作です。");
    case WSAEADDRNOTAVAIL:  return _T("WSAEADDRNOTAVAIL\r\n要求されたアドレスを割り当てることができません。");
    case WSAECONNREFUSED:   return _T("WSAECONNREFUSED\r\n接続が拒否されました。");
    case WSAENOTCONN:       return _T("WSAENOTCONN\r\nソケットが接続されていません。");
    case WSAENETUNREACH:    return _T("WSAENETUNREACH\r\nネットワークに到達できません。");
    case WSAECONNABORTED:   return _T("WSAENETUNREACH\r\nソフトウェアによって接続が中断されました。");
    case WSAECONNRESET:     return _T("WSAECONNRESET\r\nピアによって接続がリセットされました。");
    case WSAESHUTDOWN:      return _T("WSAESHUTDOWN\r\nソケットのシャットダウン後に送信できません。");
    default:
        switch (m_eUserErr) {
        case UserErrCode::HasBeenCreated:
            return _T("既にCreateされています。\r\n通常は例外が発生します。");
        case UserErrCode::HasBeenLitened:
            return _T("既にListenされています。\r\n通常は例外が発生します。");
        case UserErrCode::HasBeenShutDowned:
            return _T("ShutDownされています。\r\n通常は例外が発生します。");
        case UserErrCode::HasBeenOnAccepted:
            return _T("OnAcceptedされています。\r\n通常は例外が発生します。");
        case UserErrCode::NoUserErr:
            return _T("ユーザーエラーは発生していません。");
        }
        
        str.Format(_T("その他のエラー　%d"), ErrCode);
        return str;
    }
    return CString();
}


void CAsyncSocketDX::ErrMessageBox()
{
    CString str;
    str.Format(_T("%s\r\n%s"),_T("CAsyncSocketDX\r\n"), CodeToString(m_Err));
    AfxMessageBox(str,MB_OK|MB_ICONASTERISK);
    //m_Err = 0;
    //m_eUserErr = UserErrCode::Undef;
}



CAsyncSocketDX::ConText CAsyncSocketDX::GetConText()
{
    return m_eConText;
}


CAsyncSocketDX::ConText CAsyncSocketDX::SetConText(CAsyncSocketDX::ConText eConText)
{
    ConText StaOld = m_eConText;
    m_eConText = eConText;
    return StaOld;
}


CString CAsyncSocketDX::GetConTextString()
{
	switch (m_eConText)
	{
    case ConText::Undef:
        return _T("Undef");
    case ConText::New:
        return _T("New");
    case ConText::Create:
		return _T("Create");
	case ConText::Listen:
		return _T("Listen");
	case ConText::Close:
		return _T("Close");
    case ConText::OnClose:
        return _T("OnClose");
    case ConText::Accept:
        return _T("Accept");
    case ConText::OnAccept:
        return _T("OnAccept");
    case ConText::Connect:
        return _T("Connect");
    case ConText::OnConnect:
        return _T("OnConnect");
    case ConText::Send:
        return _T("Send");
	case ConText::OnSend:
		return _T("OnSend");
    case ConText::Receive:
        return _T("Receive");
    case ConText::OnReceive:
        return _T("OnReceive");
    case ConText::OnOutOfBandData:
        return _T("OnOutOfBandData");
    case ConText::ShutDown:
        return _T("ShutDown");
    case ConText::Err:
        return _T("Err");
    default:
        return _T("Undef");
    }
	return _T("");
}


CString CAsyncSocketDX::GetAttributeString()
{
    switch (m_eAttri)
    {
    case Attri::Cli:
        return _T("Cli");
    case Attri::Sev:
        return _T("Sev");
    case Attri::SevListen:
        return _T("SevListen");
    default:
        return _T("Undef");
    }
    return CString();
}


CString CAsyncSocketDX::GetPeerAddressString()
{
    UINT uiPort = 0;
    CString strAddress;
    GetPeerName(strAddress, uiPort);
    return strAddress;
}


CString CAsyncSocketDX::GetPeerPortString()
{
    UINT uiPort;
    CString strAddress;
    CString strPort;
    GetPeerName(strAddress, uiPort);
    strPort.Format(_T("%u"), uiPort);
    return strPort;
}


BOOL CAsyncSocketDX::Create(UINT nSocketPort, int nSocketType, long lEvent, LPCTSTR lpszSocketAddress)
{
    if (m_eConText == ConText::Create)
    {
         m_eUserErr = UserErrCode::HasBeenCreated;
        return 0;
    }
    if (m_eConText == ConText::Listen)
    {
        m_eUserErr = UserErrCode::HasBeenLitened;
        return 0;
    }
    if (m_eConText == ConText::ShutDown)
    {
        m_eUserErr = UserErrCode::HasBeenShutDowned;
        return 0;
    }
    if (m_eConText == ConText::OnAccept)
    {
        m_eUserErr = UserErrCode::HasBeenOnAccepted;
        return 0;
    }
    BOOL bVal=CAsyncSocket::Create(nSocketPort, nSocketType, lEvent,lpszSocketAddress);
    if (!bVal)
    {
        m_Err=GetLastError();
        return 0;
    }
    m_eConText = ConText::Create;
    return bVal;
}


void CAsyncSocketDX::Close()
{
    m_eConText = ConText::Close;
    CAsyncSocket::Close();
}


BOOL CAsyncSocketDX::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	BOOL rVal = CAsyncSocket::Connect(lpszHostAddress, nHostPort);
    if (!rVal)
    {
        m_Err = GetLastError();
        if (m_Err != WSAEWOULDBLOCK)
        {
            return rVal;
        }
        m_eConText = ConText::Connect;
        //CAsyncSocketとしては正常な値とする。
        return TRUE;
    }
    m_eConText = ConText::Connect;
    return rVal;
}


int CAsyncSocketDX::Receive(void* lpBuf, int nBufLen, int nFlags)
{
    int bVal = CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
    if (bVal== SOCKET_ERROR)
    {
        m_Err = GetLastError();
        return bVal;
    }
    m_eConText = ConText::Receive;
    return bVal;
}


BOOL CAsyncSocketDX::ShutDown(int nHow)
{
    BOOL rVal =CAsyncSocket::ShutDown(nHow);
    if (!rVal)
    {
        m_Err = GetLastError();
        return rVal;
    }
    m_eConText = ConText::ShutDown;
    return rVal;
}


int CAsyncSocketDX::Send(const void* lpBuf, int nBufLen, int nFlags)
{
    int bVal = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
    if (bVal == SOCKET_ERROR)
    {
        m_Err = GetLastError();
        return bVal;
    }
    m_eConText = ConText::Send;
    return bVal;
}


void CAsyncSocketDX::ClearErrCode()
{
    m_Err = 0;
    m_eUserErr = CAsyncSocketDX::UserErrCode::NoUserErr;
}
