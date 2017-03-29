// AsmTempPerformanceTable.h : CAsmTempPerformanceTable ���ʵ��



// CAsmTempPerformanceTable ʵ��

// ���������� 2005��7��27��, 20:57

#include "stdafx.h"
#include "AsmTempPerformanceTable.h"
//extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmTempPerformanceTable, CRecordset)

CAsmTempPerformanceTable::CAsmTempPerformanceTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_Number = 0;
	m_Name = "";
	m_Value = "";
	m_Note = "";
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
CString CAsmTempPerformanceTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmTempPerformanceTable::GetDefaultSQL()
{
	return _T("[dbo].[AsmTempPerformanceTable]");
}

void CAsmTempPerformanceTable::DoFieldExchange(CFieldExchange* pFX)
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
// CAsmTempPerformanceTable ���

#ifdef _DEBUG
void CAsmTempPerformanceTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmTempPerformanceTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


