// AsmTempRelationTable.h : CAsmTempRelationTable ���ʵ��



// CAsmTempRelationTable ʵ��

// ���������� 2005��7��27��, 20:58

#include "stdafx.h"
#include "AsmTempRelationTable.h"
//extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmTempRelationTable, CRecordset)

CAsmTempRelationTable::CAsmTempRelationTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_Number = 0;
	m_Relation = "";
	m_Note = "";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmTempRelationTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmTempRelationTable::GetDefaultSQL()
{
	return _T("[dbo].[AsmTempRelationTable]");
}

void CAsmTempRelationTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Relation]"), m_Relation);
	RFX_Text(pFX, _T("[Note]"), m_Note);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmTempRelationTable ���

#ifdef _DEBUG
void CAsmTempRelationTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmTempRelationTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


