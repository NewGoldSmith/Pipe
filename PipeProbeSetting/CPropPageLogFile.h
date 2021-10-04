#pragma once
#include "CProfileableDlg.h"


// CPropPageLogFile ダイアログ

class CPropPageLogFile : public CProfileableDlg
{
	DECLARE_DYNAMIC(CPropPageLogFile)

public:
	CPropPageLogFile(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CPropPageLogFile();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_LOG_FILE};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	void LoadProfileApp(CWinApp* pApp);
	void SaveProfileApp(CWinApp* pApp);
//	BOOL Changed();

public:
	afx_msg void OnBnClickedButtonSelectLogDir();
protected:
	CString m_strLogDir;
	BOOL m_bFormatDown;
	CString m_strSearchDown;
	CString m_strReplaceDown;
	BOOL m_bFormatUp;
	CString m_strSearchUp;
	CString m_strReplaceUp;
	BOOL m_bWriteCommand;
	CString m_strNamePrev;
	BOOL m_bMakeLogFile;
	CString m_strNameFormat;
public:
	afx_msg void OnBnClickedButtonMakeFilename();
	afx_msg void OnBnClickedCheckLogFormatDown();
	afx_msg void OnEnChangeEditLogSearchDown();
	afx_msg void OnEnChangeEditLogReplaceDown();
	afx_msg void OnBnClickedCheckLogFormatUp();
	afx_msg void OnEnChangeEditLogSearchUp();
	afx_msg void OnEnChangeEditLogReplaceUp();
	afx_msg void OnEnChangeEditLogNameFormat();
	afx_msg void OnBnClickedCheckLogWriteCommand();
};
