// partPerfparaTableSet.h : CpartPerfparaTableSet ���ʵ��



// CpartPerfparaTableSet ʵ��

// ���������� 2005��6��15��, 21:13

#include "stdafx.h"
#include "partPerfparaTableSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CpartPerfparaTableSet, CRecordset)

CpartPerfparaTableSet::CpartPerfparaTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_Number = 0;
	m_Name = "";
	m_Value = "";
	m_Note = "";
	m_Type = "";
	m_MaxVal=0.0;
	m_MinVal=0.0;
	m_nFields = 8;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CpartPerfparaTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CpartPerfparaTableSet::GetDefaultSQL()
{
	return _T("[dbo].[NPartPerfparaTable]");
}

void CpartPerfparaTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Value]"), m_Value);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Double(pFX,_T("[minValue]"),m_MinVal);
	RFX_Double(pFX,_T("[maxValue]"),m_MaxVal);

}
/////////////////////////////////////////////////////////////////////////////
// CpartPerfparaTableSet ���

#ifdef _DEBUG
void CpartPerfparaTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CpartPerfparaTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


