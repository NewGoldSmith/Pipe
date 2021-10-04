#pragma once
#include "CProfileableDlg.h"
#include "resource.h"
#include "..\RegexLib\Regex.h"


// CPropPageStrftimeDescription ダイアログ

class CPropPageStrftimeDescription : public CProfileableDlg
{
	DECLARE_DYNAMIC(CPropPageStrftimeDescription)

public:
	CPropPageStrftimeDescription(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CPropPageStrftimeDescription();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE_STRFTIME_DESCRIPTION };
#endif
public:
	void LoadProfileApp(CWinApp* pApp);
	void SaveProfileApp(CWinApp* pApp);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	CString m_strDescription;
public:
	virtual BOOL OnInitDialog();
	CEdit m_ctlEditDescription;
	afx_msg void OnEnChangeEditStrftimeDescription();
};
