// configure.cpp : implementation file
//

#include "stdafx.h"
#include "..\T3000.h"
#include "configure.h"


// Cconfigure dialog


IMPLEMENT_DYNAMIC(Cconfigure, CDialog)

Cconfigure::Cconfigure(CWnd* pParent /*=NULL*/)
	: CDialog(Cconfigure::IDD, pParent)
	, m_strcombo(_T(""))
{

}

Cconfigure::~Cconfigure()
{

}

void Cconfigure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_Configure, m_msflexgrid);
	DDX_Control(pDX, IDC_COMBO_SELECT, m_controlcombo);
	DDX_CBString(pDX, IDC_COMBO_SELECT, m_strcombo);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(Cconfigure, CDialog)
	ON_CBN_KILLFOCUS(IDC_COMBO_SELECT, &Cconfigure::OnCbnKillfocusComboSelect)
	ON_EN_KILLFOCUS(IDC_EDIT1, &Cconfigure::OnEnKillfocusEdit1)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &Cconfigure::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// Cconfigure message handlers

BOOL Cconfigure::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_msflexgrid.put_Rows(25); //��
	//set row high
	m_msflexgrid.put_WordWrap(TRUE);
	m_msflexgrid.put_RowHeight(0,500);
	//title middle show
	for (int n=0;n<=3;n++)
	{
		m_msflexgrid.put_ColAlignment(n,4);
	}
//�����еĸ߶�
	CString str;
	for(int i=1;i<25;i++)
	{
		str.Format(_T("INPUT%d"),i);
		m_msflexgrid.put_RowHeight(i,300);
		m_msflexgrid.put_TextMatrix(i,0,str);
		m_msflexgrid.put_TextMatrix(i,1,_T("ON/OFF"));
	}

	//�����п�
	m_msflexgrid.put_ColWidth(0,1200);
	m_msflexgrid.put_ColWidth(1,1500);
	m_msflexgrid.put_ColWidth(2,1500);
	m_msflexgrid.put_ColWidth(3,1500);



	m_msflexgrid.put_TextMatrix(0,0,_T("INPUTS"));
	m_msflexgrid.put_TextMatrix(0,1,_T("TYPE"));
	m_msflexgrid.put_TextMatrix(0,2,_T("Function"));
	m_msflexgrid.put_TextMatrix(0,3,_T("Override Time (second)"));

	//��ȡӲ������

	//201��224	1 * 24	switch (1..24) types: 0 --- low active,  1--- high active,   2 --- falling edge active,    3 --- rising edge active
	//252��275	2 * 24	override time for each switch. Uint is second. 2bytes = 65536s =~18hours max.
	memset(m_switch,0,sizeof(m_switch));
	memset(m_overridetime,0,sizeof(m_overridetime));

	 int 	flg = Read_Multi(g_tstat_id,m_switch,201,24);
	 int    flag =  Read_Multi(g_tstat_id,m_overridetime,252,24);


	WORD itemp[24];
	memcpy(itemp,m_switch,sizeof(m_switch));
	CString strtemp;
	for (int i = 0;i<24;i++)
	{
		//TYPE
		itemp[i]= itemp[i]&0x0F;
		switch(itemp[i])
		{
		case 0:
			m_msflexgrid.put_TextMatrix(i+1,1,_T("ON/OFF"));
			break;
		case 1:
			m_msflexgrid.put_TextMatrix(i+1,1,_T("OFF/ON"));
			break;
		case 2:
			m_msflexgrid.put_TextMatrix(i+1,1,_T("SW OPEN PULSE"));
			break;
		case 3:
			m_msflexgrid.put_TextMatrix(i+1,1,_T("SW CLOSE PULSE"));
			break;
		}
		//Function
		m_switch[i] = m_switch[i]>>4;
		switch(m_switch[i])
		{
		case 0:
			m_msflexgrid.put_TextMatrix(i+1,2,_T("ONLY ON"));
			break;
		case 1:
			m_msflexgrid.put_TextMatrix(i+1,2,_T("ON&OFF"));
			break;
		}		
		//Overridetime
		strtemp.Format(_T("%d"),m_overridetime[i]);
		m_msflexgrid.put_TextMatrix(i+1,3,strtemp);
}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(Cconfigure, CDialog)
	ON_EVENT(Cconfigure, IDC_MSFLEXGRID_Configure, DISPID_CLICK, Cconfigure::ClickMsflexgridConfigure, VTS_NONE)
END_EVENTSINK_MAP()

