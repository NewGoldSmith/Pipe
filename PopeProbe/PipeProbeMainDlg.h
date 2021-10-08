
// MainDlg.h : ヘッダー ファイル
//

#pragma once
#include "ExeEngine.h"
#include "Probe.h"
#include "Regex.h"
#include "ProfileLogView.h"
#include "ProfileLogFile.h"
#include "ProfileGenelal.h"
#include "ProfileChild.h"
#include "PipeProbeCommandOption.h"
#include "resource.h"

#define BUF_SIZE 16384
#define WM_USER_EXE_MESSAGE WM_USER + 1
#define WM_USER_READ_DOWN	WM_USER + 2
#define WM_USER_READ_UP		WM_USER + 3
#define WM_TASKTRAY			WM_USER + 4 
#define ID_PROBE_DOWN 0
#define ID_PROBE_UP 1
// CPipeProbeMainDlg ダイアログ
class CPipeProbeMainDlg : public CDialogEx
{
// コンストラクション
public:
	CPipeProbeMainDlg(CWnd* pParent = nullptr);	// 標準コンストラクター
	virtual ~CPipeProbeMainDlg();
// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POPEPROBE_DIALOG };
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
	void LoadProfile();
	void SaveProfile();
	afx_msg void OnDestroy();
	ExeEngine m_ExeEngine;
	Probe m_ProbeDown;
	Probe m_ProbeUp;
	char m_BufUp[BUF_SIZE];
	char m_BufDown[BUF_SIZE];
	afx_msg LRESULT OnUserReadDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserReadUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserExeMessage(WPARAM wParam, LPARAM lParam);
	HANDLE m_hChild_Down_Wr;
	HANDLE m_hChild_Up_Rd;
	Regex m_Regex;
	ProfileLogView m_ProfileLogView;
	ProfileLogFile m_ProfileLogFile;
	ProfileGenelal m_ProfileGenelal;
	ProfileChild m_ProfileChild;
	HANDLE m_hFile;
	CString m_strEditMain;
	CString m_strInfomation;
	CEdit m_ctlEditMain;
	BOOL m_bINI_Existing;

public:
	void Execute();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedDisconnectPipe();

protected:
	BOOL TrayExecute(DWORD dwCommand);
	afx_msg LRESULT OnTasktray(WPARAM wParam, LPARAM lParam);
	BOOL m_b_is_hide;
	void MainWndShow();
	void MainWndIcon();
	void MainWndHide();
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	HWND m_hConsole;
public:
	afx_msg void OnBnClickedButtonForcedTermination();
	afx_msg void OnBnClickedButtonGoTasktray();
	afx_msg void OnClose();
protected:
	BOOL m_bFirst_Appear;
	CRect m_rectMainWnd;
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnAboutbox();
};
