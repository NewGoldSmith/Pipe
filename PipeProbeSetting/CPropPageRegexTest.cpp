// CPropPageRegexTest.cpp : 実装ファイル
//

#include "pch.h"
#include "PipeProbeSettingApp.h"
#include "CPropPageRegexTest.h"
#include "afxdialogex.h"
#include "CProfileableDlg.h"
#include "..\RegexLib\Regex.h"


// CPropPageRegexTest ダイアログ

IMPLEMENT_DYNAMIC(CPropPageRegexTest, CProfileableDlg)

CPropPageRegexTest::CPropPageRegexTest(CWnd* pParent /*=nullptr*/)
	: CProfileableDlg(IDD_PROPPAGE_REGEX_TEST, pParent)
	, m_strSource(_T(""))
	, m_strSearch(_T(""))
	, m_strReplace(_T(""))
	, m_strResult(_T(""))
{

}

CPropPageRegexTest::~CPropPageRegexTest()
{
}

void CPropPageRegexTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REGEX_SOURCE_STRING, m_strSource);
	DDX_Text(pDX, IDC_EDIT_REGEX_SEARCH_STRING, m_strSearch);
	DDX_Text(pDX, IDC_EDIT_REGEX_REPLACE_STRING, m_strReplace);
	DDX_Text(pDX, IDC_EDIT_REGEX_RESULT_STRING, m_strResult);
}


BEGIN_MESSAGE_MAP(CPropPageRegexTest, CProfileableDlg)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE_REGEX, &CPropPageRegexTest::OnBnClickedButtonExecuteRegex)
END_MESSAGE_MAP()


// CPropPageRegexTest メッセージ ハンドラー


void CPropPageRegexTest::LoadProfileApp(CWinApp* pApp)
{
	CString strSec=_T("REGEX_TEST");
	CString str;
	str = pApp->GetProfileString(strSec, _T("SOURCE"), _T("test"));
	m_strSearch = pApp->GetProfileString(strSec, _T("SEARCH"), _T("(.*)(\\r?)(\\n?)"));
	m_strReplace = pApp->GetProfileString(strSec, _T("REPLACE"), _T("[DOWN]->$1$2$3"));
	m_strSource = m_Regex.TranslateEscape(str);
	UpdateData(FALSE);

}


void CPropPageRegexTest::SaveProfileApp(CWinApp* pApp)
{
	CString strSec = _T("REGEX_TEST");
	UpdateData(TRUE);
	CString str = m_Regex.InvTranslateEscape(m_strSource);
	pApp->WriteProfileString(strSec, _T("SOURCE"), str);
	pApp->WriteProfileString(strSec, _T("SEARCH"), m_strSearch);
	pApp->WriteProfileString(strSec, _T("REPLACE"), m_strReplace);
	m_bChange = FALSE;
}


void CPropPageRegexTest::OnBnClickedButtonExecuteRegex()
{
	UpdateData(TRUE);
	m_Regex.TranslateEscape(m_strSearch);
	m_Regex.TranslateEscape(m_strReplace);
	m_strResult  =m_Regex.ExecuteRegex(m_strSource, m_strSearch, m_strReplace);
	UpdateData(FALSE);
}


BOOL CPropPageRegexTest::PreTranslateMessage(MSG* pMsg)
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
			pWnd = GetDlgItem(IDC_EDIT_REGEX_SOURCE_STRING);
			if (pWnd->m_hWnd == pMsg->hwnd)
			{
					TranslateMessage(pMsg);
					DispatchMessage(pMsg);
					return TRUE;
			}
			break;
		default:
			break;
		}
	default:
		break;
	}
	return CProfileableDlg::PreTranslateMessage(pMsg);
}


BOOL CPropPageRegexTest::OnInitDialog()
{
	CProfileableDlg::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
