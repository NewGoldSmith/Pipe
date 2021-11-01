// CListBoxComboSub.cpp : 実装ファイル
//

#include "pch.h"
#include "MasteringWinSock.h"
#include "CListBoxComboSub.h"


// CListBoxComboSub

IMPLEMENT_DYNAMIC(CListBoxComboSub, CListBox)

CListBoxComboSub::CListBoxComboSub()
{

}

CListBoxComboSub::~CListBoxComboSub()
{
}


BEGIN_MESSAGE_MAP(CListBoxComboSub, CListBox)
END_MESSAGE_MAP()



// CListBoxComboSub メッセージ ハンドラー




void CListBoxComboSub::Init(CEditComboSub* pEdit)
{
	m_pEdit = pEdit;
}
