#pragma once


// CProfileableDlg ダイアログ

class  CProfileableDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProfileableDlg)
public:
	CProfileableDlg(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	CProfileableDlg();

public:
	virtual ~CProfileableDlg();

 //ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	void virtual LoadProfileApp(CWinApp* pApp);
	void virtual SaveProfileApp(CWinApp* pApp);

	virtual BOOL Changed() ;
protected:
	BOOL m_bChange;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
