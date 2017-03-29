// AsmInsVariantTableSet.h : CAsmInsVariantTableSet ���ʵ��



// CAsmInsVariantTableSet ʵ��

// ���������� 2005��9��7��, 9:47

#include "stdafx.h"
#include "AsmInsVariantTableSet.h"
IMPLEMENT_DYNAMIC(CAsmInsVariantTableSet, CRecordset)

CAsmInsVariantTableSet::CAsmInsVariantTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_InsNum = 0;
	m_Num = 0;
	m_Value = "";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmInsVariantTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmInsVariantTableSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmInsVariantTable]");
}

void CAsmInsVariantTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[InsNum]"), m_InsNum);
	RFX_Long(pFX, _T("[Num]"), m_Num);
	RFX_Text(pFX, _T("[Value]"), m_Value);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmInsVariantTableSet ���

#ifdef _DEBUG
void CAsmInsVariantTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmInsVariantTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


