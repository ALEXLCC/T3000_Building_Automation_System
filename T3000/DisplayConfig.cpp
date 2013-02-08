// DisplayConfig.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DisplayConfig.h"
#include "afxdialogex.h"
#include "globle_function.h"

// CDisplayConfig dialog
#define  THE_CHAR_LENGTH 8
IMPLEMENT_DYNAMIC(CDisplayConfig, CDialog)

CDisplayConfig::CDisplayConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayConfig::IDD, pParent)
	 
	{
	m_display_number=15;
	}

CDisplayConfig::~CDisplayConfig()
{
}
BOOL CDisplayConfig::OnInitDialog()
{
CDialog::OnInitDialog();




 Fresh_Grid();
return TRUE;
}
void CDisplayConfig::DoDataExchange(CDataExchange* pDX)
{
CDialog::DoDataExchange(pDX);
DDX_Control(pDX, IDC_INPUT_MSFLEXGRID, m_FlexGrid1);
DDX_Text(pDX, IDC_INPUT_9, m_display_number);
DDX_Control(pDX, IDC_SEQ_COM, m_ItemValueCombx);
}


BEGIN_MESSAGE_MAP(CDisplayConfig, CDialog)
	ON_BN_CLICKED(IDOK, &CDisplayConfig::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_INPUT_9, &CDisplayConfig::OnEnKillfocusInput9)
ON_CBN_SELCHANGE(IDC_SEQ_COM, &CDisplayConfig::OnCbnSelchangeSeqCom)
//ON_EN_KILLFOCUS(IDC_LINE_1, &CDisplayConfig::OnEnKillfocusLine1)
ON_BN_CLICKED(IDCANCEL, &CDisplayConfig::OnBnClickedCancel)
ON_BN_CLICKED(IDC_SAVE2, &CDisplayConfig::OnBnClickedSave2)
ON_BN_CLICKED(IDC_SAVE_LINE2, &CDisplayConfig::OnBnClickedSaveLine2)
END_MESSAGE_MAP()


 

 CString CDisplayConfig::GetTextFromReg(UINT reg){
    CString str_temp;
	 unsigned short temp_buffer[4];
	 unsigned short temp_buffer_Char[THE_CHAR_LENGTH];
	 unsigned char p[THE_CHAR_LENGTH+1]={'\0'};
	 if (Read_Multi(g_tstat_id,temp_buffer,reg,4))
		 {  unsigned short Hi_Char,Low_Char;
	 
	 for (int i=0;i<4;i++)
		 {
		 Hi_Char=temp_buffer[i];
		 Hi_Char=Hi_Char&0xff00;
		 Hi_Char=Hi_Char>>8;
		 Low_Char=temp_buffer[i];
		 Low_Char=Low_Char&0x00ff;
		 temp_buffer_Char[2*i]=Hi_Char;
		 temp_buffer_Char[2*i+1]=Low_Char;

		 } 

	 for (int i=0;i<THE_CHAR_LENGTH;i++)
		 {
		 p[i] =(unsigned char)temp_buffer_Char[i];

		 }

	 str_temp.Format(_T("%c%c%c%c%c%c%c%c"),p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);

	
		 } 
	 else
		 {
		   AfxMessageBox(_T("Reading Error"));
		 }
		 return str_temp;
 }

