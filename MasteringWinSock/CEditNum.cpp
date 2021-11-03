// CEditNum.cpp : 実装ファイル
//

#include "pch.h"
#include "MasteringWinSock.h"
#include "CEditNum.h"


// CEditNum

IMPLEMENT_DYNAMIC(CEditNum, CEdit)

CEditNum::CEditNum()
{

}

CEditNum::~CEditNum()
{
}


BEGIN_MESSAGE_MAP(CEditNum, CEdit)
END_MESSAGE_MAP()



// CEditNum メッセージ ハンドラー




int CEditNum::GetNum()
{
    CString str;
    GetWindowText(str);
    return _ttoi(str);
}


void CEditNum::SetNum(int num)
{
    TCHAR str[256];
    _itot_s(num, str, 256, 10);
    SetWindowText(str);
}
