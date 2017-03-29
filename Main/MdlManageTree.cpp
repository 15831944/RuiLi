// MdlManageTree.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "MdlManageTree.h"

//定义宏
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



// CMdlManageTree 消息处理程序


//显示模型树
void CMdlManageTree::Show(void)
{

     if(nFlagType==TYPE_ASSEMBLY)
	 {
          HTREEITEM hItemRoot=InsertItem(_T("产品模型目录"));
	 }
     else if(nFlagType==TYPE_PART)
	 {
		 m_pPartSet=&m_pPartMdlSet;
		 HTREEITEM hItemRoot=InsertItem(_T("零件模型目录"));
         PartTreeList(hItemRoot);
		 Expand(hItemRoot,TVE_EXPAND);
	 }
	 else 
	 {
		 AfxMessageBox("请先设置类型！");
		 return;
	 }
	 
	 
}
/*********************************************************************************\
函数：SetTypeBeShow()
说明：设置需要被显示的模型树的类型，type取值为TYPE_ASSEMBLY(1),TYPE_PART(2)
\*********************************************************************************/
void CMdlManageTree::SetTypeBeShow(int type)
{
	nFlagType=type;
}
/**********************************************************************************\
函数 ：TreeList()
说明：列出模型树
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
函数：AddSubPartItem()
说明：显示零件树中除在根目录项下的子项
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
		return ;//数据非空
	}
	tempSet.MoveFirst();
	do {
		tempItem= InsertItem(TVIF_TEXT|TVIF_PARAM,_T(tempSet->m_Name), 
			0, 0, 0, 0, tempSet->m_Number, item, NULL);
		AddSubPartItem(tempItem);
	} while(!tempSet->IsEOF());
    tempSet->Close();
}