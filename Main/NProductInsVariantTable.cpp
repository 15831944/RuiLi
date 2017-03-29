// NProductInsVariantTable.h : CNProductInsVariantTable ���ʵ��



// CNProductInsVariantTable ʵ��

// ���������� 2005��9��17��, 12:04

#include "stdafx.h"
#include "NProductInsVariantTable.h"
IMPLEMENT_DYNAMIC(CNProductInsVariantTable, CRecordset)

CNProductInsVariantTable::CNProductInsVariantTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PdtNum = "";
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
CString CNProductInsVariantTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNProductInsVariantTable::GetDefaultSQL()
{
	return _T("[dbo].[NProductInsVariantTable]");
}

void CNProductInsVariantTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[PdtNum]"), m_PdtNum);
	RFX_Long(pFX, _T("[Num]"), m_Num);
	RFX_Long(pFX, _T("[MdlNum]"), m_MdlNum);
	RFX_Text(pFX, _T("[Value]"), m_Value);

}
/////////////////////////////////////////////////////////////////////////////
// CNProductInsVariantTable ���

#ifdef _DEBUG
void CNProductInsVariantTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNProductInsVariantTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


