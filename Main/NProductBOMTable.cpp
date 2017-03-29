// NProductBOMTable.h : CNProductBOMTable ���ʵ��



// CNProductBOMTable ʵ��

// ���������� 2005��9��22��, 12:42

#include "stdafx.h"
#include "NProductBOMTable.h"
IMPLEMENT_DYNAMIC(CNProductBOMTable, CRecordset)

CNProductBOMTable::CNProductBOMTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PdtNum = 0;
	m_Num = 0;
	m_UpNum = 0;
	m_MdlComNum=0;
	m_PrtNum = 0;
	m_Type = "";
	m_nFields = 6;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CNProductBOMTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNProductBOMTable::GetDefaultSQL()
{
	return _T("[dbo].[NProductBOMTable]");
}

void CNProductBOMTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PdtNum]"), m_PdtNum);
	RFX_Long(pFX, _T("[Num]"), m_Num);
	RFX_Long(pFX, _T("[MdlComNum]"), m_MdlComNum);
	RFX_Long(pFX, _T("[UpNum]"), m_UpNum);
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Text(pFX, _T("[Type]"), m_Type);

}
/////////////////////////////////////////////////////////////////////////////
// CNProductBOMTable ���

#ifdef _DEBUG
void CNProductBOMTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNProductBOMTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


