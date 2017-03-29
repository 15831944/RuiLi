// AsmTempDrwTable.h : CAsmTempDrwTable ���ʵ��



// CAsmTempDrwTable ʵ��

// ���������� 2005��7��27��, 20:56

#include "stdafx.h"
#include "AsmTempDrwTable.h"
//extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmTempDrwTable, CRecordset)

CAsmTempDrwTable::CAsmTempDrwTable(CDatabase* pdb)
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
CString CAsmTempDrwTable::GetDefaultConnect()
{
	return _T("DSN=ruili;UID=cbs;PWD=cbs;APP=Microsoft\x00ae Visual Studio .NET;WSID=CHENBO;DATABASE=RuiLiMdlDb");

}

CString CAsmTempDrwTable::GetDefaultSQL()
{
	return _T("[dbo].[AsmTempDrwTable]");
}

void CAsmTempDrwTable::DoFieldExchange(CFieldExchange* pFX)
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
// CAsmTempDrwTable ���

#ifdef _DEBUG
void CAsmTempDrwTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmTempDrwTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


