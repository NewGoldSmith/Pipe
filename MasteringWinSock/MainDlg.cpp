
// MainDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "MasteringWinSock.h"
#include "MainDlg.h"
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


// MainDlg ダイアログ



MainDlg::MainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MASTERINGWINSOCK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ListenID = 0;
	m_SevID = 0;
	m_CliID = 0;
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_LISTEN, m_IPAddressListenAddress);
	DDX_Control(pDX, IDC_SPIN_LISTEN_ADDRESS, m_SpinListenAddress);
	DDX_Control(pDX, IDC_EDIT_LISTEN_PORT, m_EditListenPort);
	DDX_Control(pDX, IDC_SPIN_LISTEN_PORT, m_SpinListenPort);
	DDX_Control(pDX, IDC_IPADDRESS_CLI, m_IPAddressCliAddress);
	DDX_Control(pDX, IDC_SPIN_CLI_ADDRESS, m_SpinCliAddress);
	DDX_Control(pDX, IDC_EDIT_CLI_PORT, m_EditCliPort);
	DDX_Control(pDX, IDC_SPIN_CLI_PORT, m_SpinCliPort);
	DDX_Control(pDX, IDC_IPADDRESS_CONNECT_TO_ADDRESS, m_IPAddressConnectToAddress);
	DDX_Control(pDX, IDC_SPIN_CONNECT_TO_ADDRESS, m_SpinConnectToAddress);
	DDX_Control(pDX, IDC_EDIT_CONNECT_TO_PORT, m_EditConnectToPort);
	DDX_Control(pDX, IDC_SPIN_CONNECT_TO_PORT, m_SpinConnectToPort);
	DDX_Control(pDX, IDC_LIST_LISTEN_CREATE, m_ListListenCreate);
	DDX_Control(pDX, IDC_LIST_CLI_CREATE, m_ListCliCreate);
	DDX_Control(pDX, IDC_LIST_SEV_ACCEPTED, m_ListSevConnected);
	DDX_Control(pDX, IDC_LIST_CLI_ACCEPTED, m_ListCliConnected);
	DDX_Control(pDX, IDC_EDIT_SEV_RECEIVED_BINARY, m_EditSevReceivedBinary);
	DDX_Control(pDX, IDC_EDIT_SEV_RECEIVED_TEXT, m_EditSevReceivedText);
	DDX_Control(pDX, IDC_EDIT_CLI_RECEIVED_BINARY, m_EditCliReceivedBinary);
	DDX_Control(pDX, IDC_EDIT_CLI_RECEIVED_TEXT, m_EditCliReceivedText);
	DDX_Control(pDX, IDC_LIST_SEV_CREATE, m_ListSevCreate);
	DDX_Control(pDX, IDC_RADIO_SHUTDOWN_FLAG_RECEIVES, m_RadioShutdownReceives);
	DDX_Control(pDX, IDC_RADIO_SHUTDOWN_FLAG_SENDS, m_RadioShutdownSends);
	DDX_Control(pDX, IDC_RADIO_SHUTDOWN_FLAG_BOTH, m_RadioShutdownBoth);
	DDX_Control(pDX, IDC_COMBO_CLI_BYNARY_FOR_SENDING, m_ComboCliBynaryForSending);
	DDX_Control(pDX, IDC_COMBO_SEV_BINARY_FOR_SENDING, m_ComboSevBinaryForSending);
	DDX_Control(pDX, IDC_COMBO_SEV_TEXT_FOR_SENDING, m_ComboSevTextForSending);
	DDX_Control(pDX, IDC_COMBO_CLI_TEXT_FOR_SENDING, m_ComboCliTextForSending);
	DDX_Control(pDX, IDC_BUTTON_SEV_SEND_BINARY, m_ButtonSevSendBinary);
	DDX_Control(pDX, IDC_BUTTON_SEV_SEND_TEXT, m_ButtonSevSendText);
	DDX_Control(pDX, IDC_BUTTON_CLI_SEND_BINARY, m_ButtonCliSendBinary);
	DDX_Control(pDX, IDC_BUTTON_CLI_SEND_TEXT, m_ButtonCliSendText);
	DDX_Control(pDX, IDC_CHECK_SOCKET_BUFFER_CHANGE_FROM_DEFAULT_BUFFER_SIZE, m_CheckSocketBufferChange);
	DDX_Control(pDX, IDC_EDIT_SOCKET_BUFFER_SIZE, m_EditSocketBuffer);
	DDX_Control(pDX, IDC_SPIN_SOCKET_BUFFER_SIZE, m_SpinSocketBuffer);
}

BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_LISTEN_CREATE, &MainDlg::OnBnClickedButtonListenCreate)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN_DELETE, &MainDlg::OnBnClickedButtonListenDelete)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN_LISTEN, &MainDlg::OnBnClickedButtonListenListen)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN_CLOSE, &MainDlg::OnBnClickedButtonListenClose)
	ON_BN_CLICKED(IDC_BUTTON_CLI_CREATE, &MainDlg::OnBnClickedButtonCliCreate)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN_NEW, &MainDlg::OnBnClickedButtonListenNew)
	ON_BN_CLICKED(IDC_BUTTON_CLI_NEW, &MainDlg::OnBnClickedButtonCliNew)
	ON_BN_CLICKED(IDC_BUTTON_CLI_CLOSE, &MainDlg::OnBnClickedButtonCliClose)
	ON_BN_CLICKED(IDC_BUTTON_CLI_DELETE, &MainDlg::OnBnClickedButtonCliDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLI_CONNECT, &MainDlg::OnBnClickedButtonCliConnect)
	ON_MESSAGE(WM_USER_SOCK_LISTEN_MESSAGE, &MainDlg::OnUserPipeListenMessage)
	ON_MESSAGE(WM_USER_SOCK_SERVER_MESSAGE, &MainDlg::OnUserPipeServerMessage)
	ON_MESSAGE(WM_USER_SOCK_CLIENT_MESSAGE, &MainDlg::OnUserPipeClientMessage)
	ON_BN_CLICKED(IDC_BUTTON_ACCEPT, &MainDlg::OnBnClickedButtonAccept)
	ON_BN_CLICKED(IDC_BUTTON_SEV_CONNECTING_CLOSE, &MainDlg::OnBnClickedButtonSevConnectingClose)
	ON_BN_CLICKED(IDC_BUTTON_SEV_CONNECTING_DELETE, &MainDlg::OnBnClickedButtonSevConnectingDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLI_CONNECTING_CLOSE, &MainDlg::OnBnClickedButtonCliConnectingClose)
	ON_BN_CLICKED(IDC_BUTTON_CLI_CONNECTING_DELETE, &MainDlg::OnBnClickedButtonCliConnectingDelete)
	ON_BN_CLICKED(IDC_BUTTON_SEV_NEW, &MainDlg::OnBnClickedButtonSevNew)
	ON_BN_CLICKED(IDC_BUTTON_SEV_DELETE, &MainDlg::OnBnClickedButtonSevDelete)
	ON_BN_CLICKED(IDC_BUTTON_SEV_CREATE, &MainDlg::OnBnClickedButtonSevCreate)
	ON_BN_CLICKED(IDC_BUTTON_SEV_CLOSE, &MainDlg::OnBnClickedButtonSevClose)
	ON_BN_CLICKED(IDC_BUTTON_SEV_SEND_BINARY, &MainDlg::OnBnClickedButtonSevSendBinary)
	ON_BN_CLICKED(IDC_BUTTON_CLI_CONNECTING_RECEIVE, &MainDlg::OnBnClickedButtonCliConnectingReceive)
	ON_BN_CLICKED(IDC_BUTTON_CLI_CLEAR_TEXT, &MainDlg::OnBnClickedButtonCliClearText)
	ON_BN_CLICKED(IDC_BUTTON_SEV_SEND_TEXT, &MainDlg::OnBnClickedButtonSevSendText)
	ON_BN_CLICKED(IDC_BUTTON_SEV_CLEAR_TEXT, &MainDlg::OnBnClickedButtonSevClearText)
	ON_BN_CLICKED(IDC_BUTTON_CLI_SEND_BINARY, &MainDlg::OnBnClickedButtonCliSendBinary)
	ON_BN_CLICKED(IDC_BUTTON_CLI_SEND_TEXT, &MainDlg::OnBnClickedButtonCliSendText)
	ON_BN_CLICKED(IDC_BUTTON_SEV_CONNECTING_RECEIVE, &MainDlg::OnBnClickedButtonSevConnectingReceive)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN_SHUTDOWN, &MainDlg::OnBnClickedButtonListenShutdown)
	ON_BN_CLICKED(IDC_BUTTON_CLI_SHUTDOWN, &MainDlg::OnBnClickedButtonCliShutdown)
	ON_BN_CLICKED(IDC_BUTTON_SEV_SHUTDOWN, &MainDlg::OnBnClickedButtonSevShutdown)
	ON_BN_CLICKED(IDC_BUTTON_SEV_CONNECTING_SHUTDOWN, &MainDlg::OnBnClickedButtonSevConnectingShutdown)
	ON_BN_CLICKED(IDC_BUTTON_CLI_CONNECTED_SHUTDOWN, &MainDlg::OnBnClickedButtonCliConnectedShutdown)
