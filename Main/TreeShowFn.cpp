//��ʾ���ĺ���
//�����ؼ������������齨ģ�͵�
//��ģ���������ʵ��
#include "stdafx.h"
#include "resource.h"
#include "NPartTypeSet.h"
#include "partmdlrecordset.h"
#include "NPartInsInfoTable.h"
#include "NAsmType.h"
#include "NProductInsInfoTable.h"
#include "AsmManageTableSet.h"
//////////////////////////////////////////////////////////////////////////

//#include "partmdlrecordset.h"
#include "NPartTypeSet.h"
//#include "AsmManageTableSet.h"
#include "PartInsTableSet.h"
#include "Part_Temp_Mdl_Info.h"
#include "AsmTempManageTableSet.h"
//////////////////////////////////////////////////////////////////////////
//��ʾ���ģ�͵�����Ϣ
void AddNPartMdlIns(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item)
{
	CNPartInsInfoTable m_set(p_db);
	HTREEITEM          tempItem;
	int				   nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_set.m_strFilter.Format("[TypeNum]=%ld",(long)m_Tree->GetItemData(item));
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			tempItem=m_Tree->InsertItem(nMarsk,_T(m_set.m_Name+"("+m_set.m_InsCode+")"),
				1, 1, 0, 0, m_set.m_PrtNum, item, NULL);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
}
//��ģ��Ŀ¼������Ӿ����ģ��
void AddNPartMdl(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item)
{
	CPartMdlRecordSet  m_set(p_db);
	HTREEITEM          tempItem;
	int                nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_set.m_strFilter.Format("[TypeNum]=%ld",(long)m_Tree->GetItemData(item));
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name+"("+m_set.m_Code +")"), 
				1, 1, 0, 0, m_set.m_Number, item, NULL);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
}
void AddNPartSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item,bool IfNeedMdl=false)
{
	CNPartTypeSet m_set(p_db);
	HTREEITEM  tempItem;
	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_set.m_strFilter.Format("[FatherNum]=%ld",(long)m_Tree->GetItemData(item));
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name+"("+m_set.m_Code+")"), 
				0, 0, 0, 0, m_set.m_Number, item, NULL);
			AddNPartSubItem(m_Tree,m_set.m_pDatabase,tempItem,IfNeedMdl);
			if (IfNeedMdl)
				AddNPartMdl(m_Tree,m_set.m_pDatabase,tempItem);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
}
/************************************************************************/
/* ��ʾ���ģ�͵����ṹ��ͼ������֣�һ�־���Ŀ¼(0)��һ�������ģ��(1)��һ����
���ģ�͵�ʵ��(2)                                                       */
/************************************************************************/
BOOL ShowNPartTree(CTreeCtrl * m_Tree,CDatabase * p_db,bool IfNeedMdl)
{
	CNPartTypeSet m_set(p_db);
	m_Tree->DeleteAllItems();
	HTREEITEM  hItemRoot;
	HTREEITEM  tempItem;
	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	hItemRoot=m_Tree->InsertItem(_T("���ģ��"));
	m_set.m_strFilter="[FatherNum]=0";
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name+"("+m_set.m_Code+")"), 
				0, 0, 0, 0, m_set.m_Number, hItemRoot, NULL);
			AddNPartSubItem(m_Tree,m_set.m_pDatabase,tempItem,IfNeedMdl);
			if (IfNeedMdl)
				AddNPartMdl(m_Tree,m_set.m_pDatabase,tempItem);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
	m_Tree->Expand(hItemRoot,TVE_EXPAND);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//��ʾ�齨ģ�͵�����Ϣ
