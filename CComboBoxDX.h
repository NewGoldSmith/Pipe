#pragma once
#include "CEditComboSub.h"
#include "CListBoxComboSub.h"

// CComboBoxDX

class CComboBoxDX : public CComboBox
{
	DECLARE_DYNAMIC(CComboBoxDX)

public:
	CComboBoxDX();
	virtual ~CComboBoxDX();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void Init(CButton *pWnd );
	void GetWindowText(CString& rString);
	void SetWindowText(LPCTSTR lpszString);
	void LoadProfile(CString strProfilePathName, CString strSec);
	void SaveProfile(CString strProfilePathName, CString strSec);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	CEditComboSub m_EditSub;
	CListBoxComboSub m_ListBoxSub;
public:
//	virtual BOOL OnCreateAggregates();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
protected:
	CWnd* m_pButton;
};


