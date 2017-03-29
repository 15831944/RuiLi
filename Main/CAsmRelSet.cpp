// CAsmRelSet.h : CAsmRelSet ���ʵ��



// CAsmRelSet ʵ��

// ���������� 2005��7��8��, 13:01

#include "stdafx.h"
#include "CAsmRelSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmRelSet, CRecordset)

CAsmRelSet::CAsmRelSet(CDatabase* pdb)
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
CString CAsmRelSet::GetDefaultConnect()
{
	return _T(dbLinkInfo;);
}

CString CAsmRelSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmRelationTable]");
}

void CAsmRelSet::DoFieldExchange(CFieldExchange* pFX)
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
// CAsmRelSet ���

#ifdef _DEBUG
void CAsmRelSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmRelSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


