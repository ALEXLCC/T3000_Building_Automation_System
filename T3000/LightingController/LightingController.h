

#pragma once
#include "afxwin.h"
#include "../msflexgrid1.h"
#include "../LightingController/LightingSet.h"
#include <map>

#include "../LightingController/configure.h"
// #include <vector>
// #include <algorithm>


#include "afxdtctl.h"
#include "atlcomtime.h"
#include "afxwin.h"
#define  WM_WRITE_MESSAGE WM_USER+1050

typedef struct   LIGHTINTGCONTROLLER
{
	int iaddress;
	CString CStvalue;
}lightingcontroller;


typedef struct OUTPUTNAMESTATUS
{
	CString Name;
	CString Status;
}ONS;


// CLightingController form view

class CLightingController : public CFormView
{
	DECLARE_DYNCREATE(CLightingController)

protected:
public:
	CLightingController();           // protected constructor used by dynamic creation
	virtual ~CLightingController();

public:
	enum { IDD = IDD_DIALOG_LIGHTINGCONTROLLER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void Fresh();
	unsigned int m_inaddress;
	int m_inSerialNum;
	float m_flFirmware;
	int m_inHardware;
	CString m_CStrModel;
	int m_inBaudrate;
	void ShowLighContDlg();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_datetime;
	unsigned short LightCregister[512];
	BOOL prodtopcData();//�ɼ�����
	BOOL checkDB(CString DBname,CString strSQL);//�����Ƿ񴴽����ݿ�
	BOOL ReadDB();//��ȡ���ݿ�����
	BOOL UpdateDBALL();
	BOOL UpdateDBPART(int startnum,int endnum,WORD*savedata);

	int comnum;//��ȡ���ں�
	vector<lightingcontroller>veclightingcontroller;
	lightingcontroller m_veclightingcontroller;

	BOOL m_ArrayOutput[96];
	BOOL InitializeArray(BOOL*ARRAY);
	


	
	CListBox m_ListBox;
	afx_msg void OnLbnDblclkListInput();
	afx_msg void OnLbnSelchangeListInput();


	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);


	WORD* ConvertOutput();
	WORD WordTemp[6];

	CMsflexgrid m_msflexgrid1to96;

	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_comboBox;
	CString m_strcomboBox;

	afx_msg LRESULT OnWriteMessage(WPARAM wParam,LPARAM lParam);

	
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();


	 long row,col;
	 void DblClickMsflexgrid1();
	 CEdit m_editName;
	 afx_msg void OnEnKillfocusEditModifyname();
	 //����������Status
	 vector<ONS>m_vecONS;
	 ONS m_structONS;
 	 void ReadDB_OutNameStatus();

	 void SaveDB_OutNameStatus();



	 //DB and interface data struct
// 	 typedef struct OUT_NUM
// 	 {
// 		 CString addr;
// 		 int      output_no;
// 
// 	 }stru_outnum;
	
//typedef vector<stru_outnum> VECTORT_OUTPUT;
public:
	 typedef struct OUT_PARAMETER
	 {
		 CString outname;
		 CString outstatus;
	 }stru_outparameter;
	 stru_outparameter m_stru_outparam;

	 lightingcontroller m_strutoutaddr;
	
	typedef vector<lightingcontroller> VECTOR_OUT_ADDR;
	VECTOR_OUT_ADDR m_vecaddrinout;

	typedef vector<stru_outparameter> VECTOR_OUT_PARAM;
	VECTOR_OUT_PARAM m_vecoutparam;

public:
	typedef std::map<int,CString> MAP_OUT_ADDRESS;//int��ʾoutput���˳�� CString��output�ĵ�ַ
	MAP_OUT_ADDRESS m_mapoutputaddress;

	

	typedef std::map<CString,MAP_OUT_ADDRESS> MAP_INT_OUT;
	MAP_INT_OUT m_mapinout;


	typedef std::map<CString,VECTOR_OUT_PARAM> MAP_OUT_PARAM;
	MAP_OUT_PARAM m_mapoutputparam;

	int m_outputsum;

	BYTE senddata[100];
	



	afx_msg void OnSetmappingAddoutputbarod();
	afx_msg void OnSetmappingPrevious();
	afx_msg void OnSetmappingNext();
	afx_msg void OnSetmappingRead();
	afx_msg void OnSetmapSettingsave();
	afx_msg void OnSetmappingSand();
	afx_msg void OnBnClickedButtonLightingcontorlWeeklys();
	afx_msg void OnBnClickedButtonLightingcontorlAnnuals();
	afx_msg void OnBnClickedButtonLightingcontorlGroups();
	afx_msg void OnBnClickedButtonLightingcontorlSyncwithPC();


	WORD lightingController_time[8];
	
	COleDateTime m_date;
	COleDateTime m_time;

	void Automationflexrow();
	afx_msg void OnBnClickedButtonAdd();
	CComboBox m_comboboxpanel;
	CComboBox m_comboboxouputboard;
	CComboBox m_comboboxoutput;
	CString m_strcomboboxpanel;
	CString m_strcomboboxoutputboard;
	CString m_strcomboboxoutput;


	typedef struct ADDOUTPUTS 
	{
		CString strpanel;
		CString stroutputboard;
		CString stroutputno;
		CString stroutputname;
	}structaddoutputs;
	structaddoutputs m_structaddoutputs;

	typedef vector<structaddoutputs> vecaddoutputs;
	vecaddoutputs m_vecaddoutputs;

	map<CString,vecaddoutputs> m_mapaddoutputs;
	WORD SerialNum[100];

	BOOL GetSerialComm(vector<CString>& szComm);//���˴���
	vector<CString> m_szComm;

	int outputno;//output�˿ں�

	 void readSerial();



	 afx_msg void OnBnClickedButtonApply();
	 BOOL CheckSettingChanged();
	 CIPAddressCtrl			m_ip_addressCtrl;
	 CIPAddressCtrl			m_subnet_addressCtrl;
	 CIPAddressCtrl			m_gateway_addressCtrl;
	 CEdit m_listenPortEdit;
	 int					m_nListenPort;
	 afx_msg void OnBnClickedButtonReboot();
	 afx_msg void OnBnClickedButtonResetToFactory();

	 afx_msg void OnBnClickedCheckEnableEdit();
	 CButton					m_ReadOnlyCheckBtn;
	 CComboBox m_ipModelComBox;
	 afx_msg void OnBnClickedButtonConfigure();
	 afx_msg void OnBnClickedButtonConfigureswitch();
};



