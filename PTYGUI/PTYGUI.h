
// PTYGUI.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CPTYGUIApp:
// このクラスの実装については、PTYGUI.cpp を参照してください
//

class CPTYGUIApp : public CWinApp
{
public:
	CPTYGUIApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CPTYGUIApp theApp;
