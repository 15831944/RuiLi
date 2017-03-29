// AsmPerformanceInsTableSet.h : CAsmPerformanceInsTableSet ���ʵ��



// CAsmPerformanceInsTableSet ʵ��

// ���������� 2005��9��7��, 9:47

#include "stdafx.h"
#include "AsmPerformanceInsTableSet.h"
IMPLEMENT_DYNAMIC(CAsmPerformanceInsTableSet, CRecordset)

CAsmPerformanceInsTableSet::CAsmPerformanceInsTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_InsNum = 0;
	m_Num = 0;
	m_Value = "";
	m_Note = "";
	m_nFields = 5;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmPerformanceInsTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmPerformanceInsTableSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmPerformanceInsTable]");
}

void CAsmPerformanceInsTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[InsNum]"), m_InsNum);
	RFX_Long(pFX, _T("[Num]"), m_Num);
	RFX_Text(pFX, _T("[Value]"), m_Value);
	RFX_Text(pFX, _T("[Note]"), m_Note);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmPerformanceInsTableSet ���

#ifdef _DEBUG
void CAsmPerformanceInsTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmPerformanceInsTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


