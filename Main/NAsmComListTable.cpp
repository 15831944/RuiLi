// NAsmComListTable.h : CNAsmComListTable ���ʵ��



// CNAsmComListTable ʵ��

// ���������� 2005��9��15��, 16:59

#include "stdafx.h"
#include "NAsmComListTable.h"
IMPLEMENT_DYNAMIC(CNAsmComListTable, CRecordset)

CNAsmComListTable::CNAsmComListTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_PartNum = 0;
	m_Count = 0;
	m_nFields = 3;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CNAsmComListTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNAsmComListTable::GetDefaultSQL()
{
	return _T("[dbo].[NAsmComListTable]");
}

void CNAsmComListTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[PartNum]"), m_PartNum);
	RFX_Long(pFX, _T("[Count]"), m_Count);

}
/////////////////////////////////////////////////////////////////////////////
// CNAsmComListTable ���

#ifdef _DEBUG
void CNAsmComListTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNAsmComListTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


