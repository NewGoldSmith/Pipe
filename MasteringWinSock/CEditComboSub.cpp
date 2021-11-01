// CEditComboSub.cpp : 実装ファイル
//

#include "pch.h"
#include "MasteringWinSock.h"
#include "CEditComboSub.h"


// CEditComboSub

IMPLEMENT_DYNAMIC(CEditComboSub, CEdit)

CEditComboSub::CEditComboSub()
{

}

CEditComboSub::~CEditComboSub()
{
}


BEGIN_MESSAGE_MAP(CEditComboSub, CEdit)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CEditComboSub メッセージ ハンドラー




void CEditComboSub::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int Index;
	if (nChar == 0x2e)
	{
		if (m_pListBox->GetSafeHwnd()!=NULL)
		{
			Index = m_pListBox->GetCurSel();
			if (Index != -1)
			{
				m_pListBox->DeleteString(Index);
			}
		}
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CEditComboSub::Init(CListBoxComboSub* pListBox)
{
	m_pListBox = pListBox;
}


BOOL CEditComboSub::PreTranslateMessage(MSG* pMsg)
{
	CWnd* pWnd;
	switch (pMsg->message)
	{
	case WM_KEYDOWN:

		switch (pMsg->wParam)
		{
		case VK_TAB:
			break;
		case VK_RETURN:
			if (m_pButton != NULL)
			{
				m_pButton->PostMessage(WM_LBUTTONDOWN, 0, MAKELONG(1, 1));
			}
			return TRUE;
		default:
			break;
		}
	case WM_KEYUP:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			if (m_pButton != NULL)
			{
				m_pButton->PostMessage(WM_LBUTTONUP, 0, MAKELONG(1, 1));
			}
			return TRUE;
		default:
			break;
		}
	default:
		break;
	}

	return CEdit::PreTranslateMessage(pMsg);
}


CButton* CEditComboSub::SetButton(CButton* pButton)
{
	m_pButton = pButton;
	return nullptr;
}
