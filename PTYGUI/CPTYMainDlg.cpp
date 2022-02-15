//Copyright (c) 2021, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php

// CPTYMainDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "PTYGUI.h"
#include "CPTYMainDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
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


// CPTYMainDlg ダイアログ



CPTYMainDlg::CPTYMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PTYGUI_DIALOG, pParent)
	, m_Console()
	, m_bIsVTRunning(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPTYMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_EditCommand);
	DDX_Control(pDX, IDC_RADIO_STD_ACCESSMODE_READ, m_RadioStdAccessModeRead);
	DDX_Control(pDX, IDC_RADIO_P_ACCESSMODE_READ, m_RadioPAccessModeRead);
	DDX_Control(pDX, IDC_RADIO_INTERCEPT_OFF, m_RadioInterceptModeOff);
	DDX_Control(pDX, IDC_RADIO_P_ACCESSMODE_WRITE, m_RadioPAccessModeWrite);
	DDX_Control(pDX, IDC_RADIO_P_ACCESSSMODE_BOTH, m_RadioPAccessModeBoth);
	DDX_Control(pDX, IDC_RADIO_STD_ACCESSMODE_WRITE, m_RadioStdAccessModeWrite);
	DDX_Control(pDX, IDC_RADIO_STD_ACCESSMODE_BOTH, m_RadioStdAccessModeBoth);
	DDX_Control(pDX, IDC_RADIO_INTERCEPT_COVER, m_RadioInterceptModeCover);
	DDX_Control(pDX, IDC_RADIO_INTERCEPT_STEAL, m_RadioInterceptModeSteal);
	DDX_Control(pDX, IDC_RADIO_STD_ACCESSMODE_NO, m_RadioStdAccessModeNo);
	DDX_Control(pDX, IDC_RADIO_P_ACCESSMODE_NO, m_RadioPAccessModeNo);
	DDX_Control(pDX, IDC_STATIC_VT_FILEPATH, m_StaticVTFilePath);
	DDX_Control(pDX, IDC_EDIT_VT_TIMER, m_EditVtTimer);
	DDX_Control(pDX, IDC_EDIT_CONTENTS, m_EditContets);
	DDX_Control(pDX, IDC_EDIT_HEXTEXT, m_EditHexText);
	DDX_Control(pDX, IDC_CHECK_VT_REPEAT, m_CheckVtRepeat);
	DDX_Control(pDX, IDC_EDIT_N_BYTES_READ, m_EditNBytesRead);
	DDX_Control(pDX, IDC_SPIN_VT_TIMER, m_SpinBtTimer);
	DDX_Control(pDX, IDC_SPIN_N_BYTES_READ, m_SpinNBytesRead);
	DDX_Control(pDX, IDC_EDIT_AA, m_EditAA);
}

