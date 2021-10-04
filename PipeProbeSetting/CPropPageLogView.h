#pragma once
#include "CProfileableDlg.h"
#include "..\RegexLib\Regex.h"

// CPropPageLogView ダイアログ

class CPropPageLogView : public CProfileableDlg
{
	DECLARE_DYNAMIC(CPropPageLogView)

public:
	CPropPageLogView(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CPropPageLogView();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_LOG_VIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	void LoadProfileApp(CWinApp* pApp);
	void SaveProfileApp(CWinApp* pApp);
//	BOOL Changed();
protected:
//	BOOL m_bShowLogWindow;
	BOOL m_bFormatDown;
	CString m_strSearchDown;
	CString m_strReplaceDown;
	BOOL m_bFormatUp;
	CString m_strSearchUp;
	CString m_strPreplaceUp;
	CString m_strMaxLine;
	CSpinButtonCtrl m_ctlMaxLine;
public:
	virtual BOOL OnInitDialog();
//	afx_msg void OnBnClickedCheckShowLogWindow();
	afx_msg void OnEnChangeEditLogViewSerchDown();
	afx_msg void OnEnChangeEditLogViewFormingDown();
	afx_msg void OnBnClickedCheckLogViewFormingUp();
	afx_msg void OnEnChangeEditLogViewSerchUp();
	afx_msg void OnEnChangeEditLogViewFormingUp();
	afx_msg void OnEnChangeEditMaxLine();
protected:
	Regex m_Regex;
};
