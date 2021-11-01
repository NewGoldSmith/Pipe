// CIPAddressCtrlDX.cpp : 実装ファイル
//

#include "pch.h"
#include "CIPAddressCtrlDX.h"


// CIPAddressCtrlDX

IMPLEMENT_DYNAMIC(CIPAddressCtrlDX, CIPAddressCtrl)

CIPAddressCtrlDX::CIPAddressCtrlDX()
{

}

CIPAddressCtrlDX::~CIPAddressCtrlDX()
{
}


BEGIN_MESSAGE_MAP(CIPAddressCtrlDX, CIPAddressCtrl)
END_MESSAGE_MAP()



// CIPAddressCtrlDX メッセージ ハンドラー



CString CIPAddressCtrlDX::GetIPAddressString()
{
	TCHAR strIPAddress[256] = {};
	in_addr i_a;
	DWORD dwAddress = 0;
	if (GetAddress(dwAddress) != 4)
	{
		AfxMessageBox(_T("アドレスが正しく入力されていません。"), MB_OK | MB_ICONHAND);
		return NULL;
	}
	//ネットバイト順をホストバイト順に替える
	i_a.S_un.S_addr = ntohl(dwAddress);
	//ホストIP32ビットをテキスト形式に変換
	InetNtop(AF_INET, &i_a, strIPAddress, 256);
	return CString(strIPAddress);
}

