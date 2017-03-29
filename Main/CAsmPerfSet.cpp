// CAsmPerfSet.h : CAsmPerfSet ���ʵ��



// CAsmPerfSet ʵ��

// ���������� 2005��7��8��, 13:01

#include "stdafx.h"
#include "CAsmPerfSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmPerfSet, CRecordset)

CAsmPerfSet::CAsmPerfSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
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
CString CAsmPerfSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmPerfSet::GetDefaultSQL()
{
	return _T("[dbo].[NAsmPerformanceTable]");
}

void CAsmPerfSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Value]"), m_Value);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Double(pFX,_T("[minValue]"),m_MinVal);
	RFX_Double(pFX,_T("[maxValue]"),m_MaxVal);
}
/////////////////////////////////////////////////////////////////////////////
// CAsmPerfSet ���

#ifdef _DEBUG
void CAsmPerfSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmPerfSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


