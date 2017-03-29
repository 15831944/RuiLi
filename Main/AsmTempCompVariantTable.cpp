// AsmTempCompVariantTable.h : CAsmTempCompVariantTable ���ʵ��



// CAsmTempCompVariantTable ʵ��

// ���������� 2005��7��27��, 20:56

#include "stdafx.h"
#include "AsmTempCompVariantTable.h"
//extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmTempCompVariantTable, CRecordset)

CAsmTempCompVariantTable::CAsmTempCompVariantTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_CompNum = 0;
	m_CompMdlNum = 0;
	m_CompInsNum = 0;
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmTempCompVariantTable::GetDefaultConnect()
{
	return _T("DSN=ruili;UID=cbs;PWD=cbs;APP=Microsoft\x00ae Visual Studio .NET;WSID=CHENBO;DATABASE=RuiLiMdlDb");

}

CString CAsmTempCompVariantTable::GetDefaultSQL()
{
	return _T("[dbo].[AsmTempCompVariantTable]");
}

void CAsmTempCompVariantTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[CompNum]"), m_CompNum);
	RFX_Long(pFX, _T("[CompMdlNum]"), m_CompMdlNum);
	RFX_Long(pFX, _T("[CompInsNum]"), m_CompInsNum);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmTempCompVariantTable ���

#ifdef _DEBUG
void CAsmTempCompVariantTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmTempCompVariantTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


