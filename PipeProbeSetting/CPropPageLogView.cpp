// CPropPageLogView.cpp : 実装ファイル
//

#include "pch.h"
#include "PipeProbeSettingApp.h"
#include "CPropPageLogView.h"
#include "afxdialogex.h"
#include "CProfileableDlg.h"


// CPropPageLogView ダイアログ

IMPLEMENT_DYNAMIC(CPropPageLogView, CProfileableDlg)

CPropPageLogView::CPropPageLogView(CWnd* pParent /*=nullptr*/)
	: CProfileableDlg(IDD_PROPPAGE_LOG_VIEW, pParent)
	, m_bFormatDown(FALSE)
	, m_strSearchDown(_T(""))
	, m_strReplaceDown(_T(""))
	, m_bFormatUp(FALSE)
	, m_strSearchUp(_T(""))
	, m_strPreplaceUp(_T(""))
	, m_strMaxLine(_T(""))
{

}

CPropPageLogView::~CPropPageLogView()
{
}

void CPropPageLogView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_LOG_VIEW_FORMING_DOWN, m_bFormatDown);
	DDX_Text(pDX, IDC_EDIT_LOG_VIEW_SERCH_DOWN, m_strSearchDown);
	DDX_Text(pDX, IDC_EDIT_LOG_VIEW_FORMING_DOWN, m_strReplaceDown);
	DDX_Check(pDX, IDC_CHECK_LOG_VIEW_FORMING_UP, m_bFormatUp);
	DDX_Text(pDX, IDC_EDIT_LOG_VIEW_SERCH_UP, m_strSearchUp);
	DDX_Text(pDX, IDC_EDIT_LOG_VIEW_FORMING_UP, m_strPreplaceUp);
	DDX_Text(pDX, IDC_EDIT_MAX_LINE, m_strMaxLine);
	DDX_Control(pDX, IDC_SPIN_MAX_LINE, m_ctlMaxLine);
}


BEGIN_MESSAGE_MAP(CPropPageLogView, CProfileableDlg)
	ON_EN_CHANGE(IDC_EDIT_LOG_VIEW_SERCH_DOWN, &CPropPageLogView::OnEnChangeEditLogViewSerchDown)
	ON_EN_CHANGE(IDC_EDIT_LOG_VIEW_FORMING_DOWN, &CPropPageLogView::OnEnChangeEditLogViewFormingDown)
	ON_BN_CLICKED(IDC_CHECK_LOG_VIEW_FORMING_UP, &CPropPageLogView::OnBnClickedCheckLogViewFormingUp)
	ON_EN_CHANGE(IDC_EDIT_LOG_VIEW_SERCH_UP, &CPropPageLogView::OnEnChangeEditLogViewSerchUp)
	ON_EN_CHANGE(IDC_EDIT_LOG_VIEW_FORMING_UP, &CPropPageLogView::OnEnChangeEditLogViewFormingUp)
	ON_EN_CHANGE(IDC_EDIT_MAX_LINE, &CPropPageLogView::OnEnChangeEditMaxLine)
END_MESSAGE_MAP()


// CPropPageLogView メッセージ ハンドラー


void CPropPageLogView::LoadProfileApp(CWinApp* pApp)
{
	CString strSec=_T("LOG_VIEW");
	m_bFormatDown = pApp->GetProfileInt(strSec, _T("FORMAT_DOWN"), TRUE);
	m_strSearchDown = pApp->GetProfileString(strSec, _T("SEARCH_DOWN"), _T("(.*)(\\r?)(\\n?)"));
	m_strReplaceDown = pApp->GetProfileString(strSec, _T("REPLACE_DOWN"), _T("[DOWN]->$1$2$3"));
	m_bFormatUp = pApp->GetProfileInt(strSec, _T("FORMAT_UP"), TRUE);
	m_strSearchUp = pApp->GetProfileString(strSec, _T("SEARCH_UP"), _T("(.*)(\\r?)(\\n?)"));
	m_strPreplaceUp = pApp->GetProfileString(strSec, _T("REPLACE_UP"), _T("[UP  ]<-$1$2$3"));
	int iMaxLine = pApp->GetProfileInt(strSec, _T("MAX_LINE"), 100);

	m_strMaxLine.Format(_T("%d"),iMaxLine);
	UpdateData(FALSE);
	m_bChange = FALSE;
}


void CPropPageLogView::SaveProfileApp(CWinApp* pApp)
{
	CString strSec = _T("LOG_VIEW");
	UpdateData(TRUE);
	int iMaxLine = _ttoi(m_strMaxLine.GetBuffer());
	pApp->WriteProfileInt(strSec, _T("FORMAT_DOWN"), m_bFormatDown);
	pApp->WriteProfileString(strSec, _T("SEARCH_DOWN"), m_strSearchDown);
	pApp->WriteProfileString(strSec, _T("REPLACE_DOWN"), m_strReplaceDown);
	pApp->WriteProfileInt(strSec, _T("FORMAT_UP"), m_bFormatUp);
	pApp->WriteProfileString(strSec, _T("SEARCH_UP"), m_strSearchUp);
	pApp->WriteProfileString(strSec, _T("REPLACE_UP"), m_strPreplaceUp);
	pApp->WriteProfileInt(strSec, _T("MAX_LINE"), iMaxLine);
	m_bChange = FALSE;
}



BOOL CPropPageLogView::OnInitDialog()
{
	CProfileableDlg::OnInitDialog();

	// TODO: ここに初期化を追加してください
	m_ctlMaxLine.SetRange(1,1000);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CPropPageLogView::OnEnChangeEditLogViewSerchDown()
{
	m_bChange = TRUE;
}


void CPropPageLogView::OnEnChangeEditLogViewFormingDown()
{
	m_bChange = TRUE;
}


void CPropPageLogView::OnBnClickedCheckLogViewFormingUp()
{
	m_bChange = TRUE;
}


void CPropPageLogView::OnEnChangeEditLogViewSerchUp()
{
	m_bChange = TRUE;
}


void CPropPageLogView::OnEnChangeEditLogViewFormingUp()
{
	m_bChange = TRUE;
}


void CPropPageLogView::OnEnChangeEditMaxLine()
{
	m_bChange = TRUE;
}