END_MESSAGE_MAP()


// MainDlg メッセージ ハンドラー

BOOL MainDlg::OnInitDialog()
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

	m_SpinListenAddress.SetRange32(0x0, 0xffffffff);
	m_SpinListenAddress.SetBuddy(&m_IPAddressListenAddress);
	m_SpinListenPort.SetRange32(0x0,0x7fffffff);
	m_SpinListenPort.SetBuddy(&m_EditListenPort);

	m_SpinCliAddress.SetRange32(0x0, 0xffffffff);
	m_SpinCliAddress.SetBuddy(&m_IPAddressCliAddress);
	m_SpinCliPort.SetRange32(0x0, 0x7fffffff);
	m_SpinCliPort.SetBuddy(&m_EditCliPort);

	m_SpinConnectToAddress.SetRange32(0x0, 0xffffffff);
	m_SpinConnectToAddress.SetBuddy(&m_IPAddressConnectToAddress);
	m_SpinConnectToPort.SetRange32(0x0, 0x7fffffff);
	m_SpinConnectToPort.SetBuddy(&m_EditConnectToPort);
	
	m_ListListenCreate.Init();
	m_ListSevConnected.Init();
	m_ListCliCreate.Init();
	m_ListSevCreate.Init();
	m_ListCliConnected.Init();

	m_ComboSevBinaryForSending.Init(&m_ButtonSevSendBinary);
	m_ComboSevTextForSending.Init(&m_ButtonSevSendText);
	m_ComboCliBynaryForSending.Init(&m_ButtonCliSendBinary);
	m_ComboCliTextForSending.Init(&m_ButtonCliSendText);

	m_SpinSocketBuffer.SetRange32(0x0, 0x7fffffff);
	m_SpinSocketBuffer.SetBuddy(&m_EditSocketBuffer);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void MainDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void MainDlg::OnPaint()
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
HCURSOR MainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MainDlg::LoadProfile()
{
	StringHelper SH;
	CWinApp* pApp = AfxGetApp();
	CString strSec(_T("SYSTEM"));
	m_bFirst_Appear = pApp->GetProfileInt(strSec, _T("FIRST_APPEAR"), TRUE);
	m_rectMainWnd.left = pApp->GetProfileInt(strSec, _T("WINDOW_POS_MAIN_LEFT"), NULL);
	m_rectMainWnd.top = pApp->GetProfileInt(strSec, _T("WINDOW_POS_MAIN_TOP"), NULL);
	m_rectMainWnd.right = pApp->GetProfileInt(strSec, _T("WINDOW_POS_MAIN_RIGHT"), NULL);
	m_rectMainWnd.bottom = pApp->GetProfileInt(strSec, _T("WINDOW_POS_MAIN_BOTTOM"), NULL);

	strSec = _T("CONTENTS");
	CString strContents;


	strSec = _T("IP_ADDRESS");
	DWORD dw;
	dw= pApp->GetProfileInt(strSec, _T("IP_ADDRESS_LISTEN"), 2130706434);
	m_IPAddressListenAddress.SetAddress(dw);
	dw = pApp->GetProfileInt(strSec, _T("IP_ADDRESS_CLI"), 2130706436);
	m_IPAddressCliAddress.SetAddress(dw);
	dw = pApp->GetProfileInt(strSec, _T("IP_ADDRESS_CONNECT_TO"), 2130706434);
	m_IPAddressConnectToAddress.SetAddress(dw);

	strSec = _T("PORT");
	CString strPort;
	dw = pApp->GetProfileInt(strSec, _T("LISTEN_PORT"), 50000);
	strPort.Format(_T("%u"), dw);
	m_EditListenPort.SetWindowTextW(strPort);
	dw = pApp->GetProfileInt(strSec, _T("CLI_PORT"), 0);
	strPort.Format(_T("%u"), dw);
	m_EditCliPort.SetWindowTextW(strPort);
	dw = pApp->GetProfileInt(strSec, _T("CONNECT_TO_PORT"), 50000);
	strPort.Format(_T("%u"), dw);
	m_EditConnectToPort.SetWindowTextW(strPort);

	int num;
	strSec = _T("SHUTDOWN_HOW");
	num = pApp->GetProfileInt(strSec, _T("SHUTDOWN_HOW_N"), CAsyncSocket::sends);
	SetShutdownHowCheck(num);

	strSec = _T("COMBO_SEV_BINARY");
	m_ComboSevBinaryForSending.LoadProfile(pApp->m_pszProfileName, strSec);

	strSec = _T("COMBO_SEV_TEXT");
	m_ComboSevTextForSending.LoadProfile(pApp->m_pszProfileName, strSec);

	strSec = _T("COMBO_CLI_BINARY");
	m_ComboCliBynaryForSending.LoadProfile(pApp->m_pszProfileName, strSec);

	strSec = _T("COMBO_CLI_TEXT");
	m_ComboCliTextForSending.LoadProfile(pApp->m_pszProfileName, strSec);

	strSec = _T("SOCKET_OPTION");
	m_EditSocketBuffer.SetWindowText(pApp->GetProfileString(strSec, _T("BUFFER_SIZE"), _T("5000")));
	m_CheckSocketBufferChange.SetCheck(pApp->GetProfileInt(strSec, _T("BUFFER_CHANGE"), BST_UNCHECKED));
}


