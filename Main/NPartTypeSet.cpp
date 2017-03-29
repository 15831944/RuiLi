// NPartTypeSet.h : CNPartTypeSet ���ʵ��



// CNPartTypeSet ʵ��

// ���������� 2005��9��15��, 0:18

#include "stdafx.h"
#include "NPartTypeSet.h"
IMPLEMENT_DYNAMIC(CNPartTypeSet, CRecordset)

CNPartTypeSet::CNPartTypeSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Number = 0;
	m_FatherNum = 0;
	m_Name = "";
	m_Code = "";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CNPartTypeSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}
CString CNPartTypeSet::GetDefaultSQL()
{
	return _T("[dbo].[NPartType]");
}

void CNPartTypeSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Long(pFX, _T("[FatherNum]"), m_FatherNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Code]"), m_Code);

}
/////////////////////////////////////////////////////////////////////////////
// CNPartTypeSet ���

#ifdef _DEBUG
void CNPartTypeSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNPartTypeSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


