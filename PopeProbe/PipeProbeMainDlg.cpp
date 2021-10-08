
// MainDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "PopeProbeApp.h"
#include "PipeProbeMainDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg ダイアログ
class CAboutDlg :
	public CDialogEx
{
public:
	CAboutDlg();

	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CPipeProbeMainDlg ダイアログ



CPipeProbeMainDlg::CPipeProbeMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POPEPROBE_DIALOG, pParent)
	, m_strEditMain(_T(""))
	, m_strInfomation(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	ZeroMemory(&m_BufDown,BUF_SIZE);
	ZeroMemory(&m_BufUp, BUF_SIZE);
	m_hChild_Down_Wr=NULL;
	m_hChild_Up_Rd=NULL;
}

CPipeProbeMainDlg::~CPipeProbeMainDlg()
{
	CDialogEx::~CDialogEx();
}

void CPipeProbeMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAIN, m_strEditMain);
	DDX_Control(pDX, IDC_EDIT_MAIN, m_ctlEditMain);
	DDX_Text(pDX, IDC_STATIC_INFOMATION, m_strInfomation);
}

BEGIN_MESSAGE_MAP(CPipeProbeMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_READ_DOWN, &CPipeProbeMainDlg::OnUserReadDown)
	ON_MESSAGE(WM_USER_READ_UP, &CPipeProbeMainDlg::OnUserReadUp)
	ON_MESSAGE(WM_USER_EXE_MESSAGE, &CPipeProbeMainDlg::OnUserExeMessage)
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_PIPE_DISCONNECT, &CPipeProbeMainDlg::OnBnClickedDisconnectPipe)
	ON_MESSAGE(WM_TASKTRAY, &CPipeProbeMainDlg::OnTasktray)
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDC_BUTTON_FORCED_TERMINATION, &CPipeProbeMainDlg::OnBnClickedButtonForcedTermination)
	ON_BN_CLICKED(IDC_BUTTON_GO_TASKTRAY, &CPipeProbeMainDlg::OnBnClickedButtonGoTasktray)
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(IDM_ABOUTBOX, &CPipeProbeMainDlg::OnAboutbox)
END_MESSAGE_MAP()


// CPipeProbeMainDlg メッセージ ハンドラー

BOOL CPipeProbeMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	LoadProfile();
	if (m_bINI_Existing == FALSE)
	{
		AfxMessageBox(_T("初期設定がされていません。\r\nPipeProbeSetting.exeで初期設定を行ってください。"), MB_OK | MB_ICONEXCLAMATION);
		ExitProcess(0);
	}
	//記録したウィンドウ座標が適切か調べる。
	if (!m_bFirst_Appear)
	{
		HMONITOR hMonitor = NULL;
		MONITORINFO MonitorInfo{ 0 };
		MonitorInfo.cbSize = sizeof(MONITORINFO);
		hMonitor = MonitorFromRect(&m_rectMainWnd, MONITOR_DEFAULTTOPRIMARY);
		BOOL rVal;
		GetMonitorInfo(hMonitor, &MonitorInfo);
		//記録したウィンドウ座標がモニター外ならデフォルトの位置に表示させる。
		rVal = PtInRect(&MonitorInfo.rcWork, m_rectMainWnd.TopLeft());
		if (rVal)
		{
			SetWindowPos(NULL, m_rectMainWnd.left, m_rectMainWnd.top, m_rectMainWnd.right - m_rectMainWnd.left, m_rectMainWnd.bottom - m_rectMainWnd.top, SWP_NOZORDER);
		}
	}


	Execute();

	TrayExecute(NIM_ADD);
	//ウインドウを表示状態をセットする。
	switch (m_ProfileGenelal.m_iMainWindowShowStatus)
	{
	case 0:
		MainWndShow();
		break;
	case 1:
		MainWndIcon();
		break;
	case 2:
		MainWndHide();
		break;
	default:
		break;
	}
	CWinApp* pApp = AfxGetApp();
	PipeProbeCommandOption CCmdOp;
	pApp->ParseCommandLine(CCmdOp);
	if (CCmdOp.m_bSW_SHOWMINIMIZED == TRUE)
	{
		ShowWindow(SW_SHOWMINIMIZED);
		UpdateWindow();
	}



	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CPipeProbeMainDlg::OnPaint()
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
HCURSOR CPipeProbeMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//INIファイルからの設定ロード
void CPipeProbeMainDlg::LoadProfile()
{
	CWinApp* pApp = AfxGetApp();
	CString strSec(_T("SYSTEM"));
	m_bINI_Existing = pApp->GetProfileInt(strSec, _T("INI_EXISTING"), FALSE);
	m_bFirst_Appear = pApp->GetProfileInt(strSec, _T("FIRST_APPEAR"), TRUE);
	m_rectMainWnd.left = pApp->GetProfileInt(strSec, _T("WINDOW_POS_MAIN_LEFT"), NULL);
	m_rectMainWnd.top = pApp->GetProfileInt(strSec, _T("WINDOW_POS_MAIN_TOP"), NULL);
	m_rectMainWnd.right = pApp->GetProfileInt(strSec, _T("WINDOW_POS_MAIN_RIGHT"), NULL);
	m_rectMainWnd.bottom = pApp->GetProfileInt(strSec, _T("WINDOW_POS_MAIN_BOTTOM"), NULL);

	m_ProfileGenelal.LoadProfile();
	m_ProfileChild.LoadProfile();
	m_ProfileLogView.LoadProfile();
	m_ProfileLogFile.LoadProfile();

}

