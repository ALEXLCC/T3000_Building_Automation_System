#pragma once
#include "afxcmn.h"

#include "CM5/ListCtrlEx.h"
// CBacnetRemotePoint dialog

class CBacnetRemotePoint : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetRemotePoint)

public:
	CBacnetRemotePoint(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetRemotePoint();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_REMOTE_POINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx m_remote_point_list;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	afx_msg void OnClose();
	void Initial_List();
	afx_msg LRESULT Fresh_Remote_List(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

const int REMOTE_NUMBER = 0;
const int REMOTE_DEVICE_ID = 1;
const int REMOTE_REG = 2;
const int REMOTE_VALUE = 3;
const int REMOTE_DEVICE_STATUS = 4;
const int REMOTE_DESCRIPTION = 5;

const int REMOTE_COL_NUMBER = 6;
