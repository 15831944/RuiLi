// LstCrlOfSXTXTable.cpp : ʵ���ļ�
//û�ж���ô���������ɾ��

#include "stdafx.h"
#include "Main.h"
#include "LstCrlOfSXTXTable.h"


// CLstCrlOfSXTXTable

IMPLEMENT_DYNAMIC(CLstCrlOfSXTXTable, CListCtrl)
CLstCrlOfSXTXTable::CLstCrlOfSXTXTable()
{
}

CLstCrlOfSXTXTable::~CLstCrlOfSXTXTable()
{
}


BEGIN_MESSAGE_MAP(CLstCrlOfSXTXTable, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
END_MESSAGE_MAP()



// CLstCrlOfSXTXTable ��Ϣ�������


void CLstCrlOfSXTXTable::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//GetCurrentMessage()->pt;
	this->GetEditControl();
	*pResult = 0;
}
