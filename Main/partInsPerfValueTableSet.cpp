// partInsPerfValueTableSet.h : CpartInsPerfValueTableSet ���ʵ��



// CpartInsPerfValueTableSet ʵ��

// ���������� 2005��6��20��, 10:47

#include "stdafx.h"
#include "partInsPerfValueTableSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CpartInsPerfValueTableSet, CRecordset)

CpartInsPerfValueTableSet::CpartInsPerfValueTableSet(CDatabase* pdb)
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
CString CpartInsPerfValueTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CpartInsPerfValueTableSet::GetDefaultSQL()
{
	return _T("[dbo].[partInsPerfValueTable]");
}

void CpartInsPerfValueTableSet::DoFieldExchange(CFieldExchange* pFX)
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
// CpartInsPerfValueTableSet ���

#ifdef _DEBUG
void CpartInsPerfValueTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CpartInsPerfValueTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


