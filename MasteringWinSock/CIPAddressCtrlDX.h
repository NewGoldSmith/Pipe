#pragma once


// CIPAddressCtrlDX

class CIPAddressCtrlDX : public CIPAddressCtrl
{
	DECLARE_DYNAMIC(CIPAddressCtrlDX)

public:
	CIPAddressCtrlDX();
	virtual ~CIPAddressCtrlDX();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CString GetIPAddressString();
};


