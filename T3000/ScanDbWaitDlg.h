#pragma once
#include "afxcmn.h"
//#include "TStatScanner.h"
#include "CM5/ListCtrlEx.h"
// CScanDbWaitDlg dialog
#define WM_NETSCANINFO  WM_USER + 1200
#define WM_COMSCANINFO  WM_USER + 1201
#define WM_BACNETCOMSCANINFO WM_USER + 1202


class CTStatScanner;
class CScanDbWaitDlg : public CDialog
{
	DECLARE_DYNAMIC(CScanDbWaitDlg)

public:
	CScanDbWaitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScanDbWaitDlg();

// Dialog Data
	enum { IDD = IDD_SCANDBWAITDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedExitbutton();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnNetScanInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnComScanInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBacnetComScanInfo(WPARAM wParam, LPARAM lParam);


	void SetPromtText(CString strInfo);
	void SetTStatScaner(CTStatScanner* pScanner);
	void Initial_List();
	CString m_strInfopromp;
	CString m_strDot;

	CTStatScanner*		m_pScaner;
	CString m_strNetScanInfo;
	CString m_strComScanInfo;
	ListCtrlEx::CListCtrlEx m_scan_com_list;
};

const int SCAN_MODE = 0;
//const int SCAN_BAUDRATE = 1;
const int SCAN_SKIP = 1;
const int SCAN_STATUS = 2;
const int SCAN_FOUND = 3;
const int SCAN_NOTES = 4;

const int SCAN_ALL_COLUME = 5;
