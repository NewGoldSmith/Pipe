#pragma once
#include "CAsyncSocketDX.h"

// CListCtrlForSocket

class CListCtrlForSocket : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlForSocket)

public:
enum class UserErrCode{Undef,NotSelected,NoFindChecked,NoFindCheckedAndNotSelected};
	CListCtrlForSocket();
	virtual ~CListCtrlForSocket();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL AddSocket(CAsyncSocketDX* pSocket);
	CAsyncSocketDX* PullSocketID(UINT uiID);
	int FindID(UINT ID);
public:
	int DeleteSelectedSocket();
	void AllSocketDelete();
	void Init();
	CAsyncSocketDX* GetSelectedSocket();
	int UpDateView();
protected:
	BOOL UpDateSocketView(int Index);
	UserErrCode m_eUserErr;
public:
	void ErrMessageBox();
protected:
	CString CodeToString(int ErrCode);
	int m_Err;
public:
	CAsyncSocketDX* PullSelectedSocket();
	CAsyncSocketDX* GetSocketID(UINT ID);
	CAsyncSocketDX* GetFirstCheckedSocket();
	CAsyncSocketDX* PullFirstCheckedSocket();
	int DeleteFirstCheckSocket();
	CAsyncSocketDX* GetFirstCheckedOrSelectedSocket();
	CAsyncSocketDX* PullFirstCheckedOrSelectedSocket();
};