void AddNAsmMdlIns(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item)
{
}
//��ģ��Ŀ¼������Ӿ����ģ��
void AddNAsmMdl(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item)
{
	CAsmManageTableSet  m_set(p_db);
	HTREEITEM          tempItem;
	int                nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_set.m_strFilter.Format("[TypeNum]=%ld",(long)m_Tree->GetItemData(item));
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name+"("+m_set.m_Code +")"), 
				2, 2, 0, 0, m_set.m_Number, item, NULL);
			AddNAsmMdlIns(m_Tree,m_set.m_pDatabase,tempItem);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
}
void AddNAsmSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item,bool IfNeedMdl=false)
{
	CNAsmType  m_set(p_db);
	HTREEITEM  tempItem;
	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_set.m_strFilter.Format("[FatherNum]=%ld",(long)m_Tree->GetItemData(item));
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name+"("+m_set.m_Code +")"), 
				0, 0, 0, 0, m_set.m_Number, item, NULL);
			AddNAsmSubItem(m_Tree,m_set.m_pDatabase,tempItem,IfNeedMdl);
			if (IfNeedMdl)
				AddNAsmMdl(m_Tree,m_set.m_pDatabase,tempItem);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
}
/************************************************************************/
/* ��ʾ���ģ�͵����ṹ��ͼ������֣�һ�־���Ŀ¼(0)��һ�������ģ��(1)��һ����
���ģ�͵�ʵ��(2)                                                       */
/************************************************************************/
BOOL ShowNAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db,bool IfNeedMdl)
{
	CNAsmType m_set(p_db);
	m_Tree->DeleteAllItems();
	HTREEITEM  hItemRoot;
	HTREEITEM  tempItem;
	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	hItemRoot=m_Tree->InsertItem(_T("���/��Ʒ"));
	m_set.m_strFilter="[FatherNum]=0";
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name+"("+m_set.m_Code +")"), 
				0, 0, 0, 0, m_set.m_Number, hItemRoot, NULL);
			AddNAsmSubItem(m_Tree,m_set.m_pDatabase,tempItem,IfNeedMdl);
			if (IfNeedMdl)
				AddNAsmMdl(m_Tree,m_set.m_pDatabase,tempItem);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
	m_Tree->Expand(hItemRoot,TVE_EXPAND);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//void AddPartItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item)
//{
//	CNPartInsInfoTable m_set(p_db);
//	HTREEITEM          tempItem;
//	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
//	m_set.m_strFilter.Format("[TypeNum]=%ld",(long)m_Tree->GetItemData(item));
//	m_set.Open();
//	if (!m_set.IsBOF())
//	{
//		m_set.MoveFirst();
//		do {
//			tempItem=m_Tree->InsertItem(nMarsk,_T(m_set.m_Name), 
//				0, 0, 0, 0,m_set.m_PrtNum, item, NULL);
//			m_set.MoveNext();
//		} while(!m_set.IsEOF());
//	}
//}
/************************************************************************/
/* ģ�͵�����                                                           */
/************************************************************************/
void AddPartTypeSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item)
{
	CNPartTypeSet m_set(p_db);
	HTREEITEM  tempItem;
	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_set.m_strFilter.Format("[FatherNum]=%ld",(long)m_Tree->GetItemData(item));
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name), 
				0, 0, 0, 0, m_set.m_Number, item, NULL);
			AddPartTypeSubItem(m_Tree,m_set.m_pDatabase,tempItem);
			/*AddPartItem(m_Tree,m_set.m_pDatabase,tempItem);*/
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
}
/************************************************************************/
/* ��ʾ�������                                                         */
/************************************************************************/
BOOL ShowPartTypeTree(CTreeCtrl * m_Tree,CDatabase * p_db)
{
	CNPartTypeSet m_set(p_db);
	m_Tree->DeleteAllItems();
	HTREEITEM  hItemRoot;
	HTREEITEM  tempItem;
	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	hItemRoot=m_Tree->InsertItem(_T("���"));
	m_set.m_strFilter="[FatherNum]=0";
	try
	{
		m_set.Open();
		if (!m_set.IsBOF())
		{
			m_set.MoveFirst();
			do {
				tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name), 
					0, 0, 0, 0, m_set.m_Number, hItemRoot, NULL);
				AddPartTypeSubItem(m_Tree,m_set.m_pDatabase,tempItem);
				m_set.MoveNext();
			} while(!m_set.IsEOF());
		}
		m_Tree->Expand(hItemRoot,TVE_EXPAND);
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////
///************************************************************************/
/* ��ʱ���������Ŀ¼����                                               */
/************************************************************************/
void AddTempPartSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM hItem)
{
	CPart_Temp_Mdl_Info m_pSet(p_db);
	int i=0,j=0;
	int nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	HTREEITEM tempItem;
	m_pSet.m_strFilter.Format("[FatherNum]=%ld",(long)m_Tree->GetItemData(hItem));
	m_pSet.Open();
	if(m_pSet.IsBOF()) return;
	m_pSet.MoveFirst();
	do {
		if(m_pSet.m_Type!="mdl")
		{
			tempItem= m_Tree->InsertItem(nMarsk,_T(m_pSet.m_Name), 
				0, 0, 0, 0, m_pSet.m_Number, hItem, NULL);
			int num=m_pSet.m_Number;
			AddTempPartSubItem(m_Tree,m_pSet.m_pDatabase,tempItem);
		}
		m_pSet.MoveNext();
	} while(!m_pSet.IsEOF());
}
BOOL ShowTempPartTree(CTreeCtrl * m_Tree,CDatabase * p_db)
{
	CPart_Temp_Mdl_Info m_pSet(p_db);
	//��Ӹ��ڵ�
	m_Tree->DeleteAllItems();
	HTREEITEM hItemRoot=m_Tree->InsertItem(_T("��������ռ�"));
	m_pSet.m_strFilter=_T("[FatherNum]=0");
	m_pSet.Open();
	if(m_pSet.IsBOF()) return TRUE;
	m_pSet.MoveFirst();
	do {
		HTREEITEM hItem=m_Tree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T(m_pSet.m_Name), 
			0, 0, 0, 0, m_pSet.m_Number, hItemRoot, NULL);
		int num=m_pSet.m_Number;
		AddTempPartSubItem(m_Tree,m_pSet.m_pDatabase,hItem);
		m_pSet.MoveNext();
	} while(!m_pSet.IsEOF());
	m_Tree->Expand(hItemRoot,TVE_EXPAND);
	return TRUE;
}

