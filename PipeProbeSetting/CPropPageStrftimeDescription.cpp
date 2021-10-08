// CPropPageStrftimeDescription.cpp : 実装ファイル
//

#include "pch.h"
#include "CPropPageStrftimeDescription.h"
#include "afxdialogex.h"


// CPropPageStrftimeDescription ダイアログ

IMPLEMENT_DYNAMIC(CPropPageStrftimeDescription, CDialogEx)

CPropPageStrftimeDescription::CPropPageStrftimeDescription(CWnd* pParent /*=nullptr*/)
	: CProfileableDlg(IDD_PROPPAGE_STRFTIME_DESCRIPTION, pParent)
	, m_strDescription(_T(""))
{

}

CPropPageStrftimeDescription::~CPropPageStrftimeDescription()
{
}

void CPropPageStrftimeDescription::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STRFTIME_DESCRIPTION, m_strDescription);
	DDX_Control(pDX, IDC_EDIT_STRFTIME_DESCRIPTION, m_ctlEditDescription);
}


BEGIN_MESSAGE_MAP(CPropPageStrftimeDescription, CProfileableDlg)
	ON_EN_CHANGE(IDC_EDIT_STRFTIME_DESCRIPTION, &CPropPageStrftimeDescription::OnEnChangeEditStrftimeDescription)
END_MESSAGE_MAP()


// CPropPageStrftimeDescription メッセージ ハンドラー
void CPropPageStrftimeDescription::LoadProfileApp(CWinApp* pApp)
{
	CString strSec = _T("STRFTIME_DESCRIPTION");
	CString str = pApp->GetProfileString(strSec, _T("TEXT"), _T(""));
	UpdateData(FALSE);
	m_bChange = FALSE;
}


void CPropPageStrftimeDescription::SaveProfileApp(CWinApp* pApp)
{
	CString strSec = _T("STRFTIME_DESCRIPTION");
	UpdateData(TRUE);
	Regex reg;
	CString str =reg.InvTranslateEscape(m_strDescription);
//	pApp->WriteProfileString(strSec, _T("TEXT"), str);
	m_bChange = FALSE;
}


BOOL CPropPageStrftimeDescription::OnInitDialog()
{
	CProfileableDlg::OnInitDialog();
	CString str;
	int bRes= str.LoadString(IDS_STRING_STRFTIME_DESCRIPTION);
	Regex reg;
	m_strDescription = reg.TranslateEscape(str);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CPropPageStrftimeDescription::OnEnChangeEditStrftimeDescription()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CProfileableDlg::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
}
