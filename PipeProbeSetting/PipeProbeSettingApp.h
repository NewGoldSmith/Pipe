
// PipeProbeSetting.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once
#define BUFSIZE 1024
#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CPipeProbeSettingApp:
// このクラスの実装については、PipeProbeSetting.cpp を参照してください
//

class CPipeProbeSettingApp : public CWinApp
{
public:
	CPipeProbeSettingApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CPipeProbeSettingApp theApp;
