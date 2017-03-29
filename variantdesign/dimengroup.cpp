// dimengroup.h : Cdimengroup ���ʵ��



// Cdimengroup ʵ��

// ���������� 2005��6��1��, 9:25

#include "stdafx.h"
#include "dimengroup.h"
IMPLEMENT_DYNAMIC(Cdimengroup, CRecordset)

Cdimengroup::Cdimengroup(CDatabase* pdb)
	: CRecordset(pdb)
{
	column1 = 0;
	m_d35 = L"";
	m_d40 = L"";
	m_d43 = L"";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString Cdimengroup::GetDefaultConnect()
{
	return _T("DSN=project;DBQ=E:\\project\\ruili.mdb;DefaultDir=E:\\project;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString Cdimengroup::GetDefaultSQL()
{
	return _T("[dimengroup]");
}

void Cdimengroup::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[���]"), column1);
	RFX_Text(pFX, _T("[d35]"), m_d35);
	RFX_Text(pFX, _T("[d40]"), m_d40);
	RFX_Text(pFX, _T("[d43]"), m_d43);

}
/////////////////////////////////////////////////////////////////////////////
// Cdimengroup ���

#ifdef _DEBUG
void Cdimengroup::AssertValid() const
{
	CRecordset::AssertValid();
}

void Cdimengroup::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


