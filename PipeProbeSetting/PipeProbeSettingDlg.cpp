
// PipeProbeSettingDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "PipeProbeSettingApp.h"
#include "PipeProbeSettingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProbeSettingMainDlg ダイアログ



CProbeSettingMainDlg::CProbeSettingMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PIPEPROBE_SETTING_MAIN_DIALOG, pParent)
	, m_strSettingFilePathName(_T(""))
	, m_strExePathName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iCurSel = 0;
}

void CProbeSettingMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_ctlMainTab);
	DDX_Text(pDX, IDC_STATIC_SETTING_FILE_PATHNAME, m_strSettingFilePathName);

	DDX_Control(pDX, IDC_BUTTON_SAVE_SETTING_FILE, m_ctlButtonSaveSetting);
	if (m_strSettingFilePathName == _T(""))
	{
		m_ctlButtonSaveSetting.EnableWindow(FALSE);
	}
	else
	{
		m_ctlButtonSaveSetting.EnableWindow(TRUE);
	}
	DDX_Text(pDX, IDC_STATIC_EXE_PATHNAME, m_strExePathName);
}

BEGIN_MESSAGE_MAP(CProbeSettingMainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PROFILE_AS, &CProbeSettingMainDlg::OnBnClickedButtonSaveProfileAs)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_SETTING_FILE, &CProbeSettingMainDlg::OnBnClickedButtonSaveOverWrite)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_EXISTING_SETTING_FILE, &CProbeSettingMainDlg::OnClickedButtonLoadExistingSettingFile)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_PARENTNOTIFY()
	ON_BN_CLICKED(IDC_BUTTON_TEST_EXECUTE, &CProbeSettingMainDlg::OnBnClickedButtonTestExecute)
	ON_BN_CLICKED(IDC_BUTTON_INSTALL_EXE, &CProbeSettingMainDlg::OnBnClickedButtonInstallExe)
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CProbeSettingMainDlg メッセージ ハンドラー

BOOL CProbeSettingMainDlg::OnInitDialog()
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

//	SetWindowLong(hWnd, GWL_EXSTYLE, 0);
	::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, WS_EX_COMPOSITED);
//子ダイアログの作成及び、タブコントロールへの挿入
	m_PropGenelal.Create(IDD_PROPPAGE_GENELAL, &m_ctlMainTab);
	m_PropLogSetting.Create(IDD_PROPPAGE_LOG_FILE, &m_ctlMainTab);
	m_PropChild.Create(IDD_PROPPAGE_CHILD, &m_ctlMainTab);
	m_PropLogView.Create(IDD_PROPPAGE_LOG_VIEW, &m_ctlMainTab);
	m_PropRegexTest.Create(IDD_PROPPAGE_REGEX_TEST, &m_ctlMainTab);
	m_PropStrftimeDescription.Create(IDD_PROPPAGE_STRFTIME_DESCRIPTION, &m_ctlMainTab);
	CString str;
	str = _T("総合");
	m_ctlMainTab.InsertDlg(0,&m_PropGenelal,&str);
	str = _T("子プロセス");
	m_ctlMainTab.InsertDlg(1, &m_PropChild, &str);
	str = _T("ログ表示");
	m_ctlMainTab.InsertDlg(2, &m_PropLogView, &str);
	str = _T("ログファイル");
	m_ctlMainTab.InsertDlg(3, &m_PropLogSetting,&str);
	str = _T("正規表現テスト");
	m_ctlMainTab.InsertDlg(4, &m_PropRegexTest, &str);
	str = _T("strftime説明");
	m_ctlMainTab.InsertDlg(5, &m_PropStrftimeDescription, &str);

	m_ctlMainTab.SetCurSel(m_iCurSel);
	LoadProfileApp();

	UpdateData(FALSE);
	
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CProbeSettingMainDlg::OnPaint()
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
HCURSOR CProbeSettingMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProbeSettingMainDlg::LoadProfile()
{
	CWinApp* pApp = AfxGetApp();
	CRect rect;
	rect.left = pApp->GetProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_LEFT"), -1);
	rect.top = pApp->GetProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_TOP"), -1);
	rect.right = pApp->GetProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_RIGHT"), -1);
	rect.bottom = pApp->GetProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_BOTTOM"), -1);
	if (!(rect.left == rect.right))
	{
		HMONITOR hMonitor = NULL;
		MONITORINFO MonitorInfo{ 0 };
		MonitorInfo.cbSize = sizeof(MONITORINFO);
		hMonitor = MonitorFromRect(&rect, MONITOR_DEFAULTTOPRIMARY);
		BOOL rVal;
		GetMonitorInfo(hMonitor, &MonitorInfo);
		rVal = PtInRect(&MonitorInfo.rcWork, rect.TopLeft());
		if (rVal)
		{
			SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
		}
	}
	m_strSettingFilePathName = pApp->GetProfileString(_T("APP"), _T("SETTING_FILE_PATHNAME"), _T(""));
	m_iCurSel = pApp->GetProfileInt(_T("SYSTEM"), _T("CURRENT_CEL"), 0);
	m_strExePathName = pApp->GetProfileString(_T("APP"), _T("EXE_FILE_PATHNAME"), _T(""));
}


