// CAsmCompVariantTableSet.h : CAsmCompVariantTableSet ���ʵ��



// CAsmCompVariantTableSet ʵ��

// ���������� 2005��7��20��, 13:34

#include "stdafx.h"
#include "CAsmCompVariantTableSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmCompVariantTableSet, CRecordset)

CAsmCompVariantTableSet::CAsmCompVariantTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_CompNum = 0;
	m_CompMdlNum = 0;
	m_CompInsNum = 0;
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmCompVariantTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmCompVariantTableSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmCompVariantTable]");
}

void CAsmCompVariantTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[CompNum]"), m_CompNum);
	RFX_Long(pFX, _T("[CompMdlNum]"), m_CompMdlNum);
	RFX_Long(pFX, _T("[CompInsNum]"), m_CompInsNum);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmCompVariantTableSet ���

#ifdef _DEBUG
void CAsmCompVariantTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmCompVariantTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


