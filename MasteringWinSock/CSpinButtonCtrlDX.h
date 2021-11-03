#pragma once
#include "CIPAddressCtrlDX.h"

// CSpinButtonCtrlDX

class CSpinButtonCtrlDX : public CSpinButtonCtrl
{
	DECLARE_DYNAMIC(CSpinButtonCtrlDX)

public:
	CSpinButtonCtrlDX();
	virtual ~CSpinButtonCtrlDX();
	DWORD Init(DWORD dwPos);

protected:
	DECLARE_MESSAGE_MAP()
	CIPAddressCtrlDX* m_pCIPAddressCtrlDX;
	CWnd* SelectReturnWndPointer();
//	CEdit* m_pCEdit;
	void SetNullAllPointer();
public:
	CWnd* SetBuddy(CWnd* pWnd);
	CWnd* SetBuddy(CIPAddressCtrlDX* pWnd);
	afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
};