BEGIN_MESSAGE_MAP(CPTYMainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CPTYMainDlg::OnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CPTYMainDlg::OnClickedButtonClose)
	ON_MESSAGE(WM_USER_P_END, &CPTYMainDlg::OnUserPEnd)
	ON_BN_CLICKED(IDC_RADIO_STD_ACCESSMODE_READ, &CPTYMainDlg::OnClickedRadioStdAccessmodeRead)
	ON_BN_CLICKED(IDC_RADIO_STD_ACCESSMODE_WRITE, &CPTYMainDlg::OnBnClickedRadioStdAccessmodeWrite)
	ON_BN_CLICKED(IDC_RADIO_STD_ACCESSMODE_BOTH, &CPTYMainDlg::OnBnClickedRadioStdAccessmodeBoth)
	ON_BN_CLICKED(IDC_RADIO_P_ACCESSMODE_READ, &CPTYMainDlg::OnBnClickedRadioPAccessmodeRead)
	ON_BN_CLICKED(IDC_RADIO_P_ACCESSMODE_WRITE, &CPTYMainDlg::OnBnClickedRadioPAccessmodeWrite)
	ON_BN_CLICKED(IDC_RADIO_P_ACCESSSMODE_BOTH, &CPTYMainDlg::OnBnClickedRadioPAccesssmodeBoth)
	ON_BN_CLICKED(IDC_RADIO_INTERCEPT_OFF, &CPTYMainDlg::OnBnClickedRadioInterceptOff)
	ON_BN_CLICKED(IDC_RADIO_INTERCEPT_COVER, &CPTYMainDlg::OnBnClickedRadioInterceptCover)
	ON_BN_CLICKED(IDC_RADIO_INTERCEPT_STEAL, &CPTYMainDlg::OnBnClickedRadioInterceptSteal)
	ON_BN_CLICKED(IDC_RADIO_STD_ACCESSMODE_NO, &CPTYMainDlg::OnBnClickedRadioStdAccessmodeNo)
	ON_BN_CLICKED(IDC_RADIO_P_ACCESSMODE_NO, &CPTYMainDlg::OnBnClickedRadioPAccessmodeNo)
	ON_BN_CLICKED(IDC_BUTTON_P_READ, &CPTYMainDlg::OnBnClickedButtonPRead)
	ON_BN_CLICKED(IDC_BUTTON_SDT_READ, &CPTYMainDlg::OnBnClickedButtonSdtRead)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CPTYMainDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SDT_COMMAND, &CPTYMainDlg::OnBnClickedButtonSdtCommand)
	ON_BN_CLICKED(IDC_BUTTON_P_COMMAND, &CPTYMainDlg::OnBnClickedButtonPCommand)
	ON_BN_CLICKED(IDC_BUTTON_VTFILE_OPEN, &CPTYMainDlg::OnBnClickedButtonVtfileOpen)
	ON_BN_CLICKED(IDC_BUTTON_VT_PLAY, &CPTYMainDlg::OnBnClickedButtonVtPlay)
	ON_BN_CLICKED(IDC_BUTTON_VT_STOP, &CPTYMainDlg::OnBnClickedButtonVtStop)
	ON_BN_CLICKED(IDC_BUTTON_P_HEXTEXT, &CPTYMainDlg::OnBnClickedButtonPHextext)
	ON_BN_CLICKED(IDC_BUTTON_SDT_HEXTEXT, &CPTYMainDlg::OnBnClickedButtonSdtHextext)
	ON_BN_CLICKED(IDC_BUTTON_P_CRLF, &CPTYMainDlg::OnBnClickedButtonPCrlf)
	ON_BN_CLICKED(IDC_BUTTON_STD_ESC, &CPTYMainDlg::OnBnClickedButtonStdEsc)
	ON_BN_CLICKED(IDC_BUTTON_VTFILE_CLOSE, &CPTYMainDlg::OnBnClickedButtonVtfileClose)
	ON_EN_CHANGE(IDC_EDIT_VT_TIMER, &CPTYMainDlg::OnEnChangeEditVtTimer)
	ON_EN_CHANGE(IDC_EDIT_N_BYTES_READ, &CPTYMainDlg::OnEnChangeEditNBytesRead)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_VT_REPEAT, &CPTYMainDlg::OnBnClickedCheckVtRepeat)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CPTYMainDlg::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CPTYMainDlg メッセージ ハンドラー

