// PartRelativeDimParaTableSet.h : CPartRelativeDimParaTableSet ���ʵ��



// CPartRelativeDimParaTableSet ʵ��

// ���������� 2005��6��15��, 21:14

#include "stdafx.h"
#include "PartRelativeDimParaTableSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CPartRelativeDimParaTableSet, CRecordset)

CPartRelativeDimParaTableSet::CPartRelativeDimParaTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_Number = 0;
	m_Type = "";
	m_IdNum = "";
	m_DimSymbol = "";
	m_Name = "";
	m_SubType = "";
	m_Value = "";
	m_nFields = 8;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CPartRelativeDimParaTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CPartRelativeDimParaTableSet::GetDefaultSQL()
{
	return _T("[dbo].[NPartRelativeDimParaTable]");
}

void CPartRelativeDimParaTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Text(pFX, _T("[IdNum]"), m_IdNum);
	RFX_Text(pFX, _T("[DimSymbol]"), m_DimSymbol);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[SubType]"), m_SubType);
	RFX_Text(pFX, _T("[Value]"), m_Value);

}
/////////////////////////////////////////////////////////////////////////////
// CPartRelativeDimParaTableSet ���

#ifdef _DEBUG
void CPartRelativeDimParaTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPartRelativeDimParaTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


