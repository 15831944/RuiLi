// partInsValueTableSet.h : CpartInsValueTableSet ���ʵ��



// CpartInsValueTableSet ʵ��

// ���������� 2005��6��20��, 10:44

#include "stdafx.h"
#include "partInsValueTableSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CpartInsValueTableSet, CRecordset)

CpartInsValueTableSet::CpartInsValueTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_Num = 0;
	m_Value = "";
	m_nFields = 3;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CpartInsValueTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CpartInsValueTableSet::GetDefaultSQL()
{
	return _T("[dbo].[NPartInsVariantTable]");
}

void CpartInsValueTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[Num]"), m_Num);
	RFX_Text(pFX, _T("[Value]"), m_Value);

}
/////////////////////////////////////////////////////////////////////////////
// CpartInsValueTableSet ���

#ifdef _DEBUG
void CpartInsValueTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CpartInsValueTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