//INIファイルに設定保存
void CPipeProbeMainDlg::SaveProfile()
{
	CWinApp* pApp = AfxGetApp();
	RECT rect;
	CString strSystem(_T("SYSTEM"));
	pApp->WriteProfileInt(strSystem, _T("INI_EXISTING"), m_bINI_Existing);
	//ウィンドウ位置を保存
	GetWindowRect(&rect);
	pApp->WriteProfileInt(strSystem, _T("WINDOW_POS_MAIN_LEFT"), rect.left);
	pApp->WriteProfileInt(strSystem, _T("WINDOW_POS_MAIN_TOP"), rect.top);
	pApp->WriteProfileInt(strSystem, _T("WINDOW_POS_MAIN_RIGHT"), rect.right);
	pApp->WriteProfileInt(strSystem, _T("WINDOW_POS_MAIN_BOTTOM"), rect.bottom);
	pApp->WriteProfileInt(strSystem, _T("FIRST_APPEAR"), FALSE);

}



void CPipeProbeMainDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	SaveProfile();
	TrayExecute(NIM_DELETE);
}

//子プロセス起動処理
void CPipeProbeMainDlg::Execute()
{
	//パイプ作成
	CWinApp* pApp = AfxGetApp();
	SECURITY_ATTRIBUTES saAttr = {};

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;
	HANDLE  hChild_Up_Wr, hChild_Down_Rd;

	if (!CreatePipe(&m_hChild_Up_Rd, &hChild_Up_Wr, &saAttr, 0))
		return;
	if (!SetHandleInformation(m_hChild_Up_Rd, HANDLE_FLAG_INHERIT, 0))
		return;

	if (!CreatePipe(&hChild_Down_Rd, &m_hChild_Down_Wr, &saAttr, 0))
		return;
	if (!SetHandleInformation(m_hChild_Down_Wr, HANDLE_FLAG_INHERIT, 0))
		return;

	//コマンドライン解析　/Debugが指定されていたらコンソール立ち上げ
	PipeProbeCommandOption CCmdOp;
	pApp->ParseCommandLine(CCmdOp);
	BOOL brval;
	if (CCmdOp.m_bDebug == TRUE)
	{
		AllocConsole();
		m_hConsole = GetConsoleWindow();
		brval = ::DeleteMenu(::GetSystemMenu(m_hConsole, FALSE), SC_CLOSE, MF_BYCOMMAND);
	}

	//子プロセス起動	
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CString strCommandLine = m_ProfileChild.CreateCommandLine(m_ProfileGenelal.m_strChildPathName);
	m_ExeEngine.Execute(strCommandLine, hChild_Down_Rd, hChild_Up_Wr, hChild_Up_Wr, STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW, this, WM_USER_EXE_MESSAGE);

	//パイプ接続
	m_ProbeDown.Connect(hIn, m_hChild_Down_Wr, m_BufDown, BUF_SIZE, ID_PROBE_DOWN, WM_USER_READ_DOWN, this);
	m_ProbeUp.Connect(m_hChild_Up_Rd, hOut, m_BufUp, BUF_SIZE, ID_PROBE_UP, WM_USER_READ_UP, this);
	CloseHandle(hChild_Down_Rd);
	CloseHandle(hChild_Up_Wr);

	//ログファイル作成
	CString strPathName;
	if (m_ProfileLogFile.m_bMakeLogFile == TRUE)
	{
		strPathName = m_ProfileLogFile.m_strLogDir;
		strPathName += _T("\\") + m_ProfileLogFile.MakeFilename(m_ProfileLogFile.m_strNameFormat);
		m_hFile = CreateFile(strPathName.GetBuffer(), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			_T("\r\nログファイル作成エラー\r\n");
			m_hFile = NULL;
		}
		strPathName.ReleaseBuffer();

		//子プロセスの起動コマンドラインをログに書き込むかどうか。
		if (m_ProfileLogFile.m_bWriteCommand == TRUE)
		{
			CString strCommand = CString(_T("Child process execute command:\"")) + strCommandLine.GetBuffer() + _T("\"\r\n");
			strCommandLine.ReleaseBuffer();
			DWORD numWrite, numWriten = 0;
			numWrite = strCommand.GetLength();
			if (!WriteFile(m_hFile, strCommand.GetBuffer(), numWrite, &numWriten, NULL))
			{
				_T("\r\n書き込みエラー\r\n");
			}
			strCommand.ReleaseBuffer();
		}

	}

}

