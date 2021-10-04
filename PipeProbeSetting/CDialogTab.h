#pragma once
#include "CProfileableDlg.h"
#include "CPropPageGenelal.h"
#include "CPropPageLogFile.h"
#include "CPropPageChild.h"
#include "CPropPageLogView.h"
#include "CPropPageRegexTest.h"


// CDialogTab

class CDialogTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CDialogTab)

public:
	CDialogTab();
	virtual ~CDialogTab();

protected:
	DECLARE_MESSAGE_MAP()
public:
	LONG InsertDlg(int i, CProfileableDlg* pDlg,CString *pStr);
	BOOL DeleteDlg(int n);
	afx_msg void OnTcnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateUIState(UINT /*nAction*/, UINT /*nUIElement*/);
	void LoadProfileApp(CWinApp* pApp);
	void SaveProfileApp(CWinApp* pApp);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CWnd* GetFocusBroadCast();
	BOOL IsChanged();
	void SaveProfilesApp(LPCTSTR lpPath);
	void LoadProfilesApp(LPCTSTR lpPath);
};


