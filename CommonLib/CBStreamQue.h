//Copyright(c) 2022, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php
// 
#pragma once
#include <CBStream.h>
#include <queue>
#include <semaphore>

//It is one QUEUE, pushed with WRITE and pops with READ.
//The connection status is "connected" in the initial state.

class CBStreamQue :
    public CBStream
{
public:
    CBStreamQue();
    CBStreamQue(const CBStreamQue& otherobj)=delete;
    CBStreamQue(const CBStreamQue&& otherobj) = delete;
    CBStreamQue& operator =(const CBStreamQue& otherobj)=delete;
    CBStreamQue& operator =(const CBStreamQue&& otherobj) = delete;
    CBStreamQue& operator ()(const CBStreamQue& otherobj)=delete;
    CBStreamQue& operator ()(const CBStreamQue&& otherobj) = delete;
    virtual ~CBStreamQue();
    virtual int Read(CBinaryString& BD);
    virtual int Write(const CBinaryString& BD);
    virtual bool Connect();
    virtual bool Disconnect();
    virtual int Discard();
protected:
    std::queue<CBinaryString> m_BStringQue;
    std::counting_semaphore<1> m_Mtx;
};

