// Part_Temp_Rel.h : CPart_Temp_Rel ���ʵ��



// CPart_Temp_Rel ʵ��

// ���������� 2005��6��25��, 9:39

#include "stdafx.h"
#include "Part_Temp_Rel.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CPart_Temp_Rel, CRecordset)

CPart_Temp_Rel::CPart_Temp_Rel(CDatabase* pdb)
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
CString CPart_Temp_Rel::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CPart_Temp_Rel::GetDefaultSQL()
{
	return _T("[dbo].[Part_Temp_Rel]");
}

void CPart_Temp_Rel::DoFieldExchange(CFieldExchange* pFX)
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
// CPart_Temp_Rel ���

#ifdef _DEBUG
void CPart_Temp_Rel::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPart_Temp_Rel::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


