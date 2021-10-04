#pragma once
#include "CProfileableDlg.h"
#include "resource.h"

// CPropPageGenelal ダイアログ

class CPropPageGenelal : public CProfileableDlg
{
	DECLARE_DYNAMIC(CPropPageGenelal)

public:
	CPropPageGenelal(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CPropPageGenelal();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_GENELAL};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonChildSelect();
	void LoadProfileApp(CWinApp* pApp);
	void SaveProfileApp(CWinApp* pApp);
//	CString m_strPipeProbeDistinationPathName;
protected:
	CString m_strChildPathName;
	CString m_strPipeprobeSourcePathname;
	afx_msg void OnBnClickedButtonSelectSourcePipeprobePathname();
//	afx_msg void OnBnClickedButtonCopyPipeprobePathname();
public:
	CString GetDestinationPathName();
protected:
//	int m_radStartupShow;
public:
//	afx_msg void OnBnHotItemChangeRadioStartupShow(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnBnClickedButton1();
protected:
//	CButton m_radStartupShow;
	CButton m_radStartupIcon;
	CButton m_radStartupTasktray;
public:
	afx_msg void OnClickedRadioStartupWindow();
	afx_msg void OnClickedRadioStartupIcon();
	afx_msg void OnClickedRadioStartupTasktray();
protected:
	CButton m_radStartupWindow;
public:
	int GetMainWindowShowStatus();
	int SetMainWindowShowStatus(int iStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ErrDlg(LPTSTR lpszFunction);
	CString GetSourcePathName();
//	afx_msg void OnBnClickedButtonGenelalExecuteCopy();
	CString GetChildPathName();
};
