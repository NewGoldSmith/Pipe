// CProfileableDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "PipeProbeSettingApp.h"
#include "CProfileableDlg.h"
#include "afxdialogex.h"


// CProfileableDlg ダイアログ

IMPLEMENT_DYNAMIC(CProfileableDlg, CDialogEx)

CProfileableDlg::CProfileableDlg(UINT nIDTemplate, CWnd* pParentWnd)
	:CDialogEx(nIDTemplate, pParentWnd)
	, m_bChange(FALSE)
{

}
CProfileableDlg::CProfileableDlg()
	: CDialogEx()
	, m_bChange(FALSE)
{}

 
CProfileableDlg::~CProfileableDlg()
{
}

void CProfileableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProfileableDlg, CDialogEx)

END_MESSAGE_MAP()


// CProfileableDlg メッセージ ハンドラー


void CProfileableDlg::LoadProfileApp(CWinApp* pApp)
{
	// TODO: ここに実装コードを追加します.
}


void CProfileableDlg::SaveProfileApp(CWinApp* pApp)
{
	m_bChange = FALSE;
}

BOOL CProfileableDlg::Changed()
{
	return m_bChange;
}


BOOL CProfileableDlg::PreTranslateMessage(MSG* pMsg)
{
//主な処理はダイアログでリターンキーを押すとダイアログが終了するので、これを別の処理に置き換える。
	BOOL b;
	SHORT key;
	CWnd* pWnd;
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_UP:
			//ctl+↑は親ウィンドウにフォーカス移動
			key = GetKeyState(VK_CONTROL);
			if (key < 0)
			{
				pWnd = GetParent();
				pWnd->SetFocus();
				return TRUE;
			}

		case VK_TAB:
			break;
		case VK_RETURN:
			//フォーカスのあるウィンドウをクリックしたことにする。
			pWnd = GetFocus();
			pWnd->PostMessage(WM_LBUTTONDOWN, 0, 0);
			return TRUE;
		case VK_ESCAPE:
//親ウィンドウにフォーカスする。
			pWnd = GetParent();
			pWnd->SetFocus();
			return TRUE;
		default:
			break;
		}

	case WM_KEYUP:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			//フォーカスのあるウィンドウをクリックしたことにする。
			pWnd = GetFocus();
			pWnd->PostMessage(WM_LBUTTONUP, 0, 0);
			return TRUE;
		default:
			break;
		}

	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
