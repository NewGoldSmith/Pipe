
// PipeProbeSettingDlg.h : ヘッダー ファイル
//

#pragma once
#include "CPropPageGenelal.h"
#include "CPropPageLogFile.h"
#include "CDialogTab.h"
#include "CPropPageChild.h"
#include "CPropPageLogView.h"
#include "CPropPageRegexTest.h"
#include "CPropPageStrftimeDescription.h"

// CProbeSettingMainDlg ダイアログ
class CProbeSettingMainDlg : public CDialogEx
{
// コンストラクション
public:
	CProbeSettingMainDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PIPEPROBE_SETTING_MAIN_DIALOG};
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
	CDialogTab m_ctlMainTab;
protected:
	CPropPageGenelal m_PropGenelal;
	CPropPageLogFile m_PropLogSetting;
	CPropPageChild m_PropChild;
	CPropPageLogView m_PropLogView;
	CPropPageRegexTest m_PropRegexTest;
	CPropPageStrftimeDescription m_PropStrftimeDescription;
private:
	void LoadProfile();
	void SaveProfile();
public:
	afx_msg void OnDestroy();
protected:
	void SaveProfileApp();
	void LoadProfileApp();
	CString m_strSettingFilePathName;
public:
	afx_msg void OnBnClickedButtonSaveProfileAs();
	afx_msg void OnBnClickedButtonSaveOverWrite();
protected:
	CButton m_ctlButtonSaveSetting;
	int m_iCurSel;
public:
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedButtonLoadExistingSettingFile();
//	afx_msg void OnKillFocus(CWnd* pNewWnd);
//	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedButtonTestExecute();
protected:
	void ErrDlg(LPTSTR str);
	CString m_strExePathName;
public:
	afx_msg void OnBnClickedButtonInstallExe();
};