BEGIN_EVENTSINK_MAP(CDisplayConfig, CDialog)
	ON_EVENT(CDisplayConfig, IDC_INPUT_MSFLEXGRID, DISPID_DBLCLICK, CDisplayConfig::DblClickInputMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()

void CDisplayConfig::Fresh_Grid(){
 m_FlexGrid1.put_Rows(m_display_number);
 m_FlexGrid1.put_Cols(2);
 m_FlexGrid1.put_TextMatrix(0,0,_T("Seq_NO"));
 m_FlexGrid1.put_TextMatrix(0,1,_T("Display_Choice"));
 m_FlexGrid1.put_ColWidth(0,800);
 m_FlexGrid1.put_ColWidth(1,2200);
 for (int i=1;i<=m_display_number-1;i++)
 {
   CString str_No;
   str_No.Format(_T("%d"),i);
   m_FlexGrid1.put_TextMatrix(i,0,str_No);
   m_FlexGrid1.put_TextMatrix(i,1,m_Choice_Array[i-1]);
 }

}
void CDisplayConfig::DblClickInputMsflexgrid()
{
if(g_OutPutLevel==1)
	return;
	long lRow,lCol;
	lRow = m_FlexGrid1.get_RowSel();//��ȡ������к�	
	lCol = m_FlexGrid1.get_ColSel(); //��ȡ������к�
	if((lCol==0))
		return;
	if(lRow>m_FlexGrid1.get_Rows()) //����������������кţ���������Ч��
		return; 
	if(lRow == 0) //�����������У�Ҳ��Ч
		return;
	CRect rect;
	m_FlexGrid1.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
	ScreenToClient(rect); //ת��Ϊ�ͻ�������
	CDC* pDC =GetDC();
	//�������ص��羵�ת������
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
	long y = m_FlexGrid1.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid1.get_ColPos(lCol)/nTwipsPerDotX;
	//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
	long width = m_FlexGrid1.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid1.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//�γ�ѡ�и����ڵľ�������
	CRect rc(x,y,x+width,y+height);
	//ת������ԶԻ��������
	rc.OffsetRect(rect.left+1,rect.top+1);
	//��ȡѡ�и���ı���Ϣ
	CString strValue = m_FlexGrid1.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	 
	 
	if(lCol==1)
		{
		/*
		lcd rotate enable, 
		decide how many items will be shown on rotate mode,range0 to 21.
		Display item select:0:none 
		1:temperature 
		2:setpoint 
		3:AI1 
		4:AI2 
		5:AI3 
		6:AI4 
		7:AI5 
		8:AI6 
		9:AI7 
		10:AI8 
		11:MODE 
		12:USER INFO 
		13:CLOCK DATE 
		14:CLOCK TIME (TBD:OUTPUT)
		*/
		m_ItemValueCombx.ResetContent();
		m_ItemValueCombx.AddString(_T("temperature"));
		m_ItemValueCombx.AddString(_T("setpoint"));
		m_ItemValueCombx.AddString(_T("AI1"));
		m_ItemValueCombx.AddString(_T("AI2"));
		m_ItemValueCombx.AddString(_T("AI3"));
		m_ItemValueCombx.AddString(_T("AI4"));
		m_ItemValueCombx.AddString(_T("AI5"));
		m_ItemValueCombx.AddString(_T("AI6"));
		m_ItemValueCombx.AddString(_T("AI7"));
		m_ItemValueCombx.AddString(_T("AI8"));
		m_ItemValueCombx.AddString(_T("MODE"));
		m_ItemValueCombx.AddString(_T("USER INFO"));
		m_ItemValueCombx.AddString(_T("CLOCK DATE"));
		m_ItemValueCombx.AddString(_T("CLOCK TIME"));
		m_ItemValueCombx.ShowWindow(SW_SHOW);//��ʾ�ؼ�
		m_ItemValueCombx.MoveWindow(rc); //�ƶ���ѡ�и��λ�ã�����
		m_ItemValueCombx.BringWindowToTop();
		m_ItemValueCombx.SelectString(-1,strValue); //����ȫѡ������ֱ���޸�		
		m_ItemValueCombx.SetFocus(); //��ȡ����
		}
}


void CDisplayConfig::OnEnKillfocusInput9()
{
   UpdateData(TRUE);
   //д�Ĵ���
   /*int jugde=m_display_number+1*/
   if (m_display_number>14||m_display_number<0)
   {
     AfxMessageBox(_T("Input Error"));
   } 
   else
   {
     Fresh_Grid();
   }
  
}

void CDisplayConfig::OnCbnSelchangeSeqCom()
{
  //UpdateData(TRUE);
  //m_ItemValueCombx.GetWindowText(m_Choice_Array[m_nCurRow-1]);
  //AfxMessageBox(m_Choice_Array[m_nCurRow-1]);
}
BOOL CDisplayConfig::UpdateTextToReg(CString input_str,UINT reg){
 

	//input_str.TrimRight();
	if (input_str.GetLength()>8)
		{
		//AfxMessageBox(_T("More than 8 chars"));
		return FALSE;
		} 
	else
		{
		unsigned char p[8];
		for(int i=1;i<=THE_CHAR_LENGTH;i++)
			{
			if(i<input_str.GetLength()+1)
				p[i-1]=input_str.GetAt(i-1);
			else
				p[i-1]=' ';
			}
		if(WriteCharsToReg(p,reg,8))
			{
			 return TRUE;
			}
		else
			{
			//AfxMessageBox(_T("Error"));
			return FALSE;
			}
		}
}
//void CDisplayConfig::OnEnKillfocusLine1()
//{  CString input;
//   GetDlgItem(IDC_LINE_1)->GetWindowText(input);
//   UpdateTextToReg(input,425);
//}

BOOL CDisplayConfig::WriteCharsToReg(unsigned char *to_write,unsigned short start_address,int length){
       

    int ret=Write_Multi(g_tstat_id,to_write,start_address,length);
	if (ret==1)
	{
	  return TRUE;
	} 
	else
	{
	  return FALSE;
	}
	
}
BOOL CDisplayConfig::ReadCharsFromReg(unsigned short *put_data_into_here,unsigned short start_address,int length){

int ret=Read_Multi(g_tstat_id,put_data_into_here,start_address,length);
if (ret>0)
{
  return TRUE;
} 
else
{
  return FALSE;
}
}


void CDisplayConfig::OnBnClickedCancel()
{
// TODO: Add your control notification handler code here
CDialog::OnCancel();
}

void CDisplayConfig::OnBnClickedOk()
	{
	//GetDlgItem(IDC_LINE_1)->SetWindowText(GetTextFromReg(425));
	GetDlgItem(IDC_LINE_2)->SetWindowText(GetTextFromReg(429));
	}
void CDisplayConfig::OnBnClickedSave2()
{
   CString input;
   GetDlgItem(IDC_LINE_1)->GetWindowText(input);
   input.TrimRight();
   if (input.GetLength()>8)
	   {
	    
	   GetDlgItem(IDC_LINE1)->SetWindowText(_T(">8 chars"));
	    
	   } 
   else
	   {
        if (UpdateTextToReg(input,425))
        {
		   GetDlgItem(IDC_LINE1)->SetWindowText(_T("OK"));
        } 
        else
        {
		 GetDlgItem(IDC_LINE1)->SetWindowText(_T("Error"));
        }
        
       }
}

void CDisplayConfig::OnBnClickedSaveLine2()
{
CString input;
GetDlgItem(IDC_LINE_2)->GetWindowText(input);
input.TrimRight();
AfxMessageBox(_T("OK"));
if (input.GetLength()>8)
	{

	GetDlgItem(IDC_LINE2)->SetWindowText(_T(">8 chars"));

	} 
else
	{
	unsigned char p[8];
	for(int i=1;i<=THE_CHAR_LENGTH;i++)
		{
		if(i<input.GetLength()+1)
			p[i-1]=input.GetAt(i-1);
		else
			p[i-1]=' ';
		}
	if (UpdateTextToReg(input,429)Write_Multi(g_tstat_id,to_write,start_address,length))
		{
		GetDlgItem(IDC_LINE2)->SetWindowText(_T("OK"));
		} 
	else
		{
		GetDlgItem(IDC_LINE2)->SetWindowText(_T("Error"));
		}

	}
}