void Cconfigure::ClickMsflexgridConfigure()
{

	long lRow = m_msflexgrid.get_RowSel();//��ȡ������к�	
	long lCol = m_msflexgrid.get_ColSel(); //��ȡ������к�	

	CRect rect;	
	m_msflexgrid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
	ScreenToClient(rect); //ת��Ϊ�ͻ�������
	// MSFlexGrid�ؼ��ĺ����ĳ��ȵ�λ��"�(twips)"��
	//��Ҫ����ת��Ϊ���أ�1440�= 1Ӣ��
	CDC* pDC =GetDC();
	//�������ص��羵�ת������
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
	long y = m_msflexgrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_msflexgrid.get_ColPos(lCol)/nTwipsPerDotX;
	//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
	long width = m_msflexgrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_msflexgrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//�γ�ѡ�и����ڵľ�������
	CRect rc(x,y,x+width,y+height);
	//ת������ԶԻ��������	
	rc.OffsetRect(rect.left+1,rect.top+1);	
	//��ȡѡ�и���ı���Ϣ	

	CString strValue = m_msflexgrid.get_TextMatrix(lRow,lCol);
	if (lCol == 1||lCol ==2)
	{
		//ʹ��combobox�ؼ���ʽ��ʾ
		m_edit.ShowWindow(SW_HIDE);
		m_controlcombo.ShowWindow(SW_SHOW);

		m_controlcombo.ResetContent();

		if (lCol == 1)
		{

			m_controlcombo.AddString(_T("ON/OFF"));
			m_controlcombo.AddString(_T("OFF/ON"));
			m_controlcombo.AddString(_T("SW OPEN PULSE"));
			m_controlcombo.AddString(_T("SW CLOSE PULSE"));
		}
		else if (lCol == 2)
		{
			m_controlcombo.AddString(_T("ONLY ON"));
			m_controlcombo.AddString(_T("ON&OFF"));
		}


		m_controlcombo.MoveWindow(&rc,1); //�ƶ���ѡ�и��λ��
		m_controlcombo.BringWindowToTop();	
		m_controlcombo.SelectString(-1,strValue);
		m_controlcombo.ShowWindow(SW_SHOW);//��ʾ�ؼ�
	}else if (lCol ==3)
	{
		//ʹ��edition�ؼ���ʽ��ʾ
		m_controlcombo.ShowWindow(SW_HIDE);

		m_edit.MoveWindow(&rc,1);
		m_edit.ShowWindow(SW_SHOW);	
		m_edit.SetWindowText(strValue);	
		m_edit.SetFocus();
		m_edit.SetCapture();//ʹ����������붼�����͵����CWnd 
		int nLenth=strValue.GetLength();	
		m_edit.SetSel(nLenth,nLenth); 	

	}else
	{
		m_edit.ShowWindow(SW_HIDE);
		m_controlcombo.ShowWindow(SW_HIDE);
	}

}

void Cconfigure::OnCbnKillfocusComboSelect()
{
	long lRow = m_msflexgrid.get_RowSel();//��ȡ������к�	
	long lCol = m_msflexgrid.get_ColSel(); //��ȡ������к�	



	CString strTemp;
	int nindext = m_controlcombo.GetCurSel();//GetEditSel();	 GetCurSel
	if (nindext>=0)
	{	   
		m_controlcombo.GetLBText(nindext,strTemp);
		m_msflexgrid.put_TextMatrix(lRow,lCol,strTemp);
	}	



	
}

void Cconfigure::OnEnKillfocusEdit1()
{
	CString strText;
	m_edit.GetWindowText(strText); 
	m_edit.ShowWindow(SW_HIDE);  
	long lRow = m_msflexgrid.get_RowSel();	
	long lCol = m_msflexgrid.get_ColSel(); 

	m_msflexgrid.put_TextMatrix(lRow,lCol,strText);

}

void Cconfigure::OnBnClickedButtonSend()
{
	CString str,str1,str3;
	BYTE Sendswitch[24];
	BYTE SendTimebye[48];
	WORD SendTimeword[24];
	WORD Wtemp = 0;

	BYTE bytetmep = 0;

	memset(Sendswitch,0,sizeof(Sendswitch));
	memset(SendTimebye,0,sizeof(SendTimebye));
	memset(SendTimeword,0,sizeof(SendTimeword));

	for (int i = 0;i<24;i++)
	{
		 str = m_msflexgrid.get_TextMatrix(i+1,1);
		if(str.CompareNoCase(_T("ON/OFF")) == 0) 
		{
			Sendswitch[i] = 0;

		}else if (str.CompareNoCase(_T("OFF/ON") )== 0)
		{
			Sendswitch[i] = 1;

		}else if (str.CompareNoCase(_T("SW OPEN PULSE")) == 0)
		{
			Sendswitch[i] = 2;

		}else if (str.CompareNoCase(_T("SW CLOSE PULSE")) == 0)
		{
			Sendswitch[i] = 3;

		}

		str1 = m_msflexgrid.get_TextMatrix(i+1,2);
	     if (str1.CompareNoCase(_T("ON&OFF")) == 0)
		{
			bytetmep = 1;
			bytetmep = bytetmep<<4;
			Sendswitch[i] = Sendswitch[i]|bytetmep;
		}

		 //time
		 str3 = m_msflexgrid.get_TextMatrix(i+1,3);
		 SendTimeword[i] = _ttoi(str3);
// 		 Wtemp = SendTimeword[i];
// 		 Wtemp = Wtemp&0x0F;
// 		 SendTimebye[i*2+1] = (BYTE)Wtemp;
// 		 Wtemp = SendTimeword[i];
// 		 Wtemp = Wtemp>>8;
// 		 SendTimebye[i*2] = (BYTE)Wtemp;

	}


	//201��224	1 * 24	switch (1..24) types: 0 --- low active,  1--- high active,   2 --- falling edge active,    3 --- rising edge active
	//252��275	2 * 24	override time for each switch. Uint is second. 2bytes = 65536s =~18hours max.
	int ret1=0,ret=0;
	ret = Write_Multi(g_tstat_id,Sendswitch,201,24);
	//int ret1 = Write_Multi(g_tstat_id,SendTimebye,252,48);
	for (int i = 0;i<24;i++)
	{
		 ret1= write_one(g_tstat_id,252+i,SendTimeword[i]);
	
	}
	

	if ((ret>0)&&(ret1>0))
		AfxMessageBox(_T("Send successful!"));
	else
		AfxMessageBox(_T("Send unsuccessful!"));

}
