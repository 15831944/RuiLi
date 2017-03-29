// dimenchange.h : Cdimenchange ���ʵ��



// Cdimenchange ʵ��

// ���������� 2005��5��31��, 18:48

#include "stdafx.h"
#include "dimenchange.h"
IMPLEMENT_DYNAMIC(Cdimenchange, CRecordset)

Cdimenchange::Cdimenchange(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_dimension = L"";
	m_value =L"";
	m_nFields = 2;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString Cdimenchange::GetDefaultConnect()
{
	return _T("DSN=project;DBQ=E:\\project\\ruili.mdb;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString Cdimenchange::GetDefaultSQL()
{
	return _T("[dimenchange]");
}

void Cdimenchange::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[dimension]"), m_dimension);
	RFX_Text(pFX, _T("[value]"), m_value);

}
/////////////////////////////////////////////////////////////////////////////
// Cdimenchange ���

#ifdef _DEBUG
void Cdimenchange::AssertValid() const
{
	CRecordset::AssertValid();
}

void Cdimenchange::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


