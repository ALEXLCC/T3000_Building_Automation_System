// ToolCalibrateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MainFrm.h"
#include "ToolCalibrateDlg.h"


// CToolCalibrateDlg dialog
// CToolCalibrateDlg dialog
typedef struct _Calbrate
{
	int ID;//tstat ID
	unsigned int serialnumber;	//the serialnumber
	int baudrate;//9600 or 19200
	unsigned short hardware_revisin;//6 register of tstat
	unsigned short software_version;//7 register of tstat
	CString hex_file_path;//the hex file path
	bool flash_or_no;//selected
	bool save_settings;
	float temperature;
}_CalbrateInfo;
vector<_CalbrateInfo> gCalibrateVector;
IMPLEMENT_DYNAMIC(CToolCalibrateDlg, CDialog)

CToolCalibrateDlg::CToolCalibrateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolCalibrateDlg::IDD, pParent)
{

}

CToolCalibrateDlg::~CToolCalibrateDlg()
{
}

void CToolCalibrateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRIDFLASH_MSFLEXGRID, m_FlexGrid);
	DDX_Control(pDX, IDC_COMBO1, m_subNetList);
}


BEGIN_MESSAGE_MAP(CToolCalibrateDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CToolCalibrateDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CToolCalibrateDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CToolCalibrateDlg message handlers

void CToolCalibrateDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CToolCalibrateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	//m_strMainBuilding= pFrame->m_strCurMainBuildingName;
	CString m_strSubNetName= pFrame->m_strCurSubBuldingName;
//	m_strScanSubNetNameCobox.AddString(_T("All subNets..."));
	int k=0;
	for(int i=0;i<pFrame->m_subNetLst.size();i++)
	{
		if(m_strSubNetName.CompareNoCase(pFrame->m_subNetLst.at(i).strBuildingName)==0)
			k=i;
		m_subNetList.AddString(pFrame->m_subNetLst.at(i).strBuildingName);
	}
	if(m_subNetList.GetCount()>0)
		m_subNetList.SetCurSel(k);

	m_FlexGrid.put_TextMatrix(0,1,_T("Serial#"));
	m_FlexGrid.put_TextMatrix(0,2,_T("ID"));
	m_FlexGrid.put_TextMatrix(0,3,_T("Baudrate"));
	m_FlexGrid.put_TextMatrix(0,4,_T("Hardware Rev"));
	m_FlexGrid.put_TextMatrix(0,5,_T("Device"));
	m_FlexGrid.put_TextMatrix(0,6,_T("Temperature"));
	m_FlexGrid.put_TextMatrix(0,7,_T("Enable"));
	m_FlexGrid.put_TextMatrix(0,8,_T("Connection"));
	
	m_FlexGrid.put_ColWidth(0,0);
	m_FlexGrid.put_ColWidth(1,1100);
	m_FlexGrid.put_ColWidth(2,500);
	m_FlexGrid.put_ColWidth(3,900);
	m_FlexGrid.put_ColWidth(4,800);
	m_FlexGrid.put_ColWidth(5,1300);
	m_FlexGrid.put_ColWidth(6,1000);
	m_FlexGrid.put_ColWidth(7,800);
	m_FlexGrid.put_ColWidth(8,1200);
	gCalibrateVector.clear();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CToolCalibrateDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::PreTranslateMessage(pMsg);
}

void CToolCalibrateDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}
BEGIN_EVENTSINK_MAP(CToolCalibrateDlg, CDialog)
	ON_EVENT(CToolCalibrateDlg, IDC_GRIDFLASH_MSFLEXGRID, DISPID_CLICK, CToolCalibrateDlg::ClickGridflashMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()

void CToolCalibrateDlg::ClickGridflashMsflexgrid()
{
	int lRow,lCol;
	lRow = m_FlexGrid.get_RowSel();//��ȡ������к�	
	lCol = m_FlexGrid.get_ColSel(); //��ȡ������к�
	UpdateData(false);
	if(lRow==0 || lCol==0)
		return;
	/*
	if(m_grid_flash_calibrate.size()+1==lRow)
		return;

	int nTemp=m_grid_flash_calibrate.at(lRow-1).ID;
	read_one(nTemp,101);
	CString stringTemp;
	stringTemp.Format(_T("%.1f"),nTemp/10.0);
	m_FlexGrid.put_TextMatrix(lRow,6,stringTemp.GetString());
	GetDlgItem(IDC_EDIT2)->SetWindowText(stringTemp);
	*/

	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
	ScreenToClient(rect); //ת��Ϊ�ͻ�������	
	// MSFlexGrid�ؼ��ĺ����ĳ��ȵ�λ��"�(twips)"��
	//��Ҫ����ת��Ϊ���أ�1440�= 1Ӣ��
	CDC* pDC =GetDC();
	//�������ص��羵�ת������
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//�γ�ѡ�и����ڵľ�������
	CRect rc(x,y,x+width,y+height);
	//ת������ԶԻ��������
	rc.OffsetRect(rect.left+1,rect.top+1);
	//��ȡѡ�и���ı���Ϣ
	CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);
	row_row=lRow;
	row_col=lCol;
	if(lCol==7)
	{
		CString temp_str=m_FlexGrid.get_TextMatrix(row_row,row_col);
		if(temp_str.CompareNoCase(_T("true"))==0)
			m_FlexGrid.put_TextMatrix(row_row,row_col,_T("False"));
		if(temp_str.CompareNoCase(_T("false"))==0)
			m_FlexGrid.put_TextMatrix(row_row,row_col,_T("True"));		
	}
	else
		return;
}
