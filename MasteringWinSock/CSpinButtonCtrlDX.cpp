// CSpinButtonCtrlDX.cpp : 実装ファイル
//

#include "pch.h"
#include "CSpinButtonCtrlDX.h"


// CSpinButtonCtrlDX

IMPLEMENT_DYNAMIC(CSpinButtonCtrlDX, CSpinButtonCtrl)

CSpinButtonCtrlDX::CSpinButtonCtrlDX()
{
	SetNullAllPointer();
}

CSpinButtonCtrlDX::~CSpinButtonCtrlDX()
{
}


BEGIN_MESSAGE_MAP(CSpinButtonCtrlDX, CSpinButtonCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, &CSpinButtonCtrlDX::OnDeltapos)
END_MESSAGE_MAP()



// CSpinButtonCtrlDX メッセージ ハンドラー





DWORD CSpinButtonCtrlDX::Init(DWORD dwPos)
{
	SetBase(10);
//	SetRange32(0xffffffff,0);
	SetPos32(dwPos);
	return dwPos;
}


//CWnd* CSpinButtonCtrlDX::SetBuddy(CIPAddressCtrlDX* pCIPAddressCtrlDX)
//{
//	return CSpinButtonCtrl::SetBuddy(pCIPAddressCtrlDX);
//	//CWnd* pRetWnd = SelectReturnWndPointer();
//	//m_pCIPAddressCtrlDX = pCIPAddressCtrlDX;
//	//return pRetWnd;
//}

//CWnd* CSpinButtonCtrlDX::SetBuddy(CWnd* pWnd)
//{
//	CWnd * pRetWnd =CSpinButtonCtrl::SetBuddy(pWnd);
//	if (pRetWnd == NULL)
//	{
//		CWnd* pRetWnd = SelectReturnWndPointer();
//	}
//	return pRetWnd;
//}


CWnd* CSpinButtonCtrlDX::SelectReturnWndPointer()
{
	CWnd* pRVal=NULL;
	if (m_pCIPAddressCtrlDX != NULL)
	{
		pRVal = m_pCIPAddressCtrlDX;
	}
	else
	{
		pRVal = CSpinButtonCtrl::SetBuddy(NULL);
	}
	SetNullAllPointer();
	return pRVal;
}


void CSpinButtonCtrlDX::SetNullAllPointer()
{
	m_pCIPAddressCtrlDX = NULL;
}


CWnd* CSpinButtonCtrlDX::SetBuddy(CWnd* pWnd)
{
	CWnd * pRWnd = CSpinButtonCtrl::SetBuddy(pWnd);
	if (m_pCIPAddressCtrlDX != NULL)
	{
		pRWnd = m_pCIPAddressCtrlDX;
		m_pCIPAddressCtrlDX = NULL;
	}
	return pRWnd;
}
CWnd* CSpinButtonCtrlDX::SetBuddy(CIPAddressCtrlDX* pWnd)
{
	CWnd* pRWnd=NULL;
	if (pWnd == NULL)
	{
		pRWnd = m_pCIPAddressCtrlDX;
		m_pCIPAddressCtrlDX = NULL;
		return pRWnd;
	}

	if (m_pCIPAddressCtrlDX != NULL)
	{
		pRWnd = m_pCIPAddressCtrlDX;
		m_pCIPAddressCtrlDX = pWnd;
	}
	else {
		m_pCIPAddressCtrlDX = pWnd;
	}
	DWORD dw;
	pWnd->GetAddress(dw);
	SetPos32(dw);
	return pRWnd;
}


void CSpinButtonCtrlDX::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (m_pCIPAddressCtrlDX != NULL)
	{
		DWORD dw;
		m_pCIPAddressCtrlDX->GetAddress(dw);
		dw -= pNMUpDown->iDelta;
		m_pCIPAddressCtrlDX->SetAddress(dw);
	}
	*pResult = 0;
}
