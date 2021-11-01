// CListCtrlForSocket.cpp : 実装ファイル
//

#include "pch.h"
#include "CListCtrlForSocket.h"


// CListCtrlForSocket

IMPLEMENT_DYNAMIC(CListCtrlForSocket, CListCtrl)

CListCtrlForSocket::CListCtrlForSocket()
{
	m_eUserErr = UserErrCode::Undef;
	m_Err = 0;
}

CListCtrlForSocket::~CListCtrlForSocket()
{
}


BEGIN_MESSAGE_MAP(CListCtrlForSocket, CListCtrl)
END_MESSAGE_MAP()



// CListCtrlForSocket メッセージ ハンドラー




BOOL CListCtrlForSocket::AddSocket(CAsyncSocketDX* pSocket)
{
	int ID = pSocket->GetID();

	TCHAR strID[256] = {};
	_itot_s(ID, strID, 256, 10);
	int Index = InsertItem(0, strID);
	SetItemData(Index, (DWORD_PTR)pSocket);

	SetItemState(Index, LVNI_SELECTED, LVIS_SELECTED);
	UpDateSocketView(Index);
	return TRUE;
}


CAsyncSocketDX* CListCtrlForSocket::PullSocketID(UINT uiID)
{
	int Index = FindID(uiID);
	if (Index == -1)
	{
		return NULL;
	}
	CAsyncSocketDX * pSocket= (CAsyncSocketDX*)GetItemData(Index);
	DeleteItem(Index);
	return pSocket;
}

int CListCtrlForSocket::FindID(UINT ID)
{
	int ItemCount = GetItemCount();
	int Index = 0;
	CString str;
	CString FindStr;
	FindStr.Format(_T("%d"), ID);
	while (Index < ItemCount)
	{
		str = GetItemText(Index, 0);
		if (FindStr == str)
		{
			return Index;
		}
		Index++;
	}
	return -1;
}


int CListCtrlForSocket::DeleteSelectedSocket()
{
	int Index = GetNextItem(-1, LVNI_SELECTED);
	if (Index == -1)
	{
		m_eUserErr = UserErrCode::NotSelected;
		return -1;
	}
	CAsyncSocketDX* pSocket = (CAsyncSocketDX*)GetItemData(Index);
	delete pSocket;
	DeleteItem(Index);
	return Index;
}


void CListCtrlForSocket::AllSocketDelete()
{
	int nItem;
	nItem = GetItemCount();
	while (nItem > 0)
	{
		CAsyncSocketDX* pSocket = (CAsyncSocketDX*)GetItemData(0);
		delete pSocket;
		DeleteItem(0);
		nItem--;
	}
}



void CListCtrlForSocket::Init()
{
	InsertColumn(0, _T("ID   "));
	InsertColumn(1, _T("IP Address     "));
	InsertColumn(2, _T("Port   "));
	InsertColumn(3, _T("Peer IP Address"));
	InsertColumn(4, _T("Port   "));
	InsertColumn(5, _T("Handle"));
	InsertColumn(6, _T("Context"));
	int i = 0;
	while (i < 8)
	{
		SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		i++;
	}
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
}


CAsyncSocketDX* CListCtrlForSocket::GetSelectedSocket()
{
	int Index = GetNextItem(-1, LVNI_SELECTED);
	if (Index == -1)
	{
		m_eUserErr = UserErrCode::NotSelected;
		return NULL;
	}
	return (CAsyncSocketDX *)GetItemData(Index);
}


int CListCtrlForSocket::UpDateView()
{
	int Count = GetItemCount();
	int i = 0;
	while (i < Count)
	{
		UpDateSocketView(i);
		i++;
	}
	return 0;
}


BOOL CListCtrlForSocket::UpDateSocketView(int Index)
{
	CAsyncSocketDX* pSocket = (CAsyncSocketDX * )GetItemData(Index);

	int SubItem = 0;
	//ID取得
	TCHAR strID[256] = {};
	int ID = pSocket->GetID();
	_itot_s(ID, strID, 256, 10);
	SetItemText(Index, SubItem++, strID);
	//ソックネーム、ソックポート取得
	CString strAddress;
	UINT uiPort;
	pSocket->GetSockName(strAddress, uiPort);
	SetItemText(Index, SubItem++, strAddress);
	TCHAR strPort[256] = {};
	_itot_s(uiPort, strPort, 256, 10);
	SetItemText(Index, SubItem++, strPort);
	//ピアネーム、ピアポート取得
	CString strPeerName;
	UINT uiPeerPort = 0;
	TCHAR strPeerPort[256] = {};
	pSocket->GetPeerName(strPeerName, uiPeerPort);
	SetItemText(Index, SubItem++, strPeerName);
	_itot_s(uiPeerPort, strPeerPort, 256, 10);
	SetItemText(Index, SubItem++, strPeerPort);
	//ハンドル取得
	TCHAR strHandle[256] = {};
	_itot_s(pSocket->m_hSocket, strHandle, 10);
	SetItemText(Index, SubItem++, strHandle);
	//ソケット属性取得
	SetItemText(Index, SubItem++, pSocket->GetConTextString());
	return TRUE;
}