//ProbeよりWindowメッセージとして、読み込みの通知がされる。
afx_msg LRESULT CPipeProbeMainDlg::OnUserReadDown(WPARAM wParam, LPARAM lParam)
{
	USES_CONVERSION;
	//0以外エラー発生
	if (wParam != 0)
	{
		TRACE(_T("\r\nOnUserReadDown---Error\r\n"));
	}
	//-1は切断
	if (-1 == lParam)
	{
		TRACE(_T("\r\nOnUserReadDown切断されました。\r\n"));
	}

	//GetStringが呼ばれた後、ProbeはWrite処理を開始する。
	CString strView = m_ProbeDown.GetString();
	//ファイル書き込みは８ビット文字列に変換
	CStringA strFile = CStringA(strView);

	//表示の為の処理
	UpdateData(TRUE);
	CString strSearch = m_Regex.TranslateEscape(m_ProfileLogView.m_strSearchDown);
	CString strReplace = m_Regex.TranslateEscape(m_ProfileLogView.m_strReplaceDown);
	strView = m_Regex.ExecuteRegex(strView, strSearch, strReplace);
	int nLine = 0, nLineOver = 0, len;
	m_strEditMain += strView;
	UpdateData(FALSE);
	nLine = m_ctlEditMain.GetLineCount();
	nLineOver = nLine - m_ProfileLogView.m_iMaxLine - 1;
	int	TotalLength = 0;
	while (nLineOver > 0)
	{
		len = m_strEditMain.Find(_T("\n"), TotalLength);
		if (len == -1)
		{
			break;
		}
		TotalLength = len + 1;
		nLineOver--;
	}
	m_strEditMain.Delete(0, TotalLength);
	UpdateData(FALSE);

	//ログファイルの為の書き込み処理
	if (m_ProfileLogFile.m_bMakeLogFile == TRUE)
	{
		strFile = m_Regex.ExecuteRegex(CString(strFile), m_ProfileLogFile.m_strSearchDown, m_ProfileLogFile.m_strReplaceDown);
		DWORD numWrite=0,numWriten=0;
		numWrite = strFile.GetLength();
		if (!WriteFile(m_hFile, strFile.GetBuffer(), numWrite, &numWriten, NULL))
		{
			_T("\r\n書き込みエラー\r\n");
		}
	}
	return 0;
}