BOOL CPTYMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	USES_CONVERSION;

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
	m_SpinBtTimer.SetRange32(1, 1000);
	m_SpinNBytesRead.SetRange32(1, 1000);
	CBitmap bitmapa;
	TCHAR lpstr[256];
	LoadString((AfxGetApp())->m_hInstance, IDS_STRING_AA, lpstr, 256);
	m_EditAA.SetWindowTextW(lpstr);

	LOGFONT logfont{};
	logfont.lfHeight = 60;		//文字セルまたは文字の高さ
	logfont.lfWeight = 80;		//平均文字幅
	logfont.lfCharSet = DEFAULT_CHARSET;		//キャラクタセット 0=
	wsprintf(logfont.lfFaceName, L"%s", L"ＭＳ ゴシック");//フォント名
	m_FontAA.CreatePointFontIndirect(&logfont);
	m_EditAA.SetFont(&m_FontAA);
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CPTYMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CPTYMainDlg::OnPaint()
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
HCURSOR CPTYMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CPTYMainDlg::PreTranslateMessage(MSG* pMsg)
{
	CWnd* pWnd;
	int DLG_ITEM_ID;
	switch (pMsg->message)
	{
	case WM_KEYDOWN:

		switch (pMsg->wParam)
		{
		case VK_TAB:
			break;
		case VK_RETURN:
			DLG_ITEM_ID = ::GetDlgCtrlID(pMsg->hwnd);
			switch (DLG_ITEM_ID)
			{
			case IDC_EDIT_COMMAND:
				TranslateMessage(pMsg);
				DispatchMessage(pMsg);
				return TRUE;
			case IDC_EDIT_HEXTEXT:
				//				OnClickedButtonPassword();
				return TRUE;
			default:
				break;
			}

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
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPTYMainDlg::LoadProfile()
{
	using namespace CBWinConv;
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
	m_RadioStdAccessModeNo.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("STD_ACC_MODE_NO"), BST_CHECKED));
	if(m_RadioStdAccessModeNo.GetCheck()==BST_CHECKED)
		m_Console.SetStdAccessMode(CBStream::emAccessMode::No);

	m_RadioStdAccessModeRead.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("STD_ACC_MODE_READ"), BST_UNCHECKED));
	if (m_RadioStdAccessModeRead.GetCheck() == BST_CHECKED)
		m_Console.SetStdAccessMode(CBStream::emAccessMode::Read);

	m_RadioStdAccessModeWrite.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("STD_ACC_MODE_WRITE"), BST_UNCHECKED));
	if (m_RadioStdAccessModeWrite.GetCheck() == BST_CHECKED)
		m_Console.SetStdAccessMode(CBStream::emAccessMode::Write);

	m_RadioStdAccessModeBoth.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("STD_ACC_MODE_BOTH"), BST_UNCHECKED));
	if (m_RadioStdAccessModeBoth.GetCheck() == BST_CHECKED)
		m_Console.SetStdAccessMode(CBStream::emAccessMode::Both);

	m_RadioPAccessModeNo.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("P_ACC_MODE_NO"), BST_CHECKED));
	if (m_RadioPAccessModeNo.GetCheck() == BST_CHECKED)
		m_Console.SetAccessMode(CBStream::emAccessMode::No);

	m_RadioPAccessModeRead.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("P_ACC_MODE_READ"), BST_UNCHECKED));
	if (m_RadioPAccessModeRead.GetCheck() == BST_CHECKED)
		m_Console.SetAccessMode(CBStream::emAccessMode::Read);

	m_RadioPAccessModeWrite.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("P_ACC_MODE_WRITE"), BST_UNCHECKED));
	if (m_RadioPAccessModeWrite.GetCheck() == BST_CHECKED)
		m_Console.SetAccessMode(CBStream::emAccessMode::Write);

	m_RadioPAccessModeBoth.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("P_ACC_MODE_BOTH"), BST_UNCHECKED));
	if (m_RadioPAccessModeBoth.GetCheck() == BST_CHECKED)
		m_Console.SetAccessMode(CBStream::emAccessMode::Both);

	m_RadioInterceptModeOff.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("INTERCEPT_MODE_OFF"), BST_CHECKED));
	if (m_RadioInterceptModeOff.GetCheck() == BST_CHECKED)
		m_Console.SetInterceptMode(CPConsole::emInterceptMode::Off);

	m_RadioInterceptModeCover.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("INTERCEPT_MODE_COVER"), BST_UNCHECKED));
	if (m_RadioInterceptModeCover.GetCheck() == BST_CHECKED)
		m_Console.SetInterceptMode(CPConsole::emInterceptMode::Cover);

	m_RadioInterceptModeSteal.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("INTERCEPT_MODE_STEAL"), BST_UNCHECKED));
	if (m_RadioInterceptModeSteal.GetCheck() == BST_CHECKED)
		m_Console.SetInterceptMode(CPConsole::emInterceptMode::Steal);

	CString str;
	str =pApp->GetProfileString(_T("DLGCTL_STATUS"), _T("VT_TIMER"), _T("100"));
	m_EditVtTimer.SetWindowTextW(str);

	CBinaryString bstrw,utfstr;
	str = pApp->GetProfileString(_T("DLGCTL_STATUS"), _T("EDIT_COMMAND"), _T(""));
	bstrw.SetBinaryStringW(str.GetString(), str.GetLength());
	if (bstrw.GetDataSizeW() != 0)
	{
		bstrw.SpaceHexTextToBinaryDataW();
		m_EditCommand.SetWindowTextW(bstrw.c_strw());
	}

	str = pApp->GetProfileString(_T("DLGCTL_STATUS"), _T("HEX_TEXT_COMMAND"), _T(""));
	bstrw.SetBinaryStringW(str.GetString(), str.GetLength());
	if (bstrw.GetDataSizeW() != 0)
	{
		bstrw.SpaceHexTextToBinaryDataW();
		m_EditHexText.SetWindowTextW(bstrw.c_strw());
	}

	m_CheckVtRepeat.SetCheck(pApp->GetProfileInt(_T("DLGCTL_STATUS"), _T("VT_REPEAT"), BST_UNCHECKED));

	str = pApp->GetProfileString(_T("DLGCTL_STATUS"), _T("VT_N_BYTES_READ"), _T("16"));
	m_EditNBytesRead.SetWindowTextW(str);

	str = pApp->GetProfileString(_T("DLGCTL_STATUS"), _T("VT_FILE_PATHNAME"), _T(""));
	bstrw.SetTextStringW(str.GetString());
	bstrw.SpaceHexTextToBinaryDataW();
	m_StaticVTFilePath.SetWindowTextW(bstrw.c_strw());
}


