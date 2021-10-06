
// WCmdCliantDlgMain.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "WCmdCliant.h"
#include "WCmdCliantDlgMain.h"
#include "afxdialogex.h"
#include "WCmdCLiantOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWCmdCliantMainDlg ダイアログ



CWCmdCliantMainDlg::CWCmdCliantMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WCMDCLIANT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWCmdCliantMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWCmdCliantMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	ON_MESSAGE(WM_THREAD_END, &CWCmdCliantMainDlg::OnThreadEnd)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_TASKTRAY, &CWCmdCliantMainDlg::OnTasktray)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CWCmdCliantMainDlg メッセージ ハンドラー

BOOL CWCmdCliantMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	//コマンドライン解析
	CWinApp* pApp = AfxGetApp();
	pApp->ParseCommandLine(m_CCOp);
	if (m_CCOp.m_bDebug)
	{
		AllocConsole();
		m_hConsole = GetConsoleWindow();
		BOOL brval = ::DeleteMenu(::GetSystemMenu(m_hConsole, FALSE), SC_CLOSE, MF_BYCOMMAND);
	}
	//パイプハンドルを取得
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hIn == NULL || hOut == NULL)
	{
		if (m_CCOp.m_bDebug)
		{
			FreeConsole();
		}
		ExitProcess(5);
	}
	//トレイ表示
	TrayExecute(NIM_ADD);
	//メインウィンドウは隠す
	MainWndHide();

//読み込み、書き込みは別スレッドで行う。
	m_TParam = {};
	m_TParam.pWnd = this;
	m_TParam.hInput = hIn;
	m_TParam.hOutput = hOut;
	m_TParam.hEvent = NULL;
	m_TParam.WM_MESSAGE = WM_THREAD_END;
	m_TParam.ID_THREAD = 0;
	m_TParam.pBuf = m_Buf;
	m_TParam.Bufsize = BUFSIZE;
	CWinThread* pThread = AfxBeginThread(SubThreadProc, &m_TParam);
	m_TParam.hThread = pThread->m_hThread;

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

UINT CWCmdCliantMainDlg::SubThreadProc(LPVOID pParam)
{
	TRACE("\r\nWCmdPipeCliant.exe--SubThreadProc------Thread start---------------\r\n");
	USES_CONVERSION;
	THREAD_PARAM_WCOMMAND_CLIANT *pTParam = (THREAD_PARAM_WCOMMAND_CLIANT*)pParam;
	DWORD dwVal;
	int i = 0;
	pTParam->bContinue = TRUE;
	while (pTParam->bContinue)
	{
		ZeroMemory(pTParam->pBuf, pTParam->Bufsize);
		pTParam->bSuccess = ReadFile(pTParam->hInput, pTParam->pBuf, BUFSIZE, &pTParam->dwRead, NULL);
		if (!pTParam->bSuccess || pTParam->dwRead == 0)
		{
			pTParam->bContinue = FALSE;
			dwVal = GetLastError();
			switch (dwVal)
			{
			case ERROR_HANDLE_EOF:
				i = dwVal;
				TRACE("\r\nWCmdPipeCliant.exe--SubThreadProc------ERROR_HANDLE_EOF---------------\r\n");
				break;
			case ERROR_BROKEN_PIPE:
				i = dwVal;
				TRACE("\r\nWCmdPipeCliant.exe--SubThreadProc------ERROR_BROKEN_PIPE--------------\r\n");
				break;
			default:
				break;
			}
			break;
		}
		TRACE("\r\nWCmdPipeCliant.exe--SubThreadProc------Received Message is\r\n%s-----------------------------\r\n", pTParam->pBuf);
		int rVal = 0;
		rVal = _tcsicmp(A2T(pTParam->pBuf), _T("quit\r\n"));
		if (!rVal)
		{
			TRACE("\r\nWCmdPipeCliant.exe--SubThreadProc--------QUIT HAS BEEN ENTERED--------------\r\n");

			pTParam->bContinue = FALSE;
		}

		pTParam->bSuccess = WriteFile(pTParam->hOutput, pTParam->pBuf, pTParam->dwRead, &pTParam->dwWritten, NULL);
		if (!pTParam->bSuccess /* || (dwRead != dwWritten) */)
		{
			pTParam->bContinue = FALSE;
			dwVal = GetLastError();
			switch (dwVal)
			{
			case ERROR_HANDLE_EOF:
				i = dwVal;
				break;
			case ERROR_BROKEN_PIPE:
				i = dwVal;
				break;
			default:
				break;
			}
			break;
		}

	}
	TRACE("\r\nWCmdPipeCliant.exe--SubThreadProc--ID=%d --END-------------------------------\r\n",pTParam->ID_THREAD);
	pTParam->pWnd->PostMessage(pTParam->WM_MESSAGE);
	AfxEndThread(123, TRUE);
	return 0;
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CWCmdCliantMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CWCmdCliantMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWCmdCliantMainDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanging(lpwndpos);
	if (m_b_is_hide)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
}


