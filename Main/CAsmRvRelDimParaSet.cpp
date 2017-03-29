// CAsmRvRelDimParaSet.h : CAsmRvRelDimParaSet ���ʵ��



// CAsmRvRelDimParaSet ʵ��

// ���������� 2005��7��8��, 13:02

#include "stdafx.h"
#include "CAsmRvRelDimParaSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmRvRelDimParaSet, CRecordset)

CAsmRvRelDimParaSet::CAsmRvRelDimParaSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_ProductMdlNum = 0;
	m_Number = 0;
	m_PostNum = 0;
	m_Type = "";
	m_IdNum = "";
	m_Name = "";
	m_DimSymbol = "";
	m_SubType = "";
	m_Value = "";
	m_nFields = 9;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmRvRelDimParaSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmRvRelDimParaSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmRelativeDimParaTable]");
}

void CAsmRvRelDimParaSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ProductMdlNum]"), m_ProductMdlNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Long(pFX, _T("[PostNum]"), m_PostNum);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Text(pFX, _T("[IdNum]"), m_IdNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[DimSymbol]"), m_DimSymbol);
	RFX_Text(pFX, _T("[SubType]"), m_SubType);
	RFX_Text(pFX, _T("[Value]"), m_Value);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmRvRelDimParaSet ���

#ifdef _DEBUG
void CAsmRvRelDimParaSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmRvRelDimParaSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


