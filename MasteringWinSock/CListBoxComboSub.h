#pragma once
#include "CEditComboSub.h"
class CEditComboSub;
// CListBoxComboSub

class CListBoxComboSub : public CListBox
{
	DECLARE_DYNAMIC(CListBoxComboSub)

public:
	CListBoxComboSub();
	virtual ~CListBoxComboSub();

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	CEditComboSub* m_pEdit;
public:
	void Init(CEditComboSub* pEdit);
};


