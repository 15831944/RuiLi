// NProductInsPerfTable.h : CNProductInsPerfTable ���ʵ��



// CNProductInsPerfTable ʵ��

// ���������� 2005��9��17��, 12:04

#include "stdafx.h"
#include "NProductInsPerfTable.h"
IMPLEMENT_DYNAMIC(CNProductInsPerfTable, CRecordset)

CNProductInsPerfTable::CNProductInsPerfTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PdtNum = 0;
	m_Num = 0;
	m_MdlNum = 0;
	m_Value = "";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CNProductInsPerfTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNProductInsPerfTable::GetDefaultSQL()
{
	return _T("[dbo].[NProductInsPerfTable]");
}

void CNProductInsPerfTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PdtNum]"), m_PdtNum);
	RFX_Long(pFX, _T("[Num]"), m_Num);
	RFX_Long(pFX, _T("[MdlNum]"), m_MdlNum);
	RFX_Text(pFX, _T("[Value]"), m_Value);

}
/////////////////////////////////////////////////////////////////////////////
// CNProductInsPerfTable ���

#ifdef _DEBUG
void CNProductInsPerfTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNProductInsPerfTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


