// CAsmVarSet.h : CAsmVarSet ���ʵ��



// CAsmVarSet ʵ��

// ���������� 2005��7��8��, 13:03

#include "stdafx.h"
#include "CAsmVarSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmVarSet, CRecordset)

CAsmVarSet::CAsmVarSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_Number = 0;
	m_Name = "";
	m_PostVariant = 0;
	m_Type = "";
	m_IdNum = "";
	m_DimSymbol = "";
	m_ParaFeatName = "";
	m_SubType = "";
	m_Value = "";
	m_UpLimit = "";
	m_DwLimit = "";
	m_Note = "";
	m_nFields = 13;
	m_nDefaultType = dynaset;
}
//error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CAsmVarSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmVarSet::GetDefaultSQL()
{
	return _T("[dbo].[NAsmVariantTable]");
}

void CAsmVarSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Long(pFX, _T("[PostVariant]"), m_PostVariant);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Text(pFX, _T("[IdNum]"), m_IdNum);
	RFX_Text(pFX, _T("[DimSymbol]"), m_DimSymbol);
	RFX_Text(pFX, _T("[ParaFeatName]"), m_ParaFeatName);
	RFX_Text(pFX, _T("[SubType]"), m_SubType);
	RFX_Text(pFX, _T("[Value]"), m_Value);
	RFX_Text(pFX, _T("[UpLimit]"), m_UpLimit);
	RFX_Text(pFX, _T("[DwLimit]"), m_DwLimit);
	RFX_Text(pFX, _T("[Note]"), m_Note);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmVarSet ���

#ifdef _DEBUG
void CAsmVarSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmVarSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


