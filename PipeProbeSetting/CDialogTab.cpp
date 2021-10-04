// CDialogTab.cpp : 実装ファイル
//

#include "pch.h"
#include "PipeProbeSettingApp.h"
#include "CDialogTab.h"


// CDialogTab

IMPLEMENT_DYNAMIC(CDialogTab, CTabCtrl)

CDialogTab::CDialogTab()
{

}

CDialogTab::~CDialogTab()
{
}


BEGIN_MESSAGE_MAP(CDialogTab, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, &CDialogTab::OnTcnSelchanging)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CDialogTab::OnTcnSelchange)
	ON_WM_UPDATEUISTATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CDialogTab メッセージ ハンドラー




LONG CDialogTab::InsertDlg(int i, CProfileableDlg* pDlg,CString *pstr)
{
	TCITEM tcItem = {};
	CString str;
	CRect rect, rectItem;
	GetClientRect(&rect);
	GetItemRect(0, &rectItem);
	rect.top = rect.top + rectItem.bottom;
	rect.InflateRect(-10, -10, -10, -10);
	tcItem.pszText = pstr->GetBuffer();
	tcItem.cchTextMax = CHAR_MAX;
	tcItem.mask = TCIF_TEXT | TCIF_PARAM;
		pDlg->MoveWindow(&rect);

	tcItem.lParam = (LPARAM)pDlg;
	InsertItem(i, &tcItem);
	pstr->ReleaseBuffer();

	return 0;
}

BOOL CDialogTab::DeleteDlg(int nItem)
{
	return DeleteItem( nItem);
}


void CDialogTab::OnTcnSelchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	int i;
	i=GetCurSel();
	TCITEM tcItem = {};
	tcItem.mask = TCIF_PARAM;
	GetItem(i, &tcItem);
	CDialog* pDlg = (CDialog * )tcItem.lParam;
	pDlg->ShowWindow(SW_HIDE);
	*pResult = 0;
}


void CDialogTab::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	int i;
	i = GetCurSel();
	TCITEM tcItem = {};
	tcItem.mask = TCIF_PARAM;
	GetItem(i, &tcItem);
	CDialog* pDlg = (CDialog*)tcItem.lParam;
	pDlg->ShowWindow(SW_SHOW);
	*pResult = 0;
}


void CDialogTab::OnUpdateUIState(UINT /*nAction*/, UINT /*nUIElement*/)
{
	int i;
	i = GetCurSel();
	TCITEM tcItem = {};
	tcItem.mask = TCIF_PARAM;
	GetItem(i, &tcItem);
	CDialog* pDlg = (CDialog*)tcItem.lParam;
	pDlg->ShowWindow(SW_SHOW);

}


void CDialogTab::LoadProfileApp(CWinApp* pApp)
{
	int i,j;
	i = GetItemCount();
	TCITEM tcItem = {};
	tcItem.mask = TCIF_PARAM;
	for (j=0; j < i; j++)
	{
		GetItem(j, &tcItem);
		CProfileableDlg* pDlg = (CProfileableDlg *)tcItem.lParam;
		pDlg->LoadProfileApp(pApp);
	}

}


void CDialogTab::SaveProfileApp(CWinApp* pApp)
{
	int i, j;
	i = GetItemCount();
	TCITEM tcItem = {};
	tcItem.mask = TCIF_PARAM;
	for (j = 0; j < i; j++)
	{
		GetItem(j, &tcItem);
		CProfileableDlg* pDlg = (CProfileableDlg*)tcItem.lParam;
		pDlg->SaveProfileApp(pApp);
	}

}



BOOL CDialogTab::PreTranslateMessage(MSG* pMsg)
{
	BOOL b;
//	CWnd* pWnd;
	int nItem;
	TCITEM tcItem = {};
	tcItem.mask = TCIF_PARAM;
	CProfileableDlg* pDlg;
	SHORT key;
	switch (pMsg->message)
	{
	case WM_KEYDOWN:

		switch (pMsg->wParam)
		{
		case VK_DOWN:
			//ctl+↓は子ダイアログにフォーカス移動
			key = GetKeyState(VK_CONTROL);
			if (key < 0)
			{
				nItem = GetCurSel();
				GetItem(nItem, &tcItem);
				pDlg = (CProfileableDlg*)tcItem.lParam;
				pDlg->SetFocus();
				return TRUE;
			}
			else
			{
				pMsg->wParam = VK_TAB;
				break;
			}
		case VK_UP:
		case VK_TAB:
			break;
		case VK_RETURN:
			b = 1;
			break;
		default:
			break;
		}
	default:
		break;
	}
	return CTabCtrl::PreTranslateMessage(pMsg);
}

//使ってない。削除予定。
CWnd* CDialogTab::GetFocusBroadCast()
{
	int i;
	CWnd* pDlgItem;
	i = GetCurSel();
	TCITEM tcItem = {};
	tcItem.mask = TCIF_PARAM;
	GetItem(i, &tcItem);
		CProfileableDlg* pDlg = (CProfileableDlg*)tcItem.lParam;
		pDlgItem=pDlg->GetFocus();

	return pDlgItem;
}


BOOL CDialogTab::IsChanged()
{
	int i, j;
	BOOL bChange=FALSE;
	i = GetItemCount();
	TCITEM tcItem = {};
	tcItem.mask = TCIF_PARAM;
	for (j = 0; j < i; j++)
	{
		GetItem(j, &tcItem);
		CProfileableDlg* pDlg = (CProfileableDlg*)tcItem.lParam;
		bChange+=pDlg->Changed();
	}
	return bChange;
}




void CDialogTab::SaveProfilesApp(LPCTSTR lpPath)
{
	LPCTSTR pstrOld;
	CWinApp* pApp = AfxGetApp();
	pstrOld = pApp->m_pszProfileName;
	pApp->m_pszProfileName = lpPath;
	SaveProfileApp(pApp);
	pApp->m_pszProfileName = pstrOld;
}


void CDialogTab::LoadProfilesApp(LPCTSTR lpPath)
{
	LPCTSTR lpstrOld;
	CWinApp* pApp = AfxGetApp();
	lpstrOld = pApp->m_pszProfileName;
	pApp->m_pszProfileName = lpPath;
	LoadProfileApp(pApp);
	pApp->m_pszProfileName = lpstrOld;
}
