// Part_Temp_Pic.h : CPart_Temp_Pic ���ʵ��



// CPart_Temp_Pic ʵ��

// ���������� 2005��6��25��, 9:38

#include "stdafx.h"
#include "Part_Temp_Pic.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CPart_Temp_Pic, CRecordset)

CPart_Temp_Pic::CPart_Temp_Pic(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_Number = 0;
	m_Name = "";
	m_Note = "";
	m_IsMain = 0;
	m_File;
	m_nFields = 6;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CPart_Temp_Pic::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CPart_Temp_Pic::GetDefaultSQL()
{
	return _T("[dbo].[Part_Temp_Pic]");
}

void CPart_Temp_Pic::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Long(pFX, _T("[IsMain]"), m_IsMain);
	RFX_LongBinary(pFX,_T("[File]"),m_File);

}
/////////////////////////////////////////////////////////////////////////////
// CPart_Temp_Pic ���

#ifdef _DEBUG
void CPart_Temp_Pic::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPart_Temp_Pic::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


