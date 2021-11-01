
// MainDlg.h : ヘッダー ファイル
//

#pragma once
#include "CSpinButtonCtrlDX.h"
#include "CIPAddressCtrlDX.h"
#include "CListCtrlForSocket.h"
#include "StringHelper.h"
#include "CComboBoxDX.h"

#define WM_USER_SOCK_LISTEN_MESSAGE WM_USER +40
#define WM_USER_SOCK_SERVER_MESSAGE WM_USER +41
#define WM_USER_SOCK_CLIENT_MESSAGE WM_USER +42

// MainDlg ダイアログ
class MainDlg : public CDialogEx
{
// コンストラクション
public:
	MainDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MASTERINGWINSOCK_DIALOG };
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
	void LoadProfile();
	void SaveProfile();
	CRect m_rectMainWnd;
	BOOL m_bFirst_Appear;
public:
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
protected:
	CIPAddressCtrlDX m_IPAddressListenAddress;
	CSpinButtonCtrlDX m_SpinListenAddress;
	CEdit m_EditListenPort;
	CSpinButtonCtrlDX m_SpinListenPort;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	CIPAddressCtrlDX m_IPAddressCliAddress;
	CSpinButtonCtrlDX m_SpinCliAddress;
	CEdit m_EditCliPort;
	CSpinButtonCtrlDX m_SpinCliPort;
	CIPAddressCtrlDX m_IPAddressConnectToAddress;
	CSpinButtonCtrlDX m_SpinConnectToAddress;
	CEdit m_EditConnectToPort;
	CSpinButtonCtrlDX m_SpinConnectToPort;
	CListCtrlForSocket m_ListListenCreate;
	CListCtrlForSocket m_ListCliCreate;
//	CListCtrlForSocket m_ListListenWaitAccept;
//	CListCtrlForSocket m_ListCliWaitAccept;
	CListCtrlForSocket m_ListSevConnected;
	CListCtrlForSocket m_ListCliConnected;
//	CEdit m_EditSevBinaryForSending;
//	CEdit m_EditSevTextForSending;
//	CEdit m_EditCliBinaryForSending;
//	CEdit m_EditCliTextForSending;
	CEdit m_EditSevReceivedBinary;
	CEdit m_EditSevReceivedText;
	CEdit m_EditCliReceivedBinary;
	CEdit m_EditCliRedeivedText;
public:
	afx_msg void OnBnClickedButtonListenCreate();
protected:
	UINT m_ListenID;
	UINT m_SevID;
	UINT m_CliID;
public:
	afx_msg void OnBnClickedButtonListenDelete();
	afx_msg void OnBnClickedButtonListenListen();
	afx_msg void OnBnClickedButtonListenClose();
protected:
	void DeleteAllListItem();
public:
	afx_msg void OnBnClickedButtonCliCreate();
	afx_msg void OnBnClickedButtonListenNew();
	afx_msg void OnBnClickedButtonCliNew();
	afx_msg void OnBnClickedButtonCliClose();
	afx_msg void OnBnClickedButtonCliDelete();
	afx_msg void OnBnClickedButtonCliConnect();
//	afx_msg void OnBnClickedButtonCliWaitingClose();
//	afx_msg void OnBnClickedButtonCliWaitingDelete();
protected:
	afx_msg LRESULT OnUserPipeListenMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserPipeServerMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserPipeClientMessage(WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnBnClickedButtonWaitingAcceptClose();
//	afx_msg void OnBnClickedButtonWaitingAcceptDelete();
	afx_msg void OnBnClickedButtonAccept();
	afx_msg void OnBnClickedButtonSevConnectingClose();
	afx_msg void OnBnClickedButtonSevConnectingDelete();
	afx_msg void OnBnClickedButtonCliConnectingClose();
	afx_msg void OnBnClickedButtonCliConnectingDelete();
protected:
	void AllListUpDateView();
public:
//	afx_msg void OnBnClickedButton1();
protected:
	CListCtrlForSocket m_ListSevCreate;
public:
	afx_msg void OnBnClickedButtonSevNew();
	afx_msg void OnBnClickedButtonSevDelete();
	afx_msg void OnBnClickedButtonSevCreate();
	afx_msg void OnBnClickedButtonSevClose();
	afx_msg void OnBnClickedButtonSevSendBinary();
	afx_msg void OnBnClickedButtonCliConnectingReceive();
	afx_msg void OnBnClickedButtonCliClearText();
	afx_msg void OnBnClickedButtonSevSendText();
	afx_msg void OnBnClickedButtonSevClearText();
	afx_msg void OnBnClickedButtonCliSendBinary();
	afx_msg void OnBnClickedButtonCliSendText();
	afx_msg void OnBnClickedButtonSevConnectingReceive();
	afx_msg void OnBnClickedButtonListenShutdown();
//	afx_msg void OnBnClickedRadio3();
protected:
	CButton m_RadioShutdownReceives;
	CButton m_RadioShutdownSends;
	CButton m_RadioShutdownBoth;
public:
	afx_msg void OnBnClickedButtonCliShutdown();
	afx_msg void OnBnClickedButtonSevShutdown();
	afx_msg void OnBnClickedButtonSevConnectingShutdown();
	afx_msg void OnBnClickedButtonCliConnectedShutdown();
protected:
	CComboBoxDX m_ComboCliBynaryForSending;
	CComboBoxDX m_ComboSevBinaryForSending;
	CComboBoxDX m_ComboSevTextForSending;
	CComboBoxDX m_ComboCliTextForSending;
public:
	int GetShutdownHowCheck();
	int SetShutdownHowCheck(int nHow);
protected:
	CButton m_ButtonSevSendBinary;
	CButton m_ButtonSevSendText;
	CButton m_ButtonCliSendBinary;
	CButton m_ButtonCliSendText;
};
