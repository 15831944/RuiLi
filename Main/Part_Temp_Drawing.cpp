// Part_Temp_Drawing.h : CPart_Temp_Drawing ���ʵ��



// CPart_Temp_Drawing ʵ��

// ���������� 2005��7��28��, 14:59

#include "stdafx.h"
#include "Part_Temp_Drawing.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CPart_Temp_Drawing, CRecordset)

CPart_Temp_Drawing::CPart_Temp_Drawing(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_Number = 0;
	m_FileName = "";
	m_Note = "";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CPart_Temp_Drawing::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CPart_Temp_Drawing::GetDefaultSQL()
{
	return _T("[dbo].[Part_Temp_Drawing]");
}

void CPart_Temp_Drawing::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[FileName]"), m_FileName);
	RFX_Text(pFX, _T("[Note]"), m_Note);

}
/////////////////////////////////////////////////////////////////////////////
// CPart_Temp_Drawing ���

#ifdef _DEBUG
void CPart_Temp_Drawing::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPart_Temp_Drawing::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


