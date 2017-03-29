// NAsmBomTable.h : CNAsmBomTable ���ʵ��



// CNAsmBomTable ʵ��

// ���������� 2005��9��15��, 16:58

#include "stdafx.h"
#include "NAsmBomTable.h"
IMPLEMENT_DYNAMIC(CNAsmBomTable, CRecordset)

CNAsmBomTable::CNAsmBomTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_ComNum = 0;
	m_UpComNum = 0;
	m_Name = "";
	m_PartNum = 0;
	m_PartMdlNum = 0;
	m_Type = "";
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
CString CNAsmBomTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNAsmBomTable::GetDefaultSQL()
{
	return _T("[dbo].[NAsmBomTable]");
}

void CNAsmBomTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[ComNum]"), m_ComNum);
	RFX_Long(pFX, _T("[UpComNum]"), m_UpComNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Long(pFX, _T("[PartNum]"), m_PartNum);
	RFX_Long(pFX, _T("[PartMdlNum]"), m_PartMdlNum);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Text(pFX, _T("[FileName]"), m_FileName);
	RFX_Text(pFX, _T("[Note]"), m_Note);

}
/////////////////////////////////////////////////////////////////////////////
// CNAsmBomTable ���

#ifdef _DEBUG
void CNAsmBomTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNAsmBomTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


