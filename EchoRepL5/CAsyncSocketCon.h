#pragma once
#include <afxsock.h>
//コンソール用カスタマイズ
class CAsyncSocketCon :
    public CAsyncSocket
{

public:
    virtual void OnAccept(int nErrorCode);
    virtual void OnClose(int nErrorCode);
    virtual void OnConnect(int nErrorCode);
    virtual void OnReceive(int nErrorCode);
    virtual void OnSend(int nErrorCode);
};

