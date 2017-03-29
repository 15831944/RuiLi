// partRelationSet.h : CpartRelationSet ���ʵ��



// CpartRelationSet ʵ��

// ���������� 2005��6��15��, 21:13

#include "stdafx.h"
#include "partRelationSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CpartRelationSet, CRecordset)

CpartRelationSet::CpartRelationSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
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
CString CpartRelationSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CpartRelationSet::GetDefaultSQL()
{
	return _T("[dbo].[NPartRelationSet]");
}

void CpartRelationSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Relation]"), m_Relation);
	RFX_Text(pFX, _T("[Note]"), m_Note);

}
/////////////////////////////////////////////////////////////////////////////
// CpartRelationSet ���

#ifdef _DEBUG
void CpartRelationSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CpartRelationSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


