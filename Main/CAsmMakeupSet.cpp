// CAsmMakeupSet.h : CAsmMakeupSet ���ʵ��



// CAsmMakeupSet ʵ��

// ���������� 2005��7��8��, 13:00

#include "stdafx.h"
#include "CAsmMakeupSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmMakeupSet, CRecordset)

CAsmMakeupSet::CAsmMakeupSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_PartNum = 0;
	m_PartMdlNum = 0;
	m_DefInsNum = 0;
	m_Type = "";
	m_UpMdlNum = 0;
	m_Name = "";
	m_FileName = "";
	m_Note = "";
	m_Code ="";
	m_nFields = 10;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmMakeupSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmMakeupSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmMakeupTable]");
}

void CAsmMakeupSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[PartNum]"), m_PartNum);
	RFX_Long(pFX, _T("[PartMdlNum]"), m_PartMdlNum);
	RFX_Long(pFX, _T("[DefInsNum]"), m_DefInsNum);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Long(pFX, _T("[UpMdlNum]"), m_UpMdlNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[FileName]"), m_FileName);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Text(pFX, _T("[Code]"), m_Code);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmMakeupSet ���

#ifdef _DEBUG
void CAsmMakeupSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmMakeupSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


