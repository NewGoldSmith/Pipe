
// WCmdCliantDlgMain.h : ヘッダー ファイル
//

#pragma once

#include "WCmdCLiantOption.h"
#include "resource.h"
namespace WCmdCLiant {
//#define WM_THREAD_END	WM_USER +1
//#define WM_TASKTRAY		WM_USER + 2
//#define BUFSIZE 4096
	// CWCmdCliantMainDlg ダイアログ
	class CWCmdCliantMainDlg : public CDialogEx
	{
	protected:
		enum WM_MESSAGE{WM_Null=WM_NULL, WM_THREAD_END =WM_USER+1, WM_TASKTRAY =WM_USER+2};
		enum eParam{BUFSIZE=4096};
		// コンストラクション
	public:
		CWCmdCliantMainDlg(CWnd* pParent = nullptr);	// 標準コンストラクター
		UINT WM_BEFORE;
	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_WCMDCLIANT_DIALOG };
#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


	// 実装
	protected:
		HICON m_hIcon;
		
		// 生成された、メッセージ割り当て関数
		virtual BOOL OnInitDialog();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	protected:
		BOOL m_b_is_hide;
		void MainWndShow();
		void MainWndIcon();
		void MainWndHide();
		static UINT SubThreadProc(LPVOID pParam);
		struct THREAD_PARAM_WCOMMAND_CLIANT
		{
			CWnd* pWnd;
			HANDLE hInput;
			HANDLE hOutput;
			HANDLE hEvent;
			HANDLE hThread;
			DWORD WM_MESSAGE;
			DWORD ID_THREAD;
			char* pBuf;
			size_t Bufsize;
			DWORD dwRead, dwWritten;
			BOOL bContinue;
			BOOL bSuccess;
		};
		THREAD_PARAM_WCOMMAND_CLIANT m_TParam;

		afx_msg LRESULT OnThreadEnd(WPARAM wParam, LPARAM lParam);
		char m_Buf[BUFSIZE];
		void DisConnect();
		WCmdCLiantOption m_CCOp;
	public:
		afx_msg void OnClose();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		afx_msg void OnDestroy();
		BOOL TrayExecute(DWORD dwCommand);
	protected:
		afx_msg LRESULT OnTasktray(WPARAM wParam, LPARAM lParam);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		HWND m_hConsole;
	};
}