//ProbeよりWindowメッセージとして、読み込みの通知がされる。
afx_msg LRESULT CPipeProbeMainDlg::OnUserReadUp(WPARAM wParam, LPARAM lParam)
{
	USES_CONVERSION;
	//0以外エラー
	if (wParam != 0)
	{
		TRACE(_T("\r\nOnUserReadUp---Error\r\n"));
	}
	//-1は切断
	if (-1 == lParam)
	{
		TRACE(_T("\r\nOnUserReadUp切断されました。\r\n"));
		return 0;
	}
	//GetStringが呼ばれた後、ProbeはWrite処理を開始する。
	CString strView = m_ProbeUp.GetString();
	//ファイル書き込みは８ビット文字列に変換
	CStringA strFile = CStringA(strView);

	//表示の為の処理
	UpdateData(TRUE);
	CString strSearch = m_Regex.TranslateEscape(m_ProfileLogView.m_strSearchUp);
	CString strReplace = m_Regex.TranslateEscape(m_ProfileLogView.m_strPeplaceUp);
	strView = m_Regex.ExecuteRegex(strView, strSearch, strReplace);
	int nLine = 0, nLineOver = 0,len;
	m_strEditMain += strView;
	UpdateData(FALSE);
	nLine = m_ctlEditMain.GetLineCount();
	nLineOver = nLine - m_ProfileLogView.m_iMaxLine - 1;
	int	TotalLength = 0;
	while (nLineOver > 0)
	{
		len = m_strEditMain.Find(_T("\n"), TotalLength);
		if (len == -1)
		{
			break;
		}
		TotalLength = len + 1;
		nLineOver--;
	}
	m_strEditMain.Delete(0, TotalLength);
	TotalLength=m_strEditMain.GetLength();
	UpdateData(FALSE);
	m_ctlEditMain.SetSel(TotalLength, TotalLength);

	//ログファイルの為の書き込み処理
	if (m_ProfileLogFile.m_bMakeLogFile == TRUE)
	{
		strFile = m_Regex.ExecuteRegex(CString(strFile), m_ProfileLogFile.m_strSearchUp, m_ProfileLogFile.m_strReplaceUp);
		DWORD numWrite = 0, numWriten = 0;
		numWrite = strFile.GetLength();
		if (!WriteFile(m_hFile, strFile, numWrite, &numWriten, NULL))
		{
			_T("\r\n書き込みエラー\r\n");
		}
	}
	return 0;
}

//子プロセスが起動または、終了した時にExeEngineから通知されるユーザーメッセージハンドラ
afx_msg LRESULT CPipeProbeMainDlg::OnUserExeMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case EXE_ENGINE_START:
		TRACE(_T("\r\nOnUserExeMessage子プロセス起動\r\n"));
		m_strInfomation = _T("子プロセスが起動しました。");
		UpdateData(FALSE);
		break;
	case EXE_ENGINE_END:
		TRACE(_T("\r\nOnUserExeMessage子プロセス終了。\r\n"));
		FreeConsole();
		m_ProbeDown.DisConnect();
		m_ProbeUp.DisConnect();
		CloseHandle(m_hChild_Down_Wr);
		CloseHandle(m_hChild_Up_Rd);
		m_hChild_Down_Wr = NULL;
		m_hChild_Up_Rd = NULL;
		CloseHandle(m_hFile);
		m_strInfomation = _T("子プロセスが終了しました。");
		UpdateData(FALSE);
		PostMessage(WM_CLOSE);
		return 0;
	default:
		break;
	}
	return 0;
}


