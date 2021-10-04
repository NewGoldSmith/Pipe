// CPropPageLogFile.cpp : 実装ファイル
//

#include "pch.h"
#include "PipeProbeSettingApp.h"
#include "CPropPageLogFile.h"
#include "afxdialogex.h"
#include "CProfileableDlg.h"



// CPropPageLogFile ダイアログ

IMPLEMENT_DYNAMIC(CPropPageLogFile, CProfileableDlg)

CPropPageLogFile::CPropPageLogFile(CWnd* pParent /*=nullptr*/)
	: CProfileableDlg(IDD_PROPPAGE_LOG_FILE, pParent)

	, m_strLogDir(_T(""))
	, m_bFormatDown(FALSE)
	, m_strSearchDown(_T(""))
	, m_strReplaceDown(_T(""))
	, m_bFormatUp(FALSE)
	, m_strSearchUp(_T(""))
	, m_strReplaceUp(_T(""))
	, m_bWriteCommand(FALSE)
	, m_strNamePrev(_T(""))
	, m_bMakeLogFile(FALSE)
	, m_strNameFormat(_T(""))
{
	;
}

CPropPageLogFile::~CPropPageLogFile()
{
}

void CPropPageLogFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_LOG_DIR, m_strLogDir);
	DDX_Check(pDX, IDC_CHECK_LOG_FORMAT_DOWN, m_bFormatDown);
	DDX_Text(pDX, IDC_EDIT_LOG_SEARCH_DOWN, m_strSearchDown);
	DDX_Text(pDX, IDC_EDIT_LOG_REPLACE_DOWN, m_strReplaceDown);
	DDX_Check(pDX, IDC_CHECK_LOG_FORMAT_UP, m_bFormatUp);
	DDX_Text(pDX, IDC_EDIT_LOG_SEARCH_UP, m_strSearchUp);
	DDX_Text(pDX, IDC_EDIT_LOG_REPLACE_UP, m_strReplaceUp);
	DDX_Check(pDX, IDC_CHECK_LOG_WRITE_COMMAND, m_bWriteCommand);
	DDX_Text(pDX, IDC_STATIC_NAME_PREVIEW, m_strNamePrev);
	DDX_Check(pDX, IDC_CHECK_CREATE_LOG, m_bMakeLogFile);
	DDX_Text(pDX, IDC_EDIT_LOG_NAME_FORMAT, m_strNameFormat);
}


BEGIN_MESSAGE_MAP(CPropPageLogFile, CProfileableDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_LOG_DIR, &CPropPageLogFile::OnBnClickedButtonSelectLogDir)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_FILENAME, &CPropPageLogFile::OnBnClickedButtonMakeFilename)
	ON_BN_CLICKED(IDC_CHECK_LOG_FORMAT_DOWN, &CPropPageLogFile::OnBnClickedCheckLogFormatDown)
	ON_EN_CHANGE(IDC_EDIT_LOG_SEARCH_DOWN, &CPropPageLogFile::OnEnChangeEditLogSearchDown)
	ON_EN_CHANGE(IDC_EDIT_LOG_REPLACE_DOWN, &CPropPageLogFile::OnEnChangeEditLogReplaceDown)
	ON_BN_CLICKED(IDC_CHECK_LOG_FORMAT_UP, &CPropPageLogFile::OnBnClickedCheckLogFormatUp)
	ON_EN_CHANGE(IDC_EDIT_LOG_SEARCH_UP, &CPropPageLogFile::OnEnChangeEditLogSearchUp)
	ON_EN_CHANGE(IDC_EDIT_LOG_REPLACE_UP, &CPropPageLogFile::OnEnChangeEditLogReplaceUp)
	ON_EN_CHANGE(IDC_EDIT_LOG_NAME_FORMAT, &CPropPageLogFile::OnEnChangeEditLogNameFormat)
	ON_BN_CLICKED(IDC_CHECK_LOG_WRITE_COMMAND, &CPropPageLogFile::OnBnClickedCheckLogWriteCommand)
END_MESSAGE_MAP()


// CPropPageLogFile メッセージ ハンドラー


