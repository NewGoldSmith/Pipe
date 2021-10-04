#pragma once
#include "CProfileableDlg.h"
#include "resource.h"
// CPropPageChild ダイアログ

class CPropPageChild : public CProfileableDlg
{
	DECLARE_DYNAMIC(CPropPageChild)

public:
	CPropPageChild(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CPropPageChild();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_CHILD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	void LoadProfileApp(CWinApp* pApp);
	void SaveProfileApp(CWinApp* pApp);

protected:
	BOOL m_bAddtionalCommandOptionFront;
	CString m_strAddtionalCommandOptionFront;
	BOOL m_bAddtionalCommandOptionBack;
	CString m_strAddtionalCommandOptionBack;
	BOOL m_bIgnoreParentCommandOption;
public:
	afx_msg void OnChangeEditAddtionalCommandOptionFront();
	afx_msg void OnChangeEditAddtionalCommandOptionBack();
	afx_msg void OnClickedCheckUseAddtionalCommandOptionFront();
	afx_msg void OnClickedCheckUseAddtionalCommandOptionBack();
	afx_msg void OnClickedCheckIgnoreParentCommandOption();
	afx_msg void OnBnClickedButtonCommandPreview();
protected:
	CString m_strCommandPrev;
public:
//	afx_msg void OnBnClickedButtonDoExecuteTest();
protected:
//	ExeEngine m_ExeEngine;
};