void CProbeSettingMainDlg::SaveProfile()
{
	CWinApp* pApp = AfxGetApp();
	RECT rect;
	GetWindowRect(&rect);
	m_iCurSel = m_ctlMainTab.GetCurSel();
	pApp->WriteProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_LEFT"), rect.left);
	pApp->WriteProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_TOP"), rect.top);
	pApp->WriteProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_RIGHT"), rect.right);
	pApp->WriteProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_BOTTOM"), rect.bottom);
	pApp->WriteProfileString(_T("APP"), _T("SETTING_FILE_PATHNAME"), m_strSettingFilePathName);
	pApp->WriteProfileInt(_T("SYSTEM"), _T("CURRENT_CEL"), m_iCurSel);
	pApp->WriteProfileString(_T("APP"), _T("EXE_FILE_PATHNAME"), m_strExePathName);

}


void CProbeSettingMainDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	SaveProfile();
}



void CProbeSettingMainDlg::SaveProfileApp()
{
	if (m_strSettingFilePathName == _T(""))
	{
		AfxMessageBox(_T("PipeProbe.exeがインストールされていません。\r\n設定ファイルに保存することが出来ません。"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	m_ctlMainTab.SaveProfilesApp(m_strSettingFilePathName);
}


void CProbeSettingMainDlg::LoadProfileApp()
{
	m_ctlMainTab.LoadProfilesApp(m_strSettingFilePathName);
}

void CProbeSettingMainDlg::OnBnClickedButtonSaveProfileAs()
{
	UpdateData(TRUE);
	CFileDialog* pDlg = new CFileDialog(FALSE, _T("*.ini"), m_strSettingFilePathName, OFN_HIDEREADONLY, _T("設定ファイル（*.ini)|*.ini||"), this);
	if (pDlg->DoModal() == IDOK)
	{
		m_strSettingFilePathName = pDlg->GetPathName(); // return full path and filename
		m_ctlMainTab.SaveProfilesApp(m_strSettingFilePathName);

		UpdateData(FALSE);
	}

	delete  pDlg;
}

void CProbeSettingMainDlg::OnBnClickedButtonSaveOverWrite()
{
	UpdateData(TRUE);
	if (m_strSettingFilePathName == _T(""))
	{
		AfxMessageBox(_T("起動するファイルのパスが設定されていません。"), MB_ICONINFORMATION | MB_OK);
	}
	SaveProfileApp();
	UpdateData(FALSE);
}


void CProbeSettingMainDlg::OnClickedButtonLoadExistingSettingFile()
{
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("*.ini"), _T("PipeProbe.ini"), OFN_HIDEREADONLY, _T("設定ファイル（*.ini)|*.ini||"), this);
	if (pDlg->DoModal() == IDOK)
	{
		m_strSettingFilePathName = pDlg->GetPathName(); // return full path and filename
		LoadProfileApp();
		UpdateData(FALSE);
	}
	
	delete  pDlg;
}


void CProbeSettingMainDlg::OnOK()
{
	BOOL  bChanged = m_ctlMainTab.IsChanged();
	if (bChanged == TRUE)
	{
		int iAnswer = AfxMessageBox(_T("設定が変更されています。保存しますか？"), MB_YESNOCANCEL | MB_ICONSTOP);
		if (iAnswer == IDYES)
		{
			if (m_strSettingFilePathName == _T(""))
			{
				int iAnswer2 = AfxMessageBox(_T("設定ファイルを保存するパスが設定されていません。\r\n保存出来ません。\r\nこのまま終了しますか？"), MB_YESNOCANCEL | MB_ICONSTOP);
				switch (iAnswer2)
				{
				case IDYES:
					break;
				case IDCANCEL:
				case IDNO:
					return;
				}
			}
			else
			{
				SaveProfileApp();
			}
		}
	}
	CDialogEx::OnOK();
}


BOOL CProbeSettingMainDlg::PreTranslateMessage(MSG* pMsg)
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
			pWnd->PostMessage(WM_LBUTTONDOWN, 0, MAKELONG(1,1));
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
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CProbeSettingMainDlg::OnBnClickedButtonTestExecute()
{
	if (m_ctlMainTab.IsChanged() == TRUE)
	{
		int rVal = AfxMessageBox(_T("設定が保存されていません。\r\n保存しないと起動しません。\r\n保存しますか？"), MB_YESNO | MB_ICONASTERISK);
		if (rVal == IDYES)
		{
			OnBnClickedButtonSaveOverWrite();
		}
		else
		{
			return;
		}
	}

	if (m_strExePathName == _T(""))
	{
		int rVal = AfxMessageBox(_T("PipeProbeのインストールが実行されていません。\r\nインストールしてから起動してください。"), MB_OK | MB_ICONASTERISK);
		return;
	}

	UpdateData(TRUE);
	CWinApp* pApp = AfxGetApp();
	SECURITY_ATTRIBUTES saAttr = {};
	BOOL bSuccess = FALSE;
	PROCESS_INFORMATION PI = {};

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = FALSE;
	saAttr.lpSecurityDescriptor = NULL;

	STARTUPINFO siStartInfo = {};
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = NULL;
	siStartInfo.hStdOutput = NULL;
	siStartInfo.hStdInput = NULL;
	siStartInfo.wShowWindow = SW_SHOWNORMAL;
	siStartInfo.dwFlags |= CREATE_NEW_PROCESS_GROUP| DETACHED_PROCESS;

	CString Command = m_strExePathName + _T(" ") + CString(_T("/PPDebug"));
	bSuccess = CreateProcess(NULL,
		Command.GetBuffer(),     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&PI);  // receives PROCESS_INFORMATION 
	if (bSuccess == FALSE)
	{
		CString str(_T("実行できませんでした。\r\n"));
		ErrDlg(str.GetBuffer());
		str.ReleaseBuffer();
	}
	Command.ReleaseBuffer();
	CloseHandle(PI.hThread);
	CloseHandle(PI.hProcess);
}


void CProbeSettingMainDlg::ErrDlg(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	::MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

//PipeProbe.exeをインストール
void CProbeSettingMainDlg::OnBnClickedButtonInstallExe()
{
	UpdateData(TRUE);
	//ソースパスの存在確認
	if (m_PropGenelal.GetSourcePathName() == _T(""))
	{
		::AfxMessageBox(_T("コピー元が設定されていません。\r\n総合タブからコピー元を設定してください。"), MB_OK|MB_ICONHAND);
		return;
	}
	//子プロセスが設定されているか確認
	if (m_PropGenelal.GetChildPathName() == _T(""))
	{
		::AfxMessageBox(_T("子プロセスの設定がされていません。\r\n総合タブから子プロセスの設定をしてください。"), MB_OK | MB_ICONHAND);
		return;
	}
	
	//EXEパス作成
	TCHAR	szExePathName[_MAX_PATH];
	TCHAR	drive[_MAX_DRIVE];
	TCHAR	dir[_MAX_DIR];
	TCHAR	fname[_MAX_FNAME];
	TCHAR	ext[_MAX_EXT];
	_tsplitpath_s(m_PropGenelal.GetChildPathName(), drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
	_tmakepath_s(szExePathName, _MAX_PATH, drive, dir, _T("PipeProbe"), _T("exe"));
	CString tmpExePathName = szExePathName;

	CFileDialog* pDlg = new CFileDialog(FALSE, _T("*.exe"), tmpExePathName, OFN_HIDEREADONLY, _T("実行ファイル（*.exe)|*.exe|全て(*.*)|*.*||"), this);

	if (pDlg->DoModal() == IDOK)
	{
		//子プロセスと同じファイルになっていないか確認
		if (m_PropGenelal.GetChildPathName() == pDlg->GetPathName())
		{
			::AfxMessageBox(_T("子プロセスと同じファイルです。\r\n再帰呼び出しになるので設定できません。"), MB_OK|MB_ICONEXCLAMATION);
			return;
		}

		//ファイル存在確認。
		if (PathFileExists(pDlg->GetPathName()))
		{
			::AfxMessageBox(_T("既に、ファイルは存在しています。\r\n上書きは出来ません。\r\n既存のファイルを使用します。"), MB_OK|MB_ICONINFORMATION);
		}
		//コピー実施
		else if (!CopyFile(m_PropGenelal.GetSourcePathName(), pDlg->GetPathName(), TRUE))

		{
			CString strError(_T("Copy File"));
			ErrDlg(strError.GetBuffer());
			strError.ReleaseBuffer();
		}
		m_strExePathName = pDlg->GetPathName();

		//INIパス作成
		TCHAR	szIniPathName[_MAX_PATH];
		TCHAR	drive[_MAX_DRIVE];
		TCHAR	dir[_MAX_DIR];
		TCHAR	fname[_MAX_FNAME];
		TCHAR	ext[_MAX_EXT];
		_tsplitpath_s(pDlg->GetPathName(), drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
		_tmakepath_s(szIniPathName, _MAX_PATH, drive, dir, fname, _T("ini"));
		m_strSettingFilePathName = szIniPathName;
		//INI作成
		m_ctlMainTab.SaveProfilesApp(m_strSettingFilePathName);
	}
	delete pDlg;
	UpdateData(FALSE);
}


void CProbeSettingMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