void CPTYMainDlg::SaveProfile()
{
	using namespace CBWinConv;
	CWinApp* pApp = AfxGetApp();
	RECT rect;
	GetWindowRect(&rect);
	pApp->WriteProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_LEFT"), rect.left);
	pApp->WriteProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_TOP"), rect.top);
	pApp->WriteProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_RIGHT"), rect.right);
	pApp->WriteProfileInt(_T("SYSTEM"), _T("WINDOW_POS_MAIN_BOTTOM"), rect.bottom);
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("STD_ACC_MODE_NO"), m_RadioStdAccessModeNo.GetCheck());
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("STD_ACC_MODE_READ"), m_RadioStdAccessModeRead.GetCheck());
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("STD_ACC_MODE_WRITE"), m_RadioStdAccessModeWrite.GetCheck());
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("STD_ACC_MODE_BOTH"), m_RadioStdAccessModeBoth.GetCheck());
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("P_ACC_MODE_NO"), m_RadioPAccessModeNo.GetCheck());
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("P_ACC_MODE_READ"), m_RadioPAccessModeRead.GetCheck());
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("P_ACC_MODE_WRITE"), m_RadioPAccessModeWrite.GetCheck());
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("P_ACC_MODE_BOTH"), m_RadioPAccessModeBoth.GetCheck());
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("INTERCEPT_MODE_OFF"), m_RadioInterceptModeOff.GetCheck());
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("INTERCEPT_MODE_COVER"), m_RadioInterceptModeCover.GetCheck());
	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("INTERCEPT_MODE_STEAL"), m_RadioInterceptModeSteal.GetCheck());
	CString str;
	m_EditVtTimer.GetWindowTextW(str);
	pApp->WriteProfileStringW(_T("DLGCTL_STATUS"), _T("VT_TIMER"), str);

	CBinaryString bstr(64000);
	bstr.SetDataSizeW(m_EditCommand.GetWindowTextW(bstr.GetBufferW(), bstr.GetBufSizeW()));
	bstr.BinaryDataToSpaceHexTextW();
	pApp->WriteProfileStringW(_T("DLGCTL_STATUS"), _T("EDIT_COMMAND"), bstr.c_strw());

	bstr.SetDataSizeW(m_EditHexText.GetWindowTextW(bstr.GetBufferW(), bstr.GetBufSizeW()));
	bstr.BinaryDataToSpaceHexTextW();
	pApp->WriteProfileStringW(_T("DLGCTL_STATUS"), _T("HEX_TEXT_COMMAND"), bstr.c_strw());

	pApp->WriteProfileInt(_T("DLGCTL_STATUS"), _T("VT_REPEAT"), m_CheckVtRepeat.GetCheck());
	
	m_EditNBytesRead.GetWindowTextW(str);
	pApp->WriteProfileStringW(_T("DLGCTL_STATUS"), _T("VT_N_BYTES_READ"), str);

	m_StaticVTFilePath.GetWindowTextW(str);
	bstr.SetTextStringW(str.GetString());
	bstr.BinaryDataToSpaceHexTextW();
	pApp->WriteProfileStringW(_T("DLGCTL_STATUS"), _T("VT_FILE_PATHNAME"), bstr.c_strw());

}


void CPTYMainDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (m_Console.IsConnect())
	{
		m_Console.CloseConsole();
	}
	SaveProfile();
}


void CPTYMainDlg::OnClickedButtonOpen()
{
	USES_CONVERSION;
	if (m_Console.IsConnect())
	{
		return;
	}
	AllocConsole();
	HMENU hmenu;
	hmenu = ::GetSystemMenu(GetConsoleWindow(), FALSE);
	RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);

	CBinaryString bstr;
	bstr.SetTextStringW(L"cmd.exe");
	m_Console.SetCommand(bstr);
	m_Console.SetEventHandler(std::bind(&CPTYMainDlg::OnTerminatePConsole, this));
	m_Console.Connect();
	if (m_Console.GetInterceptMode() == CPConsole::emInterceptMode::Steal)
	{
		CBStream::emAccessMode emAccOld = m_Console.SetAccessMode(CBStream::emAccessMode::Both);
		CBinaryString strEsc("1B 5B 31 3B 31 52");
		strEsc.SpaceHexTextToBinaryDataA();
		m_Console.Write(strEsc);
		m_Console.SetAccessMode(emAccOld);
	}

}


void CPTYMainDlg::OnClickedButtonClose()
{
	if (m_Console.GetInterceptMode() == CPConsole::emInterceptMode::Steal)
	{
		CBStream::emAccessMode emAccOld= m_Console.SetAccessMode(CBStream::emAccessMode::Both);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//"\x1b[1;1Rexit\r"
		CBinaryString strEsc("1B 5B 31 3B 31 52 65 78 69 74 0D");
		strEsc.SpaceHexTextToBinaryDataA();
		m_Console.Write(strEsc);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		m_Console.SetAccessMode(emAccOld);
		m_Console.CloseConsole();
	}
	else
	{
		m_Console.CloseConsole();
	}

	FreeConsole();
}



afx_msg LRESULT CPTYMainDlg::OnUserPEnd(WPARAM wParam, LPARAM lParam)
{
	m_Console.CloseConsole();
	FreeConsole();
	return 0;
}


void CPTYMainDlg::OnTerminatePConsole()
{
	PostMessage(WM_USER_P_END, NULL, NULL);
}


void CPTYMainDlg::OnClickedRadioStdAccessmodeRead()
{
	m_Console.SetStdAccessMode(CBStream::emAccessMode::Read);
}


void CPTYMainDlg::OnBnClickedRadioStdAccessmodeWrite()
{
	m_Console.SetStdAccessMode(CBStream::emAccessMode::Write);
}


void CPTYMainDlg::OnBnClickedRadioStdAccessmodeBoth()
{
	m_Console.SetStdAccessMode(CBStream::emAccessMode::Both);
}


