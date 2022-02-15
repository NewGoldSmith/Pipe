//Copyright (c) 2021, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php

//Stream end.
//can be combined to create a bidirectional stream.
//It is assumed to set CBStreamQue.
// e.g.
// #include <iostream>
// #include <CBStreamQue.h>
// #include <CBStreamTerm.h>
//int main()
//{
//  CBinaryString str;
//  CBStreamQue que1, que2;
//  CBStreamTerm term1, term2;
//  term1.SetStream(&que1, &que2);
//  term2.SetStream(&que2, &que1);
//
//  term1.Write("test1\r\n");
//  term2.Read(str);
//  std::cout << str.c_strA();
//  term2.Write("test2\r\n");
//  term1.Read(str);
//  std::cout << str.c_strA();
//}

#pragma once
#include <CBStream.h>
#include <functional>
#include <CBStreamQue.h>

class CBStreamTerm;
class CBStreamTerm :
    public CBStream
{
public:
    CBStreamTerm();
    CBStreamTerm(const CBStreamTerm& otherobj) = delete;
    CBStreamTerm(const CBStreamTerm&& otherobj) = delete;
    CBStreamTerm& operator = (const CBStreamTerm& otherobj) = delete;
    CBStreamTerm& operator =(const CBStreamTerm&& otherobj) = delete;
    CBStreamTerm& operator ()(const CBStreamTerm& otherobj) = delete;
    CBStreamTerm& operator ()(const CBStreamTerm&& otherobj) = delete;
    virtual ~CBStreamTerm();
    virtual int Read(CBinaryString& BD);		
    virtual int Write(const CBinaryString& BD);
    virtual bool Connect();
    virtual bool Disconnect();
    virtual bool IsConnect();
    virtual void SetStream(CBStream *pStreamRead, CBStream *pStreamWrite);
    virtual void ClearStream();
    virtual int Discard();
    virtual emAccessMode SetAccessMode(const emAccessMode emaccess);
protected:
    CBStream* m_pStreamRead;
    CBStream* m_pStreamWrite;
};

