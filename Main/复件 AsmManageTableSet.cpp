// AsmManageTableSet.h : CAsmManageTableSet ���ʵ��



// CAsmManageTableSet ʵ��

// ���������� 2005��7��5��, 15:46

#include "stdafx.h"
#include "AsmManageTableSet.h"
extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmManageTableSet, CRecordset)

CAsmManageTableSet::CAsmManageTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Number = 0;
	m_FatherNumber = 0;
	m_Name = "";
	m_Type = "";
	m_MdlCode = "";
	m_Datatime;
	m_FileName = "";
	m_FileDirectory = "";
	m_MdlerName = "";
	m_MdlerDepartment = "";
	m_Note = "";
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
	return _T("[dbo].[AsmManageTable]");
}

void CAsmManageTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Long(pFX, _T("[FatherNumber]"), m_FatherNumber);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Text(pFX, _T("[MdlCode]"), m_MdlCode);
	RFX_Date(pFX, _T("[Datatime]"), m_Datatime);
	RFX_Text(pFX, _T("[FileName]"), m_FileName);
	RFX_Text(pFX, _T("[FileDirectory]"), m_FileDirectory);
	RFX_Text(pFX, _T("[MdlerName]"), m_MdlerName);
	RFX_Text(pFX, _T("[MdlerDepartment]"), m_MdlerDepartment);
	RFX_Text(pFX, _T("[Note]"), m_Note);

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


