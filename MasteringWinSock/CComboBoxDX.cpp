// CComboBoxDX.cpp : 実装ファイル
//

#include "pch.h"
#include "MasteringWinSock.h"
#include "CComboBoxDX.h"


// CComboBoxDX

IMPLEMENT_DYNAMIC(CComboBoxDX, CComboBox)

CComboBoxDX::CComboBoxDX()
{

}

CComboBoxDX::~CComboBoxDX()
{
}


BEGIN_MESSAGE_MAP(CComboBoxDX, CComboBox)
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CComboBoxDX メッセージ ハンドラー




void CComboBoxDX::Init(CButton * pWnd)
{
	m_EditSub.SetButton(pWnd);
}


void CComboBoxDX::GetWindowText(CString& rString)
{
	CComboBox::GetWindowText(rString);
	int Index = FindStringExact(0, rString);
	if (Index == CB_ERR )
	{
		int num = GetCount();
		if (num > 10)
		{
			DeleteString(num);
		}
		if (rString != _T("")) 
		{
			AddString(rString);
		}
	}
	return;
}


void CComboBoxDX::SetWindowText(LPCTSTR lpszString)
{
	CComboBox::SetWindowText(lpszString);
}

void CComboBoxDX::LoadProfile(CString strProfilePathName, CString strSec)
{
	CWinApp *pApp = AfxGetApp();
	LPCTSTR pstrProfileOld = pApp->m_pszProfileName;
	pApp->m_pszProfileName = strProfilePathName.GetBuffer();
	CString strContent;

	strContent = pApp->GetProfileString(strSec, _T("EDIT"), _T(""));
	CComboBoxDX::SetWindowText(strContent);

	CString strKey;
	CString sufix = _T("LIST");
	TCHAR strIndex[256];
	int Index = 0;
	_itot_s(Index, strIndex, 256, 10);
	strKey = sufix + strIndex;
	strContent = pApp->GetProfileString(strSec, strKey, _T(""));
	AddString(strContent);
	Index++;
	while (Index < 10)
	{
		_itot_s(Index, strIndex, 256, 10);
		strKey = sufix + strIndex;
		strContent = pApp->GetProfileString(strSec, strKey, _T(""));
		if (strContent != _T(""))
		{
			AddString(strContent);
		}
		Index++;
	}
	strProfilePathName.ReleaseBuffer();
	pApp->m_pszProfileName = pstrProfileOld;
}


void CComboBoxDX::SaveProfile(CString strProfilePathName, CString strSec)
{
	CWinApp* pApp = AfxGetApp();
	LPCTSTR pstrProfileOld = pApp->m_pszProfileName;
	pApp->m_pszProfileName = strProfilePathName.GetBuffer();

	CString strContent;
	GetWindowText(strContent);
	pApp->WriteProfileString(strSec, _T("EDIT"), strContent);

	CString strKey;
	CString sufix = _T("LIST");
	TCHAR strIndex[256];
	int Index = 0;
	_itot_s(Index, strIndex, 256, 10);
	strKey = sufix + strIndex;
	int Count = GetCount();
	while (Index < Count)
	{
		GetLBText(0, strContent);
		pApp->WriteProfileString(strSec, strKey, strContent);
		DeleteString(0);
		Index++;
		_itot_s(Index, strIndex, 256, 10);
		strKey = sufix + strIndex;
	}
	strProfilePathName.ReleaseBuffer();
	pApp->m_pszProfileName = pstrProfileOld;
}


HBRUSH CComboBoxDX::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		//Edit control
		if (m_EditSub.GetSafeHwnd() == NULL)
			m_EditSub.SubclassWindow(pWnd->GetSafeHwnd());
	}
	else if (nCtlColor == CTLCOLOR_LISTBOX)
	{
		//ListBox control
		if (m_ListBoxSub.GetSafeHwnd() == NULL)
			m_ListBoxSub.SubclassWindow(pWnd->GetSafeHwnd());
	}
	m_EditSub.Init(&m_ListBoxSub);
	m_ListBoxSub.Init(&m_EditSub);

	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}


void CComboBoxDX::OnDestroy()
{
	//unsubclass edit and list box before destruction
	if (m_EditSub.GetSafeHwnd() != NULL)
		m_EditSub.UnsubclassWindow();
	if (m_ListBoxSub.GetSafeHwnd() != NULL)
		m_ListBoxSub.UnsubclassWindow();
	CComboBox::OnDestroy();

}
