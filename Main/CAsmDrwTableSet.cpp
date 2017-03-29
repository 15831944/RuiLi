// CAsmDrwTableSet.h : CAsmDrwTableSet ���ʵ��



// CAsmDrwTableSet ʵ��

// ���������� 2005��7��22��, 16:03

#include "stdafx.h"
#include "CAsmDrwTableSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmDrwTableSet, CRecordset)

CAsmDrwTableSet::CAsmDrwTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_DrwNum = 0;
	m_DrwFileName = "";
	m_DrwNote = "";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmDrwTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmDrwTableSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmDrwTable]");
}

void CAsmDrwTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[DrwNum]"), m_DrwNum);
	RFX_Text(pFX, _T("[DrwFileName]"), m_DrwFileName);
	RFX_Text(pFX, _T("[DrwNote]"), m_DrwNote);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmDrwTableSet ���

#ifdef _DEBUG
void CAsmDrwTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmDrwTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


