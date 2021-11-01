
// MasteringWinSock.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMasteringWinSockApp:
// このクラスの実装については、MasteringWinSock.cpp を参照してください
//

class CMasteringWinSockApp : public CWinApp
{
public:
	CMasteringWinSockApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CMasteringWinSockApp theApp;
