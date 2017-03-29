// AsmInsInfoTable.h : CAsmInsInfoTable ���ʵ��



// CAsmInsInfoTable ʵ��

// ���������� 2005��7��29��, 9:30

#include "stdafx.h"
#include "AsmInsInfoTable.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmInsInfoTable, CRecordset)

CAsmInsInfoTable::CAsmInsInfoTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_InsNum = 0;
	m_InsName = "";
	m_InsCode = "";
	m_InsCreateTime;
	m_InsCreaterName = "";
	m_InsCreateDpt = "";
	m_InsNote = "";
	m_nFields = 8;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmInsInfoTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmInsInfoTable::GetDefaultSQL()
{
	return _T("[dbo].[AsmInsInfoTable]");
}

void CAsmInsInfoTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[InsNum]"), m_InsNum);
	RFX_Text(pFX, _T("[InsName]"), m_InsName);
	RFX_Text(pFX, _T("[InsCode]"), m_InsCode);
	RFX_Date(pFX, _T("[InsCreateTime]"), m_InsCreateTime);
	RFX_Text(pFX, _T("[InsCreaterName]"), m_InsCreaterName);
	RFX_Text(pFX, _T("[InsCreateDpt]"), m_InsCreateDpt);
	RFX_Text(pFX, _T("[InsNote]"), m_InsNote);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmInsInfoTable ���

#ifdef _DEBUG
void CAsmInsInfoTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmInsInfoTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


