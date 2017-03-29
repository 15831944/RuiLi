// AsmManageTableSet.h : CAsmManageTableSet ���ʵ��



// CAsmManageTableSet ʵ��

// ���������� 2005��7��5��, 15:46

#include "stdafx.h"
#include "AsmManageTableSet.h"
//extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmManageTableSet, CRecordset)

CAsmManageTableSet::CAsmManageTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Number=0;
	m_Name="";
	m_Code="";
	m_TypeNum=0;
	m_SolidFileName="";
	m_SolidFilePlace="";
	m_DrawingFileName="";
	m_DrawingFilePlace="";
	m_Datatime;
	m_Note="";
	m_Designer="";
	m_nFields = 11;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmManageTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmManageTableSet::GetDefaultSQL()
{
	return _T("[dbo].[NAsmManageTable]");
}

void CAsmManageTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Long(pFX, _T("[TypeNum]"), m_TypeNum);
	RFX_Date(pFX, _T("[Datatime]"), m_Datatime);
	RFX_Text(pFX, _T("[Code]"), m_Code);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Text(pFX, _T("[SolidFileName]"), m_SolidFileName);
	RFX_Text(pFX, _T("[SolidFilePlace]"), m_SolidFilePlace);
	RFX_Text(pFX, _T("[DrawingFileName]"), m_DrawingFileName);
	RFX_Text(pFX, _T("[DrawingFilePlace]"), m_DrawingFilePlace);
	RFX_Text(pFX, _T("[Designer]"), m_Designer);
}
/////////////////////////////////////////////////////////////////////////////
// CAsmManageTableSet ���

#ifdef _DEBUG
void CAsmManageTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmManageTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