void CPTYMainDlg::OnBnClickedRadioPAccessmodeRead()
{
	m_Console.SetAccessMode(CBStream::emAccessMode::Read);
}


void CPTYMainDlg::OnBnClickedRadioPAccessmodeWrite()
{
	m_Console.SetAccessMode(CBStream::emAccessMode::Write);
}


void CPTYMainDlg::OnBnClickedRadioPAccesssmodeBoth()
{
	m_Console.SetAccessMode(CBStream::emAccessMode::Both);
}


void CPTYMainDlg::OnBnClickedRadioInterceptOff()
{
	m_Console.SetInterceptMode(CPConsole::emInterceptMode::Off);
}


void CPTYMainDlg::OnBnClickedRadioInterceptCover()
{
	m_Console.SetInterceptMode(CPConsole::emInterceptMode::Cover);
}


void CPTYMainDlg::OnBnClickedRadioInterceptSteal()
{
	m_Console.SetInterceptMode(CPConsole::emInterceptMode::Steal);
}


void CPTYMainDlg::OnBnClickedRadioStdAccessmodeNo()
{
	m_Console.SetStdAccessMode(CBStream::emAccessMode::No);
}


void CPTYMainDlg::OnBnClickedRadioPAccessmodeNo()
{
	m_Console.SetAccessMode(CBStream::emAccessMode::No);
}


void CPTYMainDlg::OnBnClickedButtonPRead()
{
	using namespace CBWinConv;
	CBinaryString str;
	CBinaryString wstr;
	m_Console.Read(str);
	Utf8toW(str,wstr);

	m_EditContets.SetWindowTextW(wstr.c_strw());
}


void CPTYMainDlg::OnBnClickedButtonSdtRead()
{
	using namespace CBWinConv;
	CBinaryString str;
	CBinaryString wstr;
	m_Console.StdRead(str);
	Utf8toW(str, wstr);

	m_EditContets.SetWindowTextW(wstr.c_strw());
}


void CPTYMainDlg::OnBnClickedButtonClear()
{
	m_EditContets.SetWindowTextW(L"");
}


void CPTYMainDlg::OnBnClickedButtonSdtCommand()
{
	using namespace CBWinConv;
	CBinaryString wstr;
	CBinaryString str;
	wstr.SetDataSizeW(m_EditCommand.GetWindowTextW(wstr.GetBufferW(),wstr.GetBufSizeW()));
	WtoUtf8(wstr, str);
	m_Console.StdWrite(str);
}


void CPTYMainDlg::OnBnClickedButtonPCommand()
{
	using namespace CBWinConv;
	CBinaryString wstr;
	CBinaryString str;
	wstr.SetDataSizeW(m_EditCommand.GetWindowTextW(wstr.GetBufferW(), wstr.GetBufSizeW()));
	WtoUtf8(wstr, str);
	m_Console.Write(str);
}


