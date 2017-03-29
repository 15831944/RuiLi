// AsmInsInfoTableSet.h : CAsmInsInfoTableSet ���ʵ��



// CAsmInsInfoTableSet ʵ��

// ���������� 2005��9��7��, 9:46

#include "stdafx.h"
#include "AsmInsInfoTableSet.h"
IMPLEMENT_DYNAMIC(CAsmInsInfoTableSet, CRecordset)

CAsmInsInfoTableSet::CAsmInsInfoTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_InsNum = 0;
	m_InsName = "";
	m_InsCode = "";
	m_SavePlace = "";
	m_InsCreateTime;
	m_InsCreaterName = "";
	m_InsCreateDpt = "";
	m_InsNote = "";
	m_nFields = 9;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmInsInfoTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmInsInfoTableSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmInsInfoTable]");
}

void CAsmInsInfoTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[InsNum]"), m_InsNum);
	RFX_Text(pFX, _T("[InsName]"), m_InsName);
	RFX_Text(pFX, _T("[InsCode]"), m_InsCode);
	RFX_Text(pFX, _T("[SavePlace]"), m_SavePlace);
	RFX_Date(pFX, _T("[InsCreateTime]"), m_InsCreateTime);
	RFX_Text(pFX, _T("[InsCreaterName]"), m_InsCreaterName);
	RFX_Text(pFX, _T("[InsCreateDpt]"), m_InsCreateDpt);
	RFX_Text(pFX, _T("[InsNote]"), m_InsNote);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmInsInfoTableSet ���

#ifdef _DEBUG
void CAsmInsInfoTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmInsInfoTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


