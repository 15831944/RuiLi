// AsmTempMakeupTable.h : CAsmTempMakeupTable ���ʵ��



// CAsmTempMakeupTable ʵ��

// ���������� 2005��7��27��, 20:57

#include "stdafx.h"
#include "AsmTempMakeupTable.h"
//extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmTempMakeupTable, CRecordset)

CAsmTempMakeupTable::CAsmTempMakeupTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_ComNum = 0;
	m_PartMdlNum = 0;
	m_PartNum = 0;
	m_Type = "";
	m_UpComNum = 0;
	m_Name = "";
	m_FileName = "";
	m_Note = "";
	m_nFields = 9;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmTempMakeupTable::GetDefaultConnect()
{
	return _T("DSN=ruili;UID=cbs;PWD=cbs;APP=Microsoft\x00ae Visual Studio .NET;WSID=CHENBO;DATABASE=RuiLiMdlDb");

}

CString CAsmTempMakeupTable::GetDefaultSQL()
{
	return _T("[dbo].[AsmTempMakeupTable]");
}

void CAsmTempMakeupTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[ComNum]"), m_ComNum);
	RFX_Long(pFX, _T("[PartMdlNum]"), m_PartMdlNum);
	RFX_Long(pFX, _T("[PartNum]"), m_PartNum);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Long(pFX, _T("[UpComNum]"), m_UpComNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[FileName]"), m_FileName);
	RFX_Text(pFX, _T("[Note]"), m_Note);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmTempMakeupTable ���

#ifdef _DEBUG
void CAsmTempMakeupTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmTempMakeupTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


