// MdlManageTree.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "MdlManageTree.h"

//�����
#define TYPE_ASSEMBLY    1
#define TYPE_PART        2
extern CPartMdlRecordSet m_pPartMdlSet;

// CMdlManageTree

IMPLEMENT_DYNAMIC(CMdlManageTree, CTreeCtrl)
CMdlManageTree::CMdlManageTree()
: nFlagType(0)
,m_pPartSet(NULL)
{
}

CMdlManageTree::~CMdlManageTree()
{
}


BEGIN_MESSAGE_MAP(CMdlManageTree, CTreeCtrl)
END_MESSAGE_MAP()



// CMdlManageTree ��Ϣ�������


//��ʾģ����
void CMdlManageTree::Show(void)
{

     if(nFlagType==TYPE_ASSEMBLY)
	 {
          HTREEITEM hItemRoot=InsertItem(_T("��Ʒģ��Ŀ¼"));
	 }
     else if(nFlagType==TYPE_PART)
	 {
		 m_pPartSet=&m_pPartMdlSet;
		 HTREEITEM hItemRoot=InsertItem(_T("���ģ��Ŀ¼"));
         PartTreeList(hItemRoot);
		 Expand(hItemRoot,TVE_EXPAND);
	 }
	 else 
	 {
		 AfxMessageBox("�����������ͣ�");
		 return;
	 }
	 
	 
}
/*********************************************************************************\
������SetTypeBeShow()
˵����������Ҫ����ʾ��ģ���������ͣ�typeȡֵΪTYPE_ASSEMBLY(1),TYPE_PART(2)
\*********************************************************************************/
void CMdlManageTree::SetTypeBeShow(int type)
{
	nFlagType=type;
}
/**********************************************************************************\
���� ��TreeList()
˵�����г�ģ����
\**********************************************************************************/
void CMdlManageTree::PartTreeList(HTREEITEM fathernode)
{
	
	if(m_pPartSet->IsOpen()) m_pPartSet->Close();
	m_pPartSet->Open(CRecordset::dynaset,_T("SELECT * FROM [dbo].[PartManageTable] WHERE FatherNumber =0"),CRecordset::readOnly);
	if(m_pPartSet->IsBOF())
	{
		m_pPartSet->Close();
		return;
	}
    m_pPartSet->MoveFirst();
    while (!m_pPartSet->IsEOF()) {
		HTREEITEM hItem=InsertItem(TVIF_TEXT|TVIF_PARAM,_T(m_pPartSet->m_Name), 
			0, 0, 0, 0, m_pPartSet->m_Number, fathernode, NULL);
		AddSubPartItem(fathernode);
		m_pPartSet->MoveNext();
    }
	
}
/***********************************************************************************\
������AddSubPartItem()
˵������ʾ������г��ڸ�Ŀ¼���µ�����
\***********************************************************************************/
void AddSubPartItem(HTREEITEM item)
{
	CString sql_lang;
	HTREEITEM tempItem;
	sql_lang.Format("SELECT * FROM [dbo].[PartManageTable] WHERE FatherNumber =%d",
		(long)GetItemData(item));
	CPartMdlRecordSet tempSet;
	tempSet.Open(CRecordset::dynaset,_T(sql_lang.GetString()),CRecordset::readOnly);
	if(tempSet->IsBOF()!=0) 
	{
		tempSet.Close();
		return ;//���ݷǿ�
	}
	tempSet.MoveFirst();
	do {
		tempItem= InsertItem(TVIF_TEXT|TVIF_PARAM,_T(tempSet->m_Name), 
			0, 0, 0, 0, tempSet->m_Number, item, NULL);
		AddSubPartItem(tempItem);
	} while(!tempSet->IsEOF());
    tempSet->Close();
}