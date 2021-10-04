
// PopeProbe.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CPopeProbeApp:
// このクラスの実装については、PopeProbe.cpp を参照してください
//

class CPopeProbeApp : public CWinApp
{
public:
	CPopeProbeApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CPopeProbeApp theApp;