void MainDlg::SaveProfile()
{
	CWinApp* pApp = AfxGetApp();
	RECT rect;
	CString strSec(_T("SYSTEM"));
	//ウィンドウ位置を保存
	GetWindowRect(&rect);
	pApp->WriteProfileInt(strSec, _T("WINDOW_POS_MAIN_LEFT"), rect.left);
	pApp->WriteProfileInt(strSec, _T("WINDOW_POS_MAIN_TOP"), rect.top);
	pApp->WriteProfileInt(strSec, _T("WINDOW_POS_MAIN_RIGHT"), rect.right);
	pApp->WriteProfileInt(strSec, _T("WINDOW_POS_MAIN_BOTTOM"), rect.bottom);
	pApp->WriteProfileInt(strSec, _T("FIRST_APPEAR"), FALSE);

	strSec = _T("CONTENTS");
	CString strContents;
	
	strSec = _T("IP_ADDRESS");
	DWORD dw;
	m_IPAddressListenAddress.GetAddress(dw);
	pApp->WriteProfileInt(strSec, _T("IP_ADDRESS_LISTEN"), dw);
	m_IPAddressCliAddress.GetAddress(dw);
	pApp->WriteProfileInt(strSec, _T("IP_ADDRESS_CLI"), dw);
	m_IPAddressConnectToAddress.GetAddress(dw);
	pApp->WriteProfileInt(strSec, _T("IP_ADDRESS_CONNECT_TO"), dw);

	strSec = _T("PORT");
	CString strPort;
	dw=m_SpinListenPort.GetPos32();
	pApp->WriteProfileInt(strSec, _T("LISTEN_PORT"), dw);
	dw = m_SpinCliPort.GetPos32();
	pApp->WriteProfileInt(strSec, _T("CLI_PORT"), dw);
	dw = m_SpinConnectToPort.GetPos32();
	pApp->WriteProfileInt(strSec, _T("CONNECT_TO_PORT"), dw);

	strSec = _T("SHUTDOWN_HOW");
	int num;
	num = GetShutdownHowCheck();
	pApp->WriteProfileInt(strSec, _T("SHUTDOWN_HOW_N"), num);

	strSec = _T("COMBO_SEV_BINARY");
	m_ComboSevBinaryForSending.SaveProfile(pApp->m_pszProfileName, strSec);

	strSec = _T("COMBO_SEV_TEXT");
	m_ComboSevTextForSending.SaveProfile(pApp->m_pszProfileName, strSec);

	strSec = _T("COMBO_CLI_BINARY");
	m_ComboCliBynaryForSending.SaveProfile(pApp->m_pszProfileName, strSec);

	strSec = _T("COMBO_CLI_TEXT");
	m_ComboCliTextForSending.SaveProfile(pApp->m_pszProfileName, strSec);

	strSec = _T("SOCKET_OPTION");
	CString strNBuffer;
	m_EditSocketBuffer.GetWindowText(strNBuffer);
	pApp->WriteProfileString(strSec, _T("BUFFER_SIZE"), strNBuffer);
	pApp->WriteProfileInt(strSec,_T("BUFFER_CHANGE"),m_CheckSocketBufferChange.GetCheck());
}


void MainDlg::OnClose()
{
	DeleteAllListItem();
	SaveProfile();
	CDialogEx::OnClose();
}


void MainDlg::OnOK()
{
	OnClose();
	CDialogEx::OnOK();
}


void MainDlg::OnCancel()
{
	OnClose();
	CDialogEx::OnCancel();
}