/************************************************************************/
/* ��ʱ���������Ŀ¼����                                               */
/************************************************************************/
void AddTempAsmSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM hItem)
{
	CAsmTempManageTableSet m_pSet(p_db);
	int i=0,j=0;
	int nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	HTREEITEM tempItem;
	m_pSet.m_strFilter.Format("[FatherNumber]=%ld",(long)m_Tree->GetItemData(hItem));
	m_pSet.Open();
	if(m_pSet.IsBOF()) return;
	m_pSet.MoveFirst();
	do {
		if(m_pSet.m_Type!="mdl")
		{
			tempItem= m_Tree->InsertItem(nMarsk,_T(m_pSet.m_Name), 
				0, 0, 0, 0, m_pSet.m_Number, hItem, NULL);
			int num=m_pSet.m_Number;
			AddTempAsmSubItem(m_Tree,m_pSet.m_pDatabase,tempItem);
		}
		m_pSet.MoveNext();
	} while(!m_pSet.IsEOF());
}

BOOL ShowTempAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db)
{
	CAsmTempManageTableSet m_pSet(p_db);
	m_Tree->DeleteAllItems();
	HTREEITEM hItemRoot=m_Tree->InsertItem(_T("��������ռ�"));
	m_pSet.m_strFilter="[FatherNumber]=0";
	m_pSet.Open();
	if(m_pSet.IsBOF()) return TRUE;
	m_pSet.MoveFirst();
	do {
		HTREEITEM hItem=m_Tree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T(m_pSet.m_Name), 
			0, 0, 0, 0, m_pSet.m_Number, hItemRoot, NULL);
		int num=m_pSet.m_Number;
		AddTempAsmSubItem(m_Tree,m_pSet.m_pDatabase,hItem);
		m_pSet.MoveNext();
	} while(!m_pSet.IsEOF());
	m_Tree->Expand(hItemRoot,TVE_EXPAND);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