//主にリターン、エスケープを押したときに、ダイアログが閉じてしまう動作に対応
BOOL CPipeProbeMainDlg::PreTranslateMessage(MSG* pMsg)
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
			pWnd->PostMessage(WM_LBUTTONDOWN);
			return TRUE;
		case VK_ESCAPE:
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
			pWnd->PostMessage(WM_LBUTTONUP);
			return TRUE;
		default:
			break;
		}
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//パイプ切断して、終了させる。
void CPipeProbeMainDlg::OnBnClickedDisconnectPipe()
{
	m_ProbeDown.DisConnect();
	m_ProbeUp.DisConnect();
	CloseHandle(m_hChild_Down_Wr);
	m_hChild_Down_Wr = NULL;
	CloseHandle(m_hChild_Up_Rd);
	m_hChild_Up_Rd = NULL;

}

//タスクトレイ作成
BOOL CPipeProbeMainDlg::TrayExecute(DWORD dwCommand)
{
	CString sTip = _T("PipeProbe.exe");
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


	//タスクトレイのハンドラ
afx_msg LRESULT CPipeProbeMainDlg::OnTasktray(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_LBUTTONDOWN:
		if (wParam == IDR_MAINFRAME) {    // アイコンID
			if (m_b_is_hide)
			{
				MainWndShow();
			}
			else if (IsIconic())
			{
				MainWndShow();
			}
			else
			{
				MainWndHide();
			}
		}
		break;
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
	default:
		break;
	}
	return 0;
}

void CPipeProbeMainDlg::MainWndShow()
{
	m_b_is_hide = false;
	ShowWindow(SW_NORMAL);
	if (m_hConsole != 0)
	{
		::ShowWindow(m_hConsole, SW_NORMAL);
	}
	SetForegroundWindow();
	SetFocus();
}


	void CPipeProbeMainDlg::MainWndIcon()
	{
		m_b_is_hide = false;
		ShowWindow(SW_MINIMIZE);
		if (m_hConsole != 0)
		{
			::ShowWindow(m_hConsole, SW_MINIMIZE);
		}
	}


	void CPipeProbeMainDlg::MainWndHide()
	{
		m_b_is_hide = TRUE;
		ShowWindow(SW_HIDE);
		if (m_hConsole != 0)
		{
			::ShowWindow(m_hConsole, SW_HIDE);
		}
	}

//起動時にダイアログを非表示にする為のルーチン
	void CPipeProbeMainDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
	{
		CDialogEx::OnWindowPosChanging(lpwndpos);
		if (m_b_is_hide)
			lpwndpos->flags &= ~SWP_SHOWWINDOW;
	}

//強制終了の為のルーチン
	void CPipeProbeMainDlg::OnBnClickedButtonForcedTermination()
	{
		BOOL res = AfxMessageBox(_T("子プロセスを強制終了します。\r\nよろしいですか？"), MB_ICONEXCLAMATION | MB_YESNO);
		if (res == MB_OK)
		{
			m_ExeEngine.ForcedTermination();
		}
	}


	void CPipeProbeMainDlg::OnBnClickedButtonGoTasktray()
	{
		MainWndHide();
	}

//タイトルバーのクロースボタンを押した時の対応
	void CPipeProbeMainDlg::OnClose()
	{
//pipeハンドルが生きている場合、切断する。		
		if (m_hChild_Up_Rd != NULL)
		{
			OnBnClickedDisconnectPipe();
			return;
		}
		CDialogEx::OnClose();
	}



	void CPipeProbeMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
	{
		if ((nID & 0xFFF0) == IDM_ABOUTBOX)
		{
			CAboutDlg dlgAbout;
			dlgAbout.DoModal();
		}
		else
		{
			CDialogEx::OnSysCommand(nID, lParam);
		}
	}


	void CPipeProbeMainDlg::OnAboutbox()
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