BOOL MainDlg::PreTranslateMessage(MSG* pMsg)
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
	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void MainDlg::OnBnClickedButtonListenCreate()
{
	CAsyncSocketDX* pSocket = m_ListListenCreate.GetFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListListenCreate.ErrMessageBox();
		return;
	}
	if (!pSocket->Create(m_SpinListenPort.GetPos32(), 1, 63, m_IPAddressListenAddress.GetIPAddressString()))
	{
		pSocket->ErrMessageBox();
		return;
	}

	if (m_CheckSocketBufferChange.GetCheck() == BST_CHECKED)
	{
		int nBfferSize = m_EditSocketBuffer.GetNum();
		if (!pSocket->SetSockOpt(SO_RCVBUF, &nBfferSize, sizeof(int), SOL_SOCKET))
		{
			pSocket->ErrMessageBox();
			return;
		}
	}

	m_ListListenCreate.UpDateView();
}


void MainDlg::OnBnClickedButtonListenDelete()
{
	m_ListListenCreate.DeleteSelectedSocket();
}


void MainDlg::OnBnClickedButtonListenListen()
{
	CAsyncSocketDX * pSocket=m_ListListenCreate.GetSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListListenCreate.ErrMessageBox();
		return;
	}
	if (!pSocket->Listen())
	{
		pSocket->ErrMessageBox();
	}
	m_ListListenCreate.UpDateView();
}


void MainDlg::OnBnClickedButtonListenClose()
{
	CAsyncSocketDX* pSocket = m_ListListenCreate.GetSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListListenCreate.ErrMessageBox();
		return;
	}
	pSocket->Close();
	m_ListListenCreate.UpDateView();
}

void MainDlg::DeleteAllListItem()
{
	m_ListListenCreate.AllSocketDelete();
	m_ListCliCreate.AllSocketDelete();
	m_ListSevConnected.AllSocketDelete();
	m_ListCliConnected.AllSocketDelete();
	m_ListSevCreate.AllSocketDelete();
}


void MainDlg::OnBnClickedButtonCliCreate()
{
	CAsyncSocketDX* pSocket = m_ListCliCreate.GetSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListCliCreate.ErrMessageBox();
		return;
	}
	if (!pSocket->Create(m_SpinCliPort.GetPos32(), 1, 63, m_IPAddressCliAddress.GetIPAddressString()))
	{
		pSocket->ErrMessageBox();
		return;
	}
	if (m_CheckSocketBufferChange.GetCheck() == BST_CHECKED)
	{
		int nBfferSize = m_EditSocketBuffer.GetNum();
		if (!pSocket->SetSockOpt(SO_RCVBUF, &nBfferSize, sizeof(int), SOL_SOCKET))
		{
			pSocket->ErrMessageBox();
			return;
		}
	}
	m_ListCliCreate.UpDateView();
}


void MainDlg::OnBnClickedButtonListenNew()
{
	CAsyncSocketDX* pSocket = new CAsyncSocketDX;
	pSocket->Init(WM_USER_SOCK_LISTEN_MESSAGE, this, m_ListenID, CAsyncSocketDX::Attri::SevListen);
	m_ListenID++;
	m_ListListenCreate.AddSocket(pSocket);
}


void MainDlg::OnBnClickedButtonCliNew()
{
	CAsyncSocketDX* pSocket = new CAsyncSocketDX;
	pSocket->Init(WM_USER_SOCK_CLIENT_MESSAGE, this, m_CliID, CAsyncSocketDX::Attri::Cli);
	m_ListCliCreate.AddSocket(pSocket);
	m_CliID++;
}


void MainDlg::OnBnClickedButtonCliClose()
{
	CAsyncSocketDX* pSocket = m_ListCliCreate.GetSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListCliCreate.ErrMessageBox();
		return;
	}
	pSocket->Close();
	m_ListCliCreate.UpDateView();
}


void MainDlg::OnBnClickedButtonCliDelete()
{
	m_ListCliCreate.DeleteSelectedSocket();
}


void MainDlg::OnBnClickedButtonCliConnect()
{
	CAsyncSocketDX* pSocket = m_ListCliCreate.PullFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListCliCreate.ErrMessageBox();
		return;
	}
	TCHAR str[256] = {};
	m_EditConnectToPort.GetLine(0, str, 256);
	if (!pSocket->Connect(m_IPAddressConnectToAddress.GetIPAddressString(), _tstoi(str)))
	{
		pSocket->ErrMessageBox();
		m_ListCliCreate.AddSocket(pSocket);
		return;
	}
	m_ListCliConnected.AddSocket(pSocket);
}


//CAsyncSocketDXからポストされるメッセージのハンドラ。リストの更新に使っている。
afx_msg LRESULT MainDlg::OnUserPipeListenMessage(WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	CAsyncSocketDX* pSocket;
	switch (lParam)
	{
	case (LPARAM)CAsyncSocketDX::Event::OnAccept:
		break;
	case (LPARAM)CAsyncSocketDX::Event::Err:
		i = 1;
		break;
	}
	AllListUpDateView();
	return 0;
}


//CAsyncSocketDXからポストされるメッセージのハンドラ。リストの更新に使っている。
afx_msg LRESULT MainDlg::OnUserPipeServerMessage(WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	switch (lParam)
	{
	case (LPARAM)CAsyncSocketDX::Event::OnClose:
		i = 1;
		break;
	case (LPARAM)CAsyncSocketDX::Event::Err:
		i = 1;
		break;
	}
	AllListUpDateView();
	return 0;
}


//CAsyncSocketDXからポストされるメッセージのハンドラ。リストの更新に使っている。
afx_msg LRESULT MainDlg::OnUserPipeClientMessage(WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	CAsyncSocketDX* pSocket = NULL;
	switch (lParam)
	{
	case (LPARAM)CAsyncSocketDX::Event::OnConnect:
		i = 1;
		break;
	case (LPARAM)CAsyncSocketDX::Event::OnSend:
		i = 1;
		break;
	case (LPARAM)CAsyncSocketDX::Event::Err:
	case (LPARAM)CAsyncSocketDX::Event::OnClose:
		break;
	}
	AllListUpDateView();
	return 0;
}


void MainDlg::OnBnClickedButtonAccept()
{
	CAsyncSocketDX* pSocketListen = m_ListListenCreate.GetFirstCheckedOrSelectedSocket();
	if (pSocketListen == NULL)
	{
		m_ListListenCreate.ErrMessageBox();
		return;
	}
	CAsyncSocketDX* pSocketSev = m_ListSevCreate.GetFirstCheckedOrSelectedSocket();
	if (pSocketSev == NULL)
	{
		m_ListSevCreate.ErrMessageBox();
		return;
	}

	if (!pSocketListen->Accept(*pSocketSev))
	{
		pSocketListen->ErrMessageBox();
		return;
	}
	m_ListSevCreate.PullFirstCheckedOrSelectedSocket();
	m_ListSevConnected.AddSocket(pSocketSev);
}


void MainDlg::OnBnClickedButtonSevConnectingClose()
{
	CAsyncSocketDX* pSocket = m_ListSevConnected.PullFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListSevConnected.ErrMessageBox();
		return;
	}
	pSocket->Close();
	m_ListSevCreate.AddSocket(pSocket);
	m_ListSevConnected.UpDateView();
}


void MainDlg::OnBnClickedButtonSevConnectingDelete()
{
	CAsyncSocketDX* pSocket = m_ListSevConnected.PullFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListSevConnected.ErrMessageBox();
		return;
	}
	delete pSocket;
}


void MainDlg::OnBnClickedButtonCliConnectingClose()
{
	CAsyncSocketDX* pSocket = m_ListCliConnected.PullFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListCliConnected.ErrMessageBox();
		return;
	}
	pSocket->Close();
	m_ListCliCreate.AddSocket(pSocket);
}


void MainDlg::OnBnClickedButtonCliConnectingDelete()
{
	CAsyncSocketDX* pSocket = m_ListCliConnected.PullFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListCliConnected.ErrMessageBox();
		return;
	}
	delete pSocket;
}


void MainDlg::AllListUpDateView()
{
	m_ListListenCreate.UpDateView();
	m_ListSevCreate.UpDateView();
	m_ListSevConnected.UpDateView();
	m_ListCliCreate.UpDateView();
	m_ListCliConnected.UpDateView();
}



void MainDlg::OnBnClickedButtonSevNew()
{
	CAsyncSocketDX* pSocket = new CAsyncSocketDX;
	pSocket->Init(WM_USER_SOCK_SERVER_MESSAGE, this, m_SevID, CAsyncSocketDX::Attri::Sev);
	m_ListSevCreate.AddSocket(pSocket);
	m_SevID++;
}


void MainDlg::OnBnClickedButtonSevDelete()
{
	CAsyncSocketDX* pSocket = m_ListSevCreate.PullFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListSevCreate.ErrMessageBox();
		return;
	}
	delete pSocket;
}


void MainDlg::OnBnClickedButtonSevCreate()
{
	CAsyncSocketDX* pSocket = m_ListSevCreate.GetSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListSevCreate.ErrMessageBox();
		return;
	}
	if (!pSocket->Create())
	{
		pSocket->ErrMessageBox();
		return;
	}
	if (m_CheckSocketBufferChange.GetCheck() == BST_CHECKED)
	{
		int nBfferSize = m_EditSocketBuffer.GetNum();
		if (!pSocket->SetSockOpt(SO_RCVBUF, &nBfferSize, sizeof(int), SOL_SOCKET))
		{
			pSocket->ErrMessageBox();
			return;
		}
	}
	m_ListSevCreate.UpDateView();
}


void MainDlg::OnBnClickedButtonSevClose()
{
	CAsyncSocketDX* pSocket = m_ListSevCreate.GetSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListSevCreate.ErrMessageBox();
		return;
	}
	pSocket->Close();
	m_ListSevCreate.UpDateView();
	return;
}


void MainDlg::OnBnClickedButtonSevSendBinary()
{
	CAsyncSocketDX* pSocket = m_ListSevConnected.GetFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListSevConnected.ErrMessageBox();
		return;
	}

	StringHelper SH;
	CString str;
	BYTE byteData[BUF_SIZE];
	int nByte = 0;
	m_ComboSevBinaryForSending.GetWindowText(str);
	nByte=SH.TextToByteDATA(str, byteData, BUF_SIZE);
	if (nByte == -1)
	{
		SH.ErrMessageBox();
		return;
	}

	int len = pSocket->Send(byteData, nByte);
	if (len == SOCKET_ERROR)
	{
		pSocket->ErrMessageBox();
		return;
	}
	m_ListSevConnected.UpDateView();
}


void MainDlg::OnBnClickedButtonCliConnectingReceive()
{
	CAsyncSocketDX* pSocket = m_ListCliConnected.GetFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListCliConnected.ErrMessageBox();
		return;
	}
	char8_t byteData[BUF_SIZE] = {};
	int rVal = 0;
	rVal = pSocket->Receive(byteData, BUF_SIZE,0);
	if (rVal == SOCKET_ERROR)
	{
		pSocket->ErrMessageBox();
		return;
	}

	StringHelper SH;
	CString str;
	int i= SH.ByteDataToText(byteData, rVal, str);
	if (i == -1)
	{
		SH.ErrMessageBox();
		return;
	}
	if (i > 20)
	{
		str.Delete(0, i - 20);
	}
	m_EditCliReceivedBinary.SetWindowText(str);

	std::wstring stdWStr = SH.utf8_to_wide(byteData);
	CStringW strOld;
	m_EditCliReceivedText.GetWindowTextW(strOld);
	strOld += stdWStr.c_str();
	m_EditCliReceivedText.SetWindowText(strOld);
	int nLine = m_EditCliReceivedText.GetLineCount();
	m_EditCliReceivedText.LineScroll(nLine);
	m_ListCliConnected.UpDateView();
}



void MainDlg::OnBnClickedButtonCliClearText()
{
	m_EditCliReceivedText.SetWindowTextW(_T(""));
}


void MainDlg::OnBnClickedButtonSevSendText()
{
	CAsyncSocketDX* pSocket = m_ListSevConnected.GetFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListSevConnected.ErrMessageBox();
		return;
	}

	StringHelper SH;
	CString str;
	std::u8string byteData;
	int nByte = 0;
	m_ComboSevTextForSending.GetWindowTextW(str);
	//\r\n\tエスケープコードに変換。
	str=SH.TranslateEscape(str);
	byteData = SH.wide_to_utf8(std::wstring(str));
	nByte = byteData.length();
	int len = pSocket->Send(byteData.c_str(), nByte);
	if (len == SOCKET_ERROR)
	{
		pSocket->ErrMessageBox();
		return;
	}
	m_ListSevConnected.UpDateView();
}


void MainDlg::OnBnClickedButtonSevClearText()
{
	m_EditSevReceivedText.SetWindowTextW(_T(""));
}


void MainDlg::OnBnClickedButtonCliSendBinary()
{
	CAsyncSocketDX* pSocket = m_ListCliConnected.GetFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListCliConnected.ErrMessageBox();
		return;
	}

	StringHelper SH;
	CString str;
	BYTE byteData[BUF_SIZE];
	int nByte = 0;
	m_ComboCliBynaryForSending.GetWindowTextW(str);
	nByte = SH.TextToByteDATA(str, byteData, BUF_SIZE);
	if (nByte == -1)
	{
		SH.ErrMessageBox();
		return;
	}

	int len = pSocket->Send(byteData, nByte);
	if (len == SOCKET_ERROR)
	{
		pSocket->ErrMessageBox();
		return;
	}
	m_ListCliConnected.UpDateView();
}


void MainDlg::OnBnClickedButtonCliSendText()
{
	CAsyncSocketDX* pSocket = m_ListCliConnected.GetFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListCliConnected.ErrMessageBox();
		return;
	}

	StringHelper SH;
	CString str;
	std::u8string byteData;
	int nByte = 0;
	m_ComboCliTextForSending.GetWindowTextW(str);
	//\r\n\tエスケープコードに変換。
	str = SH.TranslateEscape(str);
	byteData = SH.wide_to_utf8(std::wstring(str));
	nByte = byteData.length();
	int len = pSocket->Send(byteData.c_str(), nByte);
	if (len == SOCKET_ERROR)
	{
		pSocket->ErrMessageBox();
		return;
	}
	m_ListCliConnected.UpDateView();
}


void MainDlg::OnBnClickedButtonSevConnectingReceive()
{
	CAsyncSocketDX* pSocket = m_ListSevConnected.GetFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListSevConnected.ErrMessageBox();
		return;
	}
	char8_t byteData[BUF_SIZE] = {};
	int rVal = 0;
	rVal = pSocket->Receive(byteData, BUF_SIZE, 0);
	if (rVal == SOCKET_ERROR)
	{
		pSocket->ErrMessageBox();
		return;
	}
	byteData[rVal] = L'\0';

	StringHelper SH;
	CString str;
	int i = SH.ByteDataToText(byteData, rVal, str);
	if (i == -1)
	{
		SH.ErrMessageBox();
		return;
	}
	m_EditSevReceivedBinary.SetWindowText(str);

	std::wstring stdWStr = SH.utf8_to_wide(byteData);
	CStringW strOld;
	m_EditSevReceivedText.GetWindowTextW(strOld);
	strOld += stdWStr.c_str();
	m_EditSevReceivedText.SetWindowText(strOld);
	int nLine = m_EditSevReceivedText.GetLineCount();
	m_EditSevReceivedText.LineScroll(nLine);
	m_ListSevConnected.UpDateView();
}


void MainDlg::OnBnClickedButtonListenShutdown()
{
	CAsyncSocketDX* pSocket = m_ListListenCreate.GetSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListListenCreate.ErrMessageBox();
		return;
	}
	
	if (!pSocket->ShutDown(GetShutdownHowCheck()))
	{
		pSocket->ErrMessageBox();
		return;
	}
	m_ListListenCreate.UpDateView();
}



void MainDlg::OnBnClickedButtonCliShutdown()
{
	CAsyncSocketDX* pSocket = m_ListCliCreate.GetSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListCliCreate.ErrMessageBox();
		return;
	}

	if (!pSocket->ShutDown(GetShutdownHowCheck()))
	{
		pSocket->ErrMessageBox();
		return;
	}
	m_ListCliCreate.UpDateView();
}


void MainDlg::OnBnClickedButtonSevShutdown()
{
	CAsyncSocketDX* pSocket = m_ListSevCreate.GetSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListSevCreate.ErrMessageBox();
		return;
	}

	if (!pSocket->ShutDown(GetShutdownHowCheck()))
	{
		pSocket->ErrMessageBox();
		return;
	}
	m_ListSevCreate.UpDateView();
}


void MainDlg::OnBnClickedButtonSevConnectingShutdown()
{
	CAsyncSocketDX* pSocket = m_ListSevConnected.GetFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListSevConnected.ErrMessageBox();
		return;
	}

	if (!pSocket->ShutDown(GetShutdownHowCheck()))
	{
		pSocket->ErrMessageBox();
		return;
	}
	m_ListSevConnected.UpDateView();
}


void MainDlg::OnBnClickedButtonCliConnectedShutdown()
{
	CAsyncSocketDX* pSocket = m_ListCliConnected.GetFirstCheckedOrSelectedSocket();
	if (pSocket == NULL)
	{
		m_ListCliConnected.ErrMessageBox();
		return;
	}

	if (!pSocket->ShutDown(GetShutdownHowCheck()))
	{
		pSocket->ErrMessageBox();
		return;
	}
	m_ListCliConnected.UpDateView();
}


int MainDlg::GetShutdownHowCheck()
{
	int nHow;
	if (m_RadioShutdownReceives.GetCheck() == BST_CHECKED)
	{
		nHow = CAsyncSocket::receives;
	}
	else if (m_RadioShutdownSends.GetCheck() == BST_CHECKED)
	{
		nHow = CAsyncSocket::sends;
	}
	else
	{
		nHow = CAsyncSocket::both;
	}
	return nHow;
}


int MainDlg::SetShutdownHowCheck(int nHow)
{
	int oldHow = GetShutdownHowCheck();
	switch (nHow)
	{
	case CAsyncSocket::receives:
		m_RadioShutdownReceives.SetCheck(BST_CHECKED);
		break;
		case CAsyncSocket::sends:
			m_RadioShutdownSends.SetCheck(BST_CHECKED);
		break;
		case CAsyncSocket::both:
			m_RadioShutdownBoth.SetCheck(BST_CHECKED);
		break;
	}
	return oldHow;
}

