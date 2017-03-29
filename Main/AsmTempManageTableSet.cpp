// AsmTempManageTableSet.h : CAsmTempManageTableSet ���ʵ��



// CAsmTempManageTableSet ʵ��

// ���������� 2005��7��25��, 9:58

#include "stdafx.h"
#include "AsmTempManageTableSet.h"
//extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmTempManageTableSet, CRecordset)

CAsmTempManageTableSet::CAsmTempManageTableSet(CDatabase* pdb)
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
	m_MdlTypeNum = 0;
	m_MdlTypeName = "";
	m_DrawFileName="";
	m_DrawFileDirectory="";
	m_nFields = 15;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmTempManageTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmTempManageTableSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmTempManageTable]");
}

void CAsmTempManageTableSet::DoFieldExchange(CFieldExchange* pFX)
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
	RFX_Text(pFX, _T("[DrawFileName]"), m_DrawFileName);
	RFX_Text(pFX, _T("[DrawFileDirectory]"), m_DrawFileDirectory);
	RFX_Text(pFX, _T("[MdlerName]"), m_MdlerName);
	RFX_Text(pFX, _T("[MdlerDepartment]"), m_MdlerDepartment);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Long(pFX, _T("[MdlTypeNum]"), m_MdlTypeNum);
	RFX_Text(pFX, _T("[MdlTypeName]"), m_MdlTypeName);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmTempManageTableSet ���

#ifdef _DEBUG
void CAsmTempManageTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmTempManageTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