////��ʾ���ʵ��
//void AddAsmMdlIns(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item)
//{
//	CNProductInsInfoTable  m_set(p_db);
//	HTREEITEM  tempItem;
//	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
//	m_set.m_strFilter.Format("[TypeNum]=%ld",(long)m_Tree->GetItemData(item));
//	m_set.Open();
//	if (!m_set.IsBOF())
//	{
//		m_set.MoveFirst();
//		do {
//			tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name+"("+m_set.m_InsCode+")"), 
//				2, 2, 0, 0, m_set.m_PdtNum, item, NULL);
//			m_set.MoveNext();
//		} while(!m_set.IsEOF());
//	}
//}
void AddAsmSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM item)
{
	CNAsmType  m_set(p_db);
	HTREEITEM  tempItem;
	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_set.m_strFilter.Format("[FatherNum]=%ld",(long)m_Tree->GetItemData(item));
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name+"("+m_set.m_Code +")"), 
				0, 0, 0, 0, m_set.m_Number, item, NULL);
			AddNAsmSubItem(m_Tree,m_set.m_pDatabase,tempItem);
			/*AddAsmMdlIns(m_Tree,m_set.m_pDatabase,tempItem);*/
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
}
BOOL ShowAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db)
{
	CNAsmType m_set(p_db);
	m_Tree->DeleteAllItems();
	HTREEITEM  hItemRoot;
	HTREEITEM  tempItem;
	int        nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	hItemRoot=m_Tree->InsertItem(_T("���/��Ʒ"));
	m_set.m_strFilter="[FatherNum]=0";
	m_set.Open();
	if (!m_set.IsBOF())
	{
		m_set.MoveFirst();
		do {
			tempItem = m_Tree->InsertItem(nMarsk,_T(m_set.m_Name+"("+m_set.m_Code +")"), 
				0, 0, 0, 0, m_set.m_Number, hItemRoot, NULL);
			AddAsmSubItem(m_Tree,m_set.m_pDatabase,tempItem);
			m_set.MoveNext();
		} while(!m_set.IsEOF());
	}
	m_Tree->Expand(hItemRoot,TVE_EXPAND);
	return TRUE;
}
//void AddAsmSubItem(CTreeCtrl * m_Tree,CDatabase * p_db,HTREEITEM hItem)
//{
//	//HTREEITEM tempItem;
//	//   CAsmManageTableSet m_pSet(p_db);
//	//m_pSet.m_strFilter.Format("FatherNumber=%ld",(long)m_Tree->GetItemData(hItem));
//	//m_pSet.Open();
//	//if(m_pSet.IsBOF()) return;
//	//m_pSet.MoveFirst();
//	//do {
//	//	int i=0,j=0;
//	//	int nMarsk=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
//	//	if(m_pSet.m_Type=="mdl") i=2,j=2;
//	//	tempItem= m_Tree->InsertItem(nMarsk,_T(m_pSet.m_Name), 
//	//		i, j, 0, 0, m_pSet.m_Number, hItem, NULL);
//	//	int num=m_pSet.m_Number;
//	//	AddAsmSubItem(m_Tree,m_pSet.m_pDatabase,tempItem);
//	//	m_pSet.MoveNext();
//	//} while(!m_pSet.IsEOF());
//}
///************************************************************************/
///* ��ʾ���ģ�͵�ģ����                                                 */
///************************************************************************/
//BOOL ShowAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db)
//{
//	CAsmManageTableSet m_pSet(p_db);
//	m_Tree->DeleteAllItems();
//	HTREEITEM hItemRoot=m_Tree->InsertItem(_T("װ��ģ��"));
//	m_pSet.m_strFilter="[FatherNumber]=0";
//	if(!m_pSet.Open())
//	{
//		AfxMessageBox("���ݿ����Ӳ��ɹ���");
//		return FALSE;
//	}
//	if(m_pSet.IsBOF())
//	{
//		return TRUE;
//	}
//	m_pSet.MoveFirst();
//	do {
//		HTREEITEM hItem=m_Tree->InsertItem(TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE,_T(m_pSet.m_Name), 
//			0, 0, 0, 0, m_pSet.m_Number, hItemRoot, NULL);
//		int num=m_pSet.m_Number;
//		AddAsmSubItem(m_Tree,m_pSet.m_pDatabase,hItem);
//		m_pSet.MoveNext();
//	} while(!m_pSet.IsEOF());
//	m_Tree->Expand(hItemRoot,TVE_EXPAND);
//	return TRUE;
//}