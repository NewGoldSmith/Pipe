// CPropPageGenelal.cpp : 実装ファイル
//

#include "pch.h"
#include "PipeProbeSettingApp.h"
#include "CPropPageGenelal.h"
#include "afxdialogex.h"
#include "CProfileableDlg.h"


// CPropPageGenelal ダイアログ

IMPLEMENT_DYNAMIC(CPropPageGenelal, CProfileableDlg)

CPropPageGenelal::CPropPageGenelal(CWnd* pParent /*=nullptr*/)
	: CProfileableDlg(IDD_PROPPAGE_GENELAL, pParent)
	, m_strChildPathName(_T(""))
	
//	, m_strPipeProbeDistinationPathName(_T(""))
	, m_strPipeprobeSourcePathname(_T(""))
{

}

CPropPageGenelal::~CPropPageGenelal()
{
}

void CPropPageGenelal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_CHILD_PATHNAME, m_strChildPathName);
	//  DDX_Text(pDX, IDC_STATIC_PIPEPROBE_PLACEMENT_PATHNAME, m_strPipeProbeDistinationPathName);
	DDX_Text(pDX, IDC_STATIC_PIPEPROBE_SOURCE_PATHNAME, m_strPipeprobeSourcePathname);
	DDX_Control(pDX, IDC_RADIO_STARTUP_ICON, m_radStartupIcon);
	DDX_Control(pDX, IDC_RADIO_STARTUP_TASKTRAY, m_radStartupTasktray);
	DDX_Control(pDX, IDC_RADIO_STARTUP_WINDOW, m_radStartupWindow);
}


BEGIN_MESSAGE_MAP(CPropPageGenelal, CProfileableDlg)
	ON_BN_CLICKED(IDC_BUTTON_CHILD_SELECT, &CPropPageGenelal::OnBnClickedButtonChildSelect)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_SOURCE_PIPEPROBE_PATHNAME, &CPropPageGenelal::OnBnClickedButtonSelectSourcePipeprobePathname)
//	ON_BN_CLICKED(IDC_BUTTON_COPY_PIPEPROBE_PATHNAME, &CPropPageGenelal::OnBnClickedButtonCopyPipeprobePathname)
	ON_BN_CLICKED(IDC_RADIO_STARTUP_WINDOW, &CPropPageGenelal::OnClickedRadioStartupWindow)
	ON_BN_CLICKED(IDC_RADIO_STARTUP_ICON, &CPropPageGenelal::OnClickedRadioStartupIcon)
	ON_BN_CLICKED(IDC_RADIO_STARTUP_TASKTRAY, &CPropPageGenelal::OnClickedRadioStartupTasktray)
//	ON_BN_CLICKED(IDC_BUTTON_GENELAL_EXECUTE_COPY, &CPropPageGenelal::OnBnClickedButtonGenelalExecuteCopy)
END_MESSAGE_MAP()


// CPropPageGenelal メッセージ ハンドラー



void CPropPageGenelal::OnBnClickedButtonChildSelect()
{
	UpdateData(TRUE);
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("*.exe"), _T(""), OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("実行ファイル（*.exe)|*.exe|全て(*.*)|*.*||"), this);

	if (pDlg->DoModal() == IDOK)
	{
		if (m_strPipeprobeSourcePathname == pDlg->GetPathName())
		{
			AfxMessageBox(_T("PipeProbeと同じパスです。\r\n再帰呼び出しになるので選択出来ません。"), MB_OK | MB_ICONWARNING);
			delete pDlg;
			return;
		}
		m_strChildPathName = pDlg->GetPathName(); // return full path and filename
		m_bChange = TRUE;
	}
	UpdateData(FALSE);
	delete pDlg;
}


void CPropPageGenelal::LoadProfileApp(CWinApp* pApp)
{
	CString strSec = _T("GENERAL");
	m_strChildPathName =pApp->GetProfileString(strSec, _T("CHILD_PATHNAME"),_T(""));
//	m_strPipeProbeDistinationPathName= pApp->GetProfileString(strSec, _T("PIPEPROBE_PLACEMENT_PATHNAME"), _T(""));
	m_strPipeprobeSourcePathname = pApp->GetProfileString(strSec, _T("PIPEPROBE_SOURCE_PATHNAME"), _T(""));
	SetMainWindowShowStatus(pApp->GetProfileInt(strSec, _T("SHOW_STATUS"), 0));
	UpdateData(FALSE);
}


