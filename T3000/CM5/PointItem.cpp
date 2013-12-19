//PointItem Code by Fance Du 2013 11 05
#include "stdafx.h"
#include "PointItem.h"

CPointItem::CPointItem(void)
{
 	m_point.x=0;
	m_point.y=0;
	m_index = 0;
	m_pNextItem =NULL;

//	m_CPU_Loading = GetCPUpercent();
}


CPointItem::~CPointItem(void)
{
}

void CPointItem::SetPoint(MyPoint point)  //���ô�ItemPoint������
{
	m_point.x = point.x;
	m_point.y = point.y;
}

MyPoint CPointItem::GetPoint() const		//��ȡ ����;
{
	MyPoint tempPoint;
	tempPoint.x=m_point.x;
	tempPoint.y=m_point.y;
	return tempPoint;
}



void CPointItem::SetIndex(int nIndex)	//���ô�ItemPoint�����;
{
	m_index = nIndex;
}

int CPointItem::GetIndex() const
{
	return m_index;
}


void CPointItem::SetNext(CPointItem *pnext)
{
	m_pNextItem = pnext;
}

CPointItem* CPointItem::GetNext(void) const
{
	return m_pNextItem;
}



void CPointItem::SetCPUPersent(int ncpupersent)
{
	m_CPU_Loading = ncpupersent;
}


