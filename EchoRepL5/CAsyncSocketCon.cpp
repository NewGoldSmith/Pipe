#include "CAsyncSocketCon.h"


void CAsyncSocketCon::OnAccept(int nErrorCode)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CAsyncSocket::OnAccept(nErrorCode);
}


void CAsyncSocketCon::OnClose(int nErrorCode)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CAsyncSocket::OnClose(nErrorCode);
}


void CAsyncSocketCon::OnConnect(int nErrorCode)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CAsyncSocket::OnConnect(nErrorCode);
}


void CAsyncSocketCon::OnReceive(int nErrorCode)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CAsyncSocket::OnReceive(nErrorCode);
}


void CAsyncSocketCon::OnSend(int nErrorCode)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CAsyncSocket::OnSend(nErrorCode);
}
