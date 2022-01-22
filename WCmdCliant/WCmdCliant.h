
// WCmdCliant.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif
namespace WCmdCLiant {
#include "resource.h"		// メイン シンボル


	// CWCmdCliantApp:
	// このクラスの実装については、WCmdCliant.cpp を参照してください
	//

	class CWCmdCliantApp : public CWinApp
	{
	public:
		CWCmdCliantApp();

		// オーバーライド
	public:
		virtual BOOL InitInstance();

		// 実装

		DECLARE_MESSAGE_MAP()
	};

	extern CWCmdCliantApp theApp;
}