// CSourceEdit.cpp : 実装ファイル
//

#include "pch.h"
#include "PipeProbeSetting.h"
#include "CSourceEdit.h"


// CSourceEdit

IMPLEMENT_DYNAMIC(CSourceEdit, CEdit)

CSourceEdit::CSourceEdit()
{

}

CSourceEdit::~CSourceEdit()
{
}


BEGIN_MESSAGE_MAP(CSourceEdit, CEdit)
//	ON_WM_CHARTOITEM()
ON_WM_CHAR()
ON_WM_KEYDOWN()
ON_WM_KEYUP()
END_MESSAGE_MAP()



// CSourceEdit メッセージ ハンドラー




BOOL CSourceEdit::PreTranslateMessage(MSG* pMsg)
{
	BOOL b;
	switch (pMsg->message)
	{
	case WM_KEYDOWN:

		switch (pMsg->wParam)
		{
		case VK_TAB:
//			b = IsDialogMessage(pMsg);
			if ( TRUE)
			{
//				::TranslateMessage(pMsg);
//				::DispatchMessage(pMsg);
			}
			break;
		case VK_RETURN:
//			b = IsDialogMessage(pMsg);
			//if (b == TRUE)
			//{
			//}
//			return TRUE;
			break;
		default:
			break;
		}
	default:
		break;
	}
	return CEdit::PreTranslateMessage(pMsg);
}

void CSourceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == '\t')
	{
//		ReplaceSel(_T("\t"), FALSE);
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

