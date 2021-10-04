#pragma once
#include "CProfileableDlg.h"
//#include <regex>
#include "..\RegexLib\Regex.h"

// CPropPageRegexTest ダイアログ

class CPropPageRegexTest : public CProfileableDlg
{
	DECLARE_DYNAMIC(CPropPageRegexTest)

public:
	CPropPageRegexTest(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CPropPageRegexTest();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_REGEX_TEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	void LoadProfileApp(CWinApp* pApp);
	void SaveProfileApp(CWinApp* pApp);
protected:
	CString m_strSource;
	CString m_strSearch;
	CString m_strReplace;
	CString m_strResult;
	afx_msg void OnBnClickedButtonExecuteRegex();
	Regex m_Regex;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
};
