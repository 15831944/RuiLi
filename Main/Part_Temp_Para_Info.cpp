// Part_Temp_Para_Info.h : CPart_Temp_Para_Info ���ʵ��



// CPart_Temp_Para_Info ʵ��

// ���������� 2005��6��25��, 9:38

#include "stdafx.h"
#include "Part_Temp_Para_Info.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CPart_Temp_Para_Info, CRecordset)

CPart_Temp_Para_Info::CPart_Temp_Para_Info(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_Number = 0;
	m_Name = "";
	m_Type = "";
	m_IdNum = "";
	m_DimSymbol = "";
	m_ParaFeatName = "";
	m_SubType = "";
	m_Value = "";
	m_UpLimit = "";
	m_DwLimit = "";
	m_Note = "";
	m_nFields = 12;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CPart_Temp_Para_Info::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CPart_Temp_Para_Info::GetDefaultSQL()
{
	return _T("[dbo].[Part_Temp_Para_Info]");
}

void CPart_Temp_Para_Info::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
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
// CPart_Temp_Para_Info ���

#ifdef _DEBUG
void CPart_Temp_Para_Info::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPart_Temp_Para_Info::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


