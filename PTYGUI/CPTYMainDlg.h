//Copyright (c) 2021, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php

// CPTYMainDlg.h : ヘッダー ファイル
//

#pragma once

#include <CPConsole.h>
#include <CBStreamQue.h>
#include <CBStreamTerm.h>
#include <CBWinConv.h>
#include <CReadNumByte.h>
#include <functional>

// CPTYMainDlg ダイアログ
class CPTYMainDlg;
class CPTYMainDlg : public CDialogEx
{
// コンストラクション
public:
	CPTYMainDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PTYGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void LoadProfile();
public:
	void SaveProfile();
	afx_msg void OnDestroy();
	CEdit m_EditCommand;
	CEdit m_EditHexText;
	afx_msg void OnClickedButtonOpen();
protected:
	CPConsole m_Console;
//	CJointIntercept m_JointIntercept;
//	BStreamQue m_StreamQue;
public:
	afx_msg void OnClickedButtonClose();
protected:
	enum emUSER_MESSAGE :UINT { WM_USER_P_END = WM_USER + 1 };
	enum emTimerId:UINT {emTimer1};
protected:
	afx_msg LRESULT OnUserPEnd(WPARAM wParam, LPARAM lParam);
public:
	void OnTerminatePConsole();
	afx_msg void OnClickedRadioStdAccessmodeRead();
protected:
	CButton m_RadioStdAccessModeRead;
	CButton m_RadioPAccessModeRead;
	CButton m_RadioInterceptModeOff;
public:
	afx_msg void OnBnClickedRadioStdAccessmodeWrite();
	afx_msg void OnBnClickedRadioStdAccessmodeBoth();
	afx_msg void OnBnClickedRadioPAccessmodeRead();
	afx_msg void OnBnClickedRadioPAccessmodeWrite();
	afx_msg void OnBnClickedRadioPAccesssmodeBoth();
	afx_msg void OnBnClickedRadioInterceptOff();
	afx_msg void OnBnClickedRadioInterceptCover();
	afx_msg void OnBnClickedRadioInterceptSteal();
protected:
	CButton m_RadioPAccessModeWrite;
	CButton m_RadioPAccessModeBoth;
	CButton m_RadioStdAccessModeWrite;
	CButton m_RadioStdAccessModeBoth;
	CButton m_RadioInterceptModeCover;
	CButton m_RadioInterceptModeSteal;
	CButton m_RadioStdAccessModeNo;
	CButton m_RadioPAccessModeNo;
public:
	afx_msg void OnBnClickedRadioStdAccessmodeNo();
	afx_msg void OnBnClickedRadioPAccessmodeNo();
protected:
	CStatic m_StaticVTFilePath;
	CEdit m_EditVtTimer;
public:
	afx_msg void OnBnClickedButtonPRead();
	afx_msg void OnBnClickedButtonSdtRead();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonSdtCommand();
	afx_msg void OnBnClickedButtonPCommand();
	afx_msg void OnBnClickedButtonVtfileOpen();
	afx_msg void OnBnClickedButtonVtPlay();
	afx_msg void OnBnClickedButtonVtStop();
protected:
	CEdit m_EditContets;
public:
	afx_msg void OnBnClickedButtonPHextext();
	afx_msg void OnBnClickedButtonSdtHextext();
	afx_msg void OnBnClickedButtonPCrlf();
	afx_msg void OnBnClickedButtonStdEsc();
	afx_msg void OnBnClickedButtonVtfileClose();
	afx_msg void OnBnClickedButtonReset();
protected:
	CButton m_CheckVtRepeat;
	CEdit m_EditNBytesRead;
	CSpinButtonCtrl m_SpinBtTimer;
	CSpinButtonCtrl m_SpinNBytesRead;
public:
	afx_msg void OnEnChangeEditVtTimer();
	afx_msg void OnEnChangeEditNBytesRead();
protected:
	CReadNumByte m_VTReadNumByte;
	bool m_bIsVTRunning;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckVtRepeat();
protected:
	CStatic m_StaticBitmap;
	CEdit m_EditAA;
	CFont m_FontAA;
};
