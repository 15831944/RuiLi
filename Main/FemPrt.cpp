// FemPrt.h : CFemPrt ���ʵ��



// CFemPrt ʵ��

// ���������� 2006��1��11��, 23:09

#include "stdafx.h"
#include "FemPrt.h"
IMPLEMENT_DYNAMIC(CFemPrt, CRecordset)

CFemPrt::CFemPrt(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Number = 0;
	m_FatherNum = 0;
	m_Name = "";
	m_Type = "";
	m_ConnPrtMdlNum = 0;
	m_FileDirectory = "";
	m_Note = "";
	m_DateTime;
	m_nFields = 8;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CFemPrt::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CFemPrt::GetDefaultSQL()
{
	return _T("[dbo].[FemPrt]");
}

void CFemPrt::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Long(pFX, _T("[FatherNum]"), m_FatherNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Long(pFX, _T("[ConnPrtMdlNum]"), m_ConnPrtMdlNum);
	RFX_Text(pFX, _T("[FileDirectory]"), m_FileDirectory);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Date(pFX, _T("[DateTime]"), m_DateTime);

}
/////////////////////////////////////////////////////////////////////////////
// CFemPrt ���

#ifdef _DEBUG
void CFemPrt::AssertValid() const
{
	CRecordset::AssertValid();
}

void CFemPrt::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


