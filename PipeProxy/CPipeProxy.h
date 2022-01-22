#pragma once
//#include <WTypesbase.h>
#include <WinExeEngine.h>
#include <WinSock2.h>
#include <CBSSocket.h>
#include <CPipeTerm.h>
#include <CProtoNPCP11.h>
class CPipeProxy :
    public CNPCP11
{
protected:
    CPipeProxy(CPipeProxy& otherObj) = delete;
    CPipeProxy& operator = (CPipeProxy& otherObj) = delete;
    CPipeProxy& operator ()(CPipeProxy& otherObj) = delete;
public:
    CPipeProxy();
    virtual ~CPipeProxy();
    bool OneWork();
    virtual bool Init(CBStream* pBDStream, emDeviceType emThisType, std::string strThisID);

protected:
    virtual bool OnLowEXEC(const CBinaryString& bstr);
    virtual bool OnLowNOTE(const CBinaryString& bstr);
    virtual bool OnLowQUIT(const CBinaryString& bstr);
    virtual bool OnLowSEND(const CBinaryString& bstr);
    typedef std::function<void (void) > type_pExeEventHandler;
    type_pExeEventHandler pEventFunction;
    void OnEvExeEnd();
    void OnEvSevDisconnected();
    WinExeEngine m_ExeEngine;
    HANDLE m_hChildUpRd;
    HANDLE m_hChildDownWr;
    CPipeTerm m_PipeChild;
    bool m_bEndFlag;
};

