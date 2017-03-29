// LstCrlOfSXTXTable.cpp : 实现文件
//没有多大用处，但不可删除

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



// CLstCrlOfSXTXTable 消息处理程序


void CLstCrlOfSXTXTable::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//GetCurrentMessage()->pt;
	this->GetEditControl();
	*pResult = 0;
}