void CWCmdCliantMainDlg::MainWndShow()
{
	m_b_is_hide = false;
	ShowWindow(SW_NORMAL);
	SetForegroundWindow();
	SetFocus();
}


void CWCmdCliantMainDlg::MainWndIcon()
{
	m_b_is_hide = false;
	ShowWindow(SW_MINIMIZE);
}


void CWCmdCliantMainDlg::MainWndHide()
{
	m_b_is_hide = TRUE;
	ShowWindow(SW_HIDE);
}


afx_msg LRESULT CWCmdCliantMainDlg::OnThreadEnd(WPARAM wParam, LPARAM lParam)
{
	OnOK();
	return 0;
}


void CWCmdCliantMainDlg::OnClose()
{
	CDialogEx::OnClose();
}


BOOL CWCmdCliantMainDlg::PreTranslateMessage(MSG* pMsg)
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
			//フォーカスのあるウィンドウをクリックしたことにする。
			pWnd = GetFocus();
			pWnd->PostMessage(WM_LBUTTONDOWN, 0, MAKELONG(1, 1));
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		case VK_DOWN:
			pWnd = NULL;
			break;

		default:
			break;
		}
	case WM_KEYUP:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			//フォーカスのあるウィンドウをクリックしたことにする。
			pWnd = GetFocus();
			pWnd->PostMessage(WM_LBUTTONUP, 0, MAKELONG(1, 1));
			return TRUE;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void CWCmdCliantMainDlg::DisConnect()
{
	m_TParam.bContinue = FALSE;
	CancelIoEx(m_TParam.hInput, NULL);
	CancelIoEx(m_TParam.hOutput, NULL);
}


void CWCmdCliantMainDlg::OnDestroy()
{
	if (m_CCOp.m_bDebug == TRUE)
	{
		FreeConsole();
	}
	DisConnect();
	WaitForSingleObject(m_TParam.hThread, INFINITE);
	TrayExecute(NIM_DELETE);
	CDialogEx::OnDestroy();

}


BOOL CWCmdCliantMainDlg::TrayExecute(DWORD dwCommand)
{
	CString sTip = _T("WCmdCliant.exe");
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = GetSafeHwnd();
	nid.uID = IDR_MAINFRAME;        // <-- アイコンIDを指定してます
	nid.uCallbackMessage = WM_TASKTRAY;    // <-- WM_USER + 1 とかでメッセージ定義
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	_tcscpy_s(nid.szTip, _countof(nid.szTip), (LPCTSTR)sTip);

	return Shell_NotifyIcon(dwCommand, &nid);
}


afx_msg LRESULT CWCmdCliantMainDlg::OnTasktray(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
		//if (wParam == IDR_MAINFRAME) {    // アイコンID
		//	if (m_b_is_hide)
		//	{
		//		MainWndShow();
		//	}
		//	else if (IsIconic())
		//	{
		//		MainWndShow();
		//	}
		//	else
		//	{
		//		MainWndHide();
		//	}

		//}
		//break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if (wParam == IDR_MAINFRAME) {

			CMenu menu;
			CMenu Submenu;
			menu.LoadMenu(IDR_MENU_TASK_MENU);
			Submenu.Attach(menu.GetSubMenu(0)->GetSafeHmenu());
			CMenu* pPopup = &Submenu;
			SetForegroundWindow();
			POINT pt;
			GetCursorPos(&pt);
			pPopup->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
			pPopup->Detach();
			menu.DestroyMenu();
		}
		break;
	}
	return 0;
}


BOOL CWCmdCliantMainDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_TASK_MENU_CLOSE)
	{
		PostMessage(WM_CLOSE);
	}

	return CDialogEx::OnCommand(wParam, lParam);
}

