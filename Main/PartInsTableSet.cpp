// PartInsTableSet.h : CPartInsTableSet ���ʵ��



// CPartInsTableSet ʵ��

// ���������� 2005��6��20��, 10:43

#include "stdafx.h"
#include "PartInsTableSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CPartInsTableSet, CRecordset)

CPartInsTableSet::CPartInsTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_InsNum = 0;
	m_InsName = "";
	m_InsCode = "";
	m_InsNote = "";
	m_InsCreateTime;
	m_InsCreaterName = "";
	m_InsCreateDpt = "";
	m_nFields = 8;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CPartInsTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CPartInsTableSet::GetDefaultSQL()
{
	return _T("[dbo].[PartInsTable]");
}

void CPartInsTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[InsNum]"), m_InsNum);
	RFX_Text(pFX, _T("[InsName]"), m_InsName);
	RFX_Text(pFX, _T("[InsCode]"), m_InsCode);
	RFX_Text(pFX, _T("[InsNote]"), m_InsNote);
	RFX_Date(pFX, _T("[InsCreateTime]"), m_InsCreateTime);
	RFX_Text(pFX, _T("[InsCreaterName]"), m_InsCreaterName);
	RFX_Text(pFX, _T("[InsCreateDpt]"), m_InsCreateDpt);

}
/////////////////////////////////////////////////////////////////////////////
// CPartInsTableSet ���

#ifdef _DEBUG
void CPartInsTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPartInsTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