void CPropPageGenelal::SaveProfileApp(CWinApp* pApp)
{
	CString strSec = _T("GENERAL");
	UpdateData(TRUE);
	pApp->WriteProfileString(strSec, _T("CHILD_PATHNAME"), m_strChildPathName);
//	pApp->WriteProfileString(strSec, _T("PIPEPROBE_PLACEMENT_PATHNAME"), m_strPipeProbeDistinationPathName);
	pApp->WriteProfileString(strSec, _T("PIPEPROBE_SOURCE_PATHNAME"), m_strPipeprobeSourcePathname);
//このソフトで確かに設定した記録を残す為に、SYSTEMセクションにINI_EXISTINGを書き込む。これがFALSEならPipeProbeは立ち上がらない。
	pApp->WriteProfileInt(_T("SYSTEM"), _T("INI_EXISTING"), TRUE);

	pApp->WriteProfileInt(strSec, _T("SHOW_STATUS"), GetMainWindowShowStatus());

	m_bChange = FALSE;
}

void CPropPageGenelal::OnBnClickedButtonSelectSourcePipeprobePathname()
{
	UpdateData(TRUE);
	CFileDialog* pDlg = new CFileDialog(TRUE, _T("exe"), m_strPipeprobeSourcePathname, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, _T("実行ファイル（*.exe)|*.exe|全て(*.*)|*.*||"), this);

	if (pDlg->DoModal() == IDOK)
	{
		if (m_strChildPathName == pDlg->GetPathName())
		{
			AfxMessageBox(_T("子プロセスと同じパスです。\r\n再帰呼び出しになるので選択出来ません。"), MB_OK | MB_ICONWARNING);
			delete pDlg;
			return;
		}
		m_strPipeprobeSourcePathname = pDlg->GetPathName(); // return full path and filename
		m_bChange = TRUE;
	}
	UpdateData(FALSE);
	delete pDlg;
}


CString CPropPageGenelal::GetDestinationPathName()
{
	return CString();//m_strPipeProbeDistinationPathName;
}


//ラジオボタンは変更を検出出来ない為(m_bChangeが使えない)、手動でチェックを切り替え 
void CPropPageGenelal::OnClickedRadioStartupWindow()
{
	m_radStartupWindow.SetCheck(TRUE);
	m_radStartupIcon.SetCheck(FALSE);
	m_radStartupTasktray.SetCheck(FALSE);
	m_bChange = TRUE;
}


void CPropPageGenelal::OnClickedRadioStartupIcon()
{
	m_radStartupWindow.SetCheck(FALSE);
	m_radStartupIcon.SetCheck(TRUE);
	m_radStartupTasktray.SetCheck(FALSE);
	m_bChange = TRUE;
}


void CPropPageGenelal::OnClickedRadioStartupTasktray()
{
	m_radStartupWindow.SetCheck(FALSE);
	m_radStartupIcon.SetCheck(FALSE);
	m_radStartupTasktray.SetCheck(TRUE);
	m_bChange = TRUE;
}

//ラジオボタンの状態を取得
int CPropPageGenelal::GetMainWindowShowStatus()
{
	if (m_radStartupWindow.GetCheck() == BST_CHECKED)
	{
		return 0;
	}
	else if (m_radStartupIcon.GetCheck() == BST_CHECKED)
	{
		return 1;
	}
	else if( m_radStartupTasktray.GetCheck() == BST_CHECKED)
	{
		return 2;
	}
	return 3;
}

//ラジオボタンに状態をセット
int CPropPageGenelal::SetMainWindowShowStatus(int iStatus)
{
	switch (iStatus)
	{
	case 0:
		m_radStartupWindow.SetCheck(BST_CHECKED);
		break;
	case 1:
		m_radStartupIcon.SetCheck(BST_CHECKED);
		break;
	case 2:
		m_radStartupTasktray.SetCheck(BST_CHECKED);
		break;
	default:
		return 0;

	}
	return TRUE;
}


BOOL CPropPageGenelal::PreTranslateMessage(MSG* pMsg)
{
	CWnd* pWnd;
	switch (pMsg->message)
	{
	case WM_KEYDOWN:

		switch (pMsg->wParam)
		{
		case VK_TAB:
			NextDlgCtrl();
			return TRUE;
			break;
		case VK_RETURN:
			pWnd = GetFocus();
			pWnd->PostMessage(WM_LBUTTONDOWN, 0, 0);
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
			pWnd = GetFocus();
			pWnd->PostMessage(WM_LBUTTONUP, 0, 0);
			return TRUE;
		default:
			break;
		}
	default:
		break;
	}	return CProfileableDlg::PreTranslateMessage(pMsg);
}


void CPropPageGenelal::ErrDlg(LPTSTR lpszFunction)
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
	::MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK|MB_ICONERROR);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}


CString CPropPageGenelal::GetSourcePathName()
{
	return m_strPipeprobeSourcePathname;
}


CString CPropPageGenelal::GetChildPathName()
{
	return m_strChildPathName;
}
