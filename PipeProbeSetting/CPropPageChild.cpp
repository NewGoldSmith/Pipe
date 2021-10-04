// CPropPageChild.cpp : 実装ファイル
//

#include "pch.h"
#include "PipeProbeSettingApp.h"
#include "CPropPageChild.h"
#include "afxdialogex.h"
#include "CProfileableDlg.h"
#include "..\PopeProbe\ExeEngine.h"

// CPropPageChild ダイアログ

IMPLEMENT_DYNAMIC(CPropPageChild, CProfileableDlg)

CPropPageChild::CPropPageChild(CWnd* pParent /*=nullptr*/)
	: CProfileableDlg(IDD_PROPPAGE_CHILD, pParent)
	, m_bAddtionalCommandOptionFront(FALSE)
	, m_strAddtionalCommandOptionFront(_T(""))
	, m_bAddtionalCommandOptionBack(FALSE)
	, m_strAddtionalCommandOptionBack(_T(""))
	, m_bIgnoreParentCommandOption(FALSE)
	, m_strCommandPrev(_T(""))
{

}

CPropPageChild::~CPropPageChild()
{
}

void CPropPageChild::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_USE_ADDTIONAL_COMMAND_OPTION_FRONT, m_bAddtionalCommandOptionFront);
	DDX_Text(pDX, IDC_EDIT_ADDTIONAL_COMMAND_OPTION_FRONT, m_strAddtionalCommandOptionFront);
	DDX_Check(pDX, IDC_CHECK_USE_ADDTIONAL_COMMAND_OPTION_BACK, m_bAddtionalCommandOptionBack);
	DDX_Text(pDX, IDC_EDIT_ADDTIONAL_COMMAND_OPTION_BACK, m_strAddtionalCommandOptionBack);
	DDX_Check(pDX, IDC_CHECK_IGNORE_PARENT_COMMAND_OPTION, m_bIgnoreParentCommandOption);
	DDX_Text(pDX, IDC_STATIC_COMMAND_PREVIEW, m_strCommandPrev);
}


BEGIN_MESSAGE_MAP(CPropPageChild, CProfileableDlg)
	ON_EN_CHANGE(IDC_EDIT_ADDTIONAL_COMMAND_OPTION_FRONT, &CPropPageChild::OnChangeEditAddtionalCommandOptionFront)
	ON_EN_CHANGE(IDC_EDIT_ADDTIONAL_COMMAND_OPTION_BACK, &CPropPageChild::OnChangeEditAddtionalCommandOptionBack)
	ON_BN_CLICKED(IDC_CHECK_USE_ADDTIONAL_COMMAND_OPTION_FRONT, &CPropPageChild::OnClickedCheckUseAddtionalCommandOptionFront)
	ON_BN_CLICKED(IDC_CHECK_USE_ADDTIONAL_COMMAND_OPTION_BACK, &CPropPageChild::OnClickedCheckUseAddtionalCommandOptionBack)
	ON_BN_CLICKED(IDC_CHECK_IGNORE_PARENT_COMMAND_OPTION, &CPropPageChild::OnClickedCheckIgnoreParentCommandOption)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_PREVIEW, &CPropPageChild::OnBnClickedButtonCommandPreview)
END_MESSAGE_MAP()


// CPropPageChild メッセージ ハンドラー


void CPropPageChild::LoadProfileApp(CWinApp* pApp)
{
	CString strSec = _T("CHILD");
	m_bAddtionalCommandOptionFront = pApp->GetProfileInt(strSec, _T("USE_ADDTIONAL_COMMAND_FRONT"), FALSE);
	m_strAddtionalCommandOptionFront = pApp->GetProfileString(strSec, _T("COMMAND_FRONT"), _T(""));
	m_bAddtionalCommandOptionBack = pApp->GetProfileInt(strSec, _T("USE_ADDTIONAL_COMMAND_BACK"), FALSE);
	m_strAddtionalCommandOptionBack = pApp->GetProfileString(strSec, _T("COMMAND_BACK"), _T(""));
	m_bIgnoreParentCommandOption = pApp->GetProfileInt(strSec, _T("IGNORE_PARENT_COMMAND"), FALSE);
	UpdateData(FALSE);
}


void CPropPageChild::SaveProfileApp(CWinApp* pApp)
{
	CString strSec = _T("CHILD");
	UpdateData(TRUE);
	pApp->WriteProfileInt(strSec, _T("USE_ADDTIONAL_COMMAND_FRONT"), m_bAddtionalCommandOptionFront);
	pApp->WriteProfileString(strSec, _T("COMMAND_FRONT"), m_strAddtionalCommandOptionFront);
	pApp->WriteProfileInt(strSec, _T("USE_ADDTIONAL_COMMAND_BACK"), m_bAddtionalCommandOptionBack);
	pApp->WriteProfileString(strSec, _T("COMMAND_BACK"), m_strAddtionalCommandOptionBack);
	pApp->WriteProfileInt(strSec, _T("IGNORE_PARENT_COMMAND"), m_bIgnoreParentCommandOption);
	m_bChange = FALSE;
}


void CPropPageChild::OnChangeEditAddtionalCommandOptionFront()
{
	m_bChange = TRUE;
}


void CPropPageChild::OnChangeEditAddtionalCommandOptionBack()
{
	m_bChange = TRUE;
}


void CPropPageChild::OnClickedCheckUseAddtionalCommandOptionFront()
{
	m_bChange = TRUE;
}


void CPropPageChild::OnClickedCheckUseAddtionalCommandOptionBack()
{
	m_bChange = TRUE;
}


void CPropPageChild::OnClickedCheckIgnoreParentCommandOption()
{
	m_bChange = TRUE;
}


void CPropPageChild::OnBnClickedButtonCommandPreview()
{
	UpdateData(TRUE);
	m_strCommandPrev = _T("child.exe");
	if (m_bAddtionalCommandOptionFront == TRUE)
	{
		m_strCommandPrev += _T(" ")+m_strAddtionalCommandOptionFront  ;
	}
	if (m_bIgnoreParentCommandOption == FALSE)
	{
		m_strCommandPrev += _T(" ") + CString("/ParentOp");
	}
	if (m_bAddtionalCommandOptionBack == TRUE)
	{
		m_strCommandPrev += _T(" ") + m_strAddtionalCommandOptionBack;
	}
	UpdateData(FALSE);
}