void CListCtrlForSocket::ErrMessageBox()
{
	CString str;
	str.Format(_T("%s\r\n%s"), _T("CListCtrlForSocket"),CodeToString(m_Err));
	AfxMessageBox(str, MB_OK | MB_ICONASTERISK);
}


CString CListCtrlForSocket::CodeToString(int ErrCode)
{
	CString str;
	switch (m_eUserErr)
	{
	case UserErrCode::NotSelected:
		return _T("選択がされていません。");
	case UserErrCode::NoFindChecked:
		return _T("チェックがされていません。");
	case UserErrCode::NoFindCheckedAndNotSelected:
		return _T("チェックか選択がされていません。");
	}

	str.Format(_T("その他のエラー　%d"), ErrCode);
	return str;
}


CAsyncSocketDX* CListCtrlForSocket::PullSelectedSocket()
{
	int Index = GetNextItem(-1, LVNI_SELECTED);
	if (Index == -1)
	{
		m_eUserErr = UserErrCode::NotSelected;
		return NULL;
	}
	CAsyncSocketDX * pSocket= (CAsyncSocketDX*)GetItemData(Index);
	DeleteItem(Index);
	return pSocket;
}


CAsyncSocketDX* CListCtrlForSocket::GetSocketID(UINT uiID)
{
	int Index = FindID(uiID);
	if (Index == -1)
	{
		return NULL;
	}
	CAsyncSocketDX* pSocket = (CAsyncSocketDX*)GetItemData(Index);
	return pSocket;
}


CAsyncSocketDX* CListCtrlForSocket::GetFirstCheckedSocket()
{
	CAsyncSocketDX* pSocket = NULL;
	int Num = 0;
	int Index = 0;
	BOOL bCheck = FALSE;
	Num = GetItemCount();
	while (Index < Num)
	{
		if (GetCheck(Index))
		{
			return (CAsyncSocketDX *)GetItemData(Index);
		}
		Index++;
	}
	if (Index == Num)
	{
		m_eUserErr = UserErrCode::NoFindChecked;
	}
	return nullptr;
}


CAsyncSocketDX* CListCtrlForSocket::PullFirstCheckedSocket()
{
	CAsyncSocketDX* pSocket = NULL;
	int Num = 0;
	int Index = 0;
	BOOL bCheck = FALSE;
	Num = GetItemCount();
	while (Index < Num)
	{
		if (GetCheck(Index))
		{
			pSocket= (CAsyncSocketDX*)GetItemData(Index);
			DeleteItem(Index);
			break;
		}
		Index++;
	}
	if (Index == Num)
	{
		m_eUserErr = UserErrCode::NoFindChecked;
	}
	return pSocket;
}


int CListCtrlForSocket::DeleteFirstCheckSocket()
{
	CAsyncSocketDX* pSocket = NULL;
	int Num = 0;
	int Index = 0;
	Num = GetItemCount();
	while (Index < Num)
	{
		if (GetCheck(Index))
		{
			pSocket = (CAsyncSocketDX*)GetItemData(Index);
			delete pSocket;
			DeleteItem(Index);
			break;
		}
		Index++;
	}
	if (Index == Num)
	{
		m_eUserErr = UserErrCode::NoFindChecked;
		return -1;
	}
	return Index;
}


CAsyncSocketDX* CListCtrlForSocket::GetFirstCheckedOrSelectedSocket()
{
	CAsyncSocketDX* pSocket = NULL;
	pSocket = GetFirstCheckedSocket();
	if (pSocket == NULL)
	{
		pSocket = GetSelectedSocket();
	}
	if (pSocket==NULL)
	{
		m_eUserErr = UserErrCode::NoFindCheckedAndNotSelected;
	}
	return pSocket;
}


CAsyncSocketDX* CListCtrlForSocket::PullFirstCheckedOrSelectedSocket()
{
	CAsyncSocketDX* pSocket = NULL;
	pSocket = PullFirstCheckedSocket();
	if (pSocket == NULL)
	{
		pSocket = PullSelectedSocket();
	}
	if (pSocket == NULL)
	{
		m_eUserErr = UserErrCode::NoFindCheckedAndNotSelected;
	}
	return pSocket;
}