void CPTYMainDlg::OnBnClickedButtonVtfileOpen()
{
	CString strPathName;
	m_StaticVTFilePath.GetWindowTextW(strPathName);

	TCHAR szFilters[] = _T("VT Files (*.vt)|*.vt|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("vt"), strPathName,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	// Display the file dialog. When user clicks OK, fileDlg.DoModal() 
	// returns IDOK.
	if (fileDlg.DoModal() == IDOK)
	{
		strPathName = fileDlg.GetPathName();
		m_StaticVTFilePath.SetWindowText(strPathName);
	}
	OnBnClickedButtonReset();
}


void CPTYMainDlg::OnBnClickedButtonVtPlay()
{
	using namespace std::placeholders;
	using namespace std;
	if (m_bIsVTRunning == true)
	{
		KillTimer(emTimer1);
		m_bIsVTRunning = false;
		return;
	}
	if (m_VTReadNumByte.IsOpen() == false)
	{
		CString strPathName;
		m_StaticVTFilePath.GetWindowTextW(strPathName);
		CBinaryString wstr(strPathName.GetString());
		m_VTReadNumByte.SetFileName(wstr);
		bool rval = m_VTReadNumByte.Open();
		if (rval == false)
		{
			MessageBox(_T("Files can not be opened."));
		}
	}

	m_VTReadNumByte.SetRepeat(m_CheckVtRepeat.GetCheck() == BST_CHECKED);
	CString str;
	m_EditNBytesRead.GetWindowTextW(str);
	int numReadByte = std::stoi(str.GetString());
	m_VTReadNumByte.SetNumBytesInOneTime(numReadByte);
	m_EditVtTimer.GetWindowTextW(str);
	int numDev = m_SpinBtTimer.GetPos32();
	SetTimer(emTimer1, 1001- numDev,NULL);
	m_bIsVTRunning = true;
}


void CPTYMainDlg::OnBnClickedButtonVtStop()
{
	KillTimer(emTimer1);
	m_VTReadNumByte.Close();
	m_bIsVTRunning = false;
}



void CPTYMainDlg::OnBnClickedButtonPHextext()
{
	using namespace CBWinConv;
	CBinaryString wstr;
	wstr.SetDataSizeW(m_EditHexText.GetWindowTextW(wstr.GetBufferW(), wstr.GetBufSizeW()));
	wstr.SpaceHexTextToBinaryDataW();
	m_Console.Write(wstr);
}



void CPTYMainDlg::OnBnClickedButtonSdtHextext()
{
	using namespace CBWinConv;
	CBinaryString wstr;
	wstr.SetDataSizeW(m_EditHexText.GetWindowTextW(wstr.GetBufferW(), wstr.GetBufSizeW()));
	wstr.SpaceHexTextToBinaryDataW();
	m_Console.StdWrite(wstr);
}


void CPTYMainDlg::OnBnClickedButtonPCrlf()
{
	m_Console.Write("\r\n");
}


void CPTYMainDlg::OnBnClickedButtonStdEsc()
{
	CBinaryString str("1B");
	str.HexTextToBinaryDataA();
	m_Console.StdWrite(str);
}


void CPTYMainDlg::OnBnClickedButtonVtfileClose()
{
	m_VTReadNumByte.Close();
	m_bIsVTRunning = false;
	KillTimer(emTimer1);
}


void CPTYMainDlg::OnBnClickedButtonReset()
{
	KillTimer(emTimer1);
	m_VTReadNumByte.Close();
	m_bIsVTRunning = false;
	CBinaryString bstr("\x1b");
	bstr += "c";
	m_Console.StdWrite(bstr);
	OnBnClickedButtonVtPlay();
}


void CPTYMainDlg::OnEnChangeEditVtTimer()
{
	using namespace std;
	if (m_EditVtTimer.m_hWnd != nullptr)
	{
		CString str;
		int nDev = m_SpinBtTimer.GetPos32();
		SetTimer(emTimer1,1001-nDev,NULL);
	}
}


void CPTYMainDlg::OnEnChangeEditNBytesRead()
{
	using namespace std;
	if (m_EditNBytesRead.m_hWnd!=nullptr)
	{
		CString str;
		int iBytes = m_SpinNBytesRead.GetPos32();
		m_VTReadNumByte.SetNumBytesInOneTime(iBytes);
	}
}


void CPTYMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	CBinaryString bstr;
	switch (nIDEvent)
	{
	case emTimer1:
		m_VTReadNumByte.ReadOneTime(bstr);
		if (m_VTReadNumByte.IsEnd())
		{
			KillTimer(emTimer1);
		}
		m_Console.StdWrite(bstr);
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CPTYMainDlg::OnBnClickedCheckVtRepeat()
{
	m_VTReadNumByte.SetRepeat(m_CheckVtRepeat.GetCheck() == BST_CHECKED);
}



//void CPTYMainDlg::OnBnClickedButtonReset()
//{
//	// TODO: ここにコントロール通知ハンドラー コードを追加します。
//}