void CPropPageLogFile::LoadProfileApp(CWinApp* pApp)
{
	CString strSec = _T("LOG_FILE");
	m_strLogDir = pApp->GetProfileString(strSec, _T("DIR"), _T(""));
	m_bFormatDown = pApp->GetProfileInt(strSec, _T("FORMAT_DOWN"), TRUE);
	m_strSearchDown = pApp->GetProfileString(strSec, _T("SEARCH_DOWN"), _T("(.*)(\\r?)(\\n?)"));
	m_strReplaceDown = pApp->GetProfileString(strSec, _T("REPLACE_DOWN"), _T("[DOWN]->$1$2$3"));
	m_bFormatUp = pApp->GetProfileInt(strSec, _T("FORMAT_UP"), TRUE);
	m_strSearchUp = pApp->GetProfileString(strSec, _T("SEARCH_UP"), _T("(.*)(\\r?)(\\n?)"));
	m_strReplaceUp = pApp->GetProfileString(strSec, _T("REPLACE_UP"), _T("[UP  ]<-$1$2$3"));
	m_bWriteCommand = pApp->GetProfileInt(strSec, _T("LEAD_WRITE_COMMAND"), 0);
	m_bMakeLogFile = pApp->GetProfileInt(strSec, _T("CREATE_LOG"), 0);
	m_strNameFormat = pApp->GetProfileString(strSec, _T("NAME_FORMAT"), _T("Pipe%y-%m-%d-%H-%M-%S?:.log"));
	UpdateData(FALSE);
}


void CPropPageLogFile::SaveProfileApp(CWinApp* pApp)
{
	CString strSec = _T("LOG_FILE");
	UpdateData(TRUE);
	pApp->WriteProfileString(strSec, _T("DIR"), m_strLogDir);
	pApp->WriteProfileInt(strSec, _T("FORMAT_DOWN"), m_bFormatDown);
	pApp->WriteProfileString(strSec, _T("SEARCH_DOWN"), m_strSearchDown);
	pApp->WriteProfileString(strSec, _T("REPLACE_DOWN"), m_strReplaceDown);
	pApp->WriteProfileInt(strSec, _T("FORMAT_UP"), m_bFormatUp);
	pApp->WriteProfileString(strSec, _T("SEARCH_UP"), m_strSearchUp);
	pApp->WriteProfileString(strSec, _T("REPLACE_UP"), m_strReplaceUp);
	pApp->WriteProfileInt(strSec, _T("LEAD_WRITE_COMMAND"), m_bWriteCommand);
	pApp->WriteProfileInt(strSec, _T("CREATE_LOG"), m_bMakeLogFile);
	pApp->WriteProfileString(strSec, _T("NAME_FORMAT"), m_strNameFormat);
	if (m_bMakeLogFile == TRUE && m_strLogDir == _T(""))
	{
		AfxMessageBox(_T("ログファイルディレクトリが設定されていません。\r\nログファイルは作成されません。"), MB_ICONASTERISK | MB_OK);
	}
	m_bChange = FALSE;
}

void CPropPageLogFile::OnBnClickedButtonSelectLogDir()
{
	CFolderPickerDialog* pDlg = new CFolderPickerDialog(m_strLogDir, 0,this,0);

	if (pDlg->DoModal() == IDOK)
	{
		m_strLogDir = pDlg->GetPathName(); // return full path and filename
		m_bChange = TRUE;
	}
	UpdateData(FALSE);
}



void CPropPageLogFile::OnBnClickedButtonMakeFilename()
{
	USES_CONVERSION;
	UpdateData(TRUE);
	time_t osBinaryTime;  // C run-time time (defined in <time.h>)
	time(&osBinaryTime);
	CTime ctime(osBinaryTime);
	m_strNamePrev=ctime.Format(m_strNameFormat);
	LPWSTR pwStr = T2W(m_strNamePrev.GetBuffer());
	m_strNamePrev.ReleaseBuffer();
	int i = PathCleanupSpec(NULL,pwStr);
	m_strNamePrev = W2T(pwStr);
	UpdateData(FALSE);
}


void CPropPageLogFile::OnBnClickedCheckLogFormatDown()
{
	m_bChange = TRUE;
}


void CPropPageLogFile::OnEnChangeEditLogSearchDown()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CProfileableDlg::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	m_bChange = TRUE;
}


void CPropPageLogFile::OnEnChangeEditLogReplaceDown()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CProfileableDlg::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	m_bChange = TRUE;
}


void CPropPageLogFile::OnBnClickedCheckLogFormatUp()
{
	m_bChange = TRUE;
}


void CPropPageLogFile::OnEnChangeEditLogSearchUp()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CProfileableDlg::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	m_bChange = TRUE;
}


void CPropPageLogFile::OnEnChangeEditLogReplaceUp()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CProfileableDlg::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	m_bChange = TRUE;
}


void CPropPageLogFile::OnEnChangeEditLogNameFormat()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CProfileableDlg::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	m_bChange = TRUE;
}


void CPropPageLogFile::OnBnClickedCheckLogWriteCommand()
{
	m_bChange = TRUE;
}
