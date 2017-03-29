// NAsmType.h : CNAsmType ���ʵ��



// CNAsmType ʵ��

// ���������� 2005��9��17��, 12:07

#include "stdafx.h"
#include "NAsmType.h"
IMPLEMENT_DYNAMIC(CNAsmType, CRecordset)

CNAsmType::CNAsmType(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Number = 0;
	m_FatherNum = 0;
	m_Name = "";
	m_Code = "";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CNAsmType::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNAsmType::GetDefaultSQL()
{
	return _T("[dbo].[NAsmType]");
}

void CNAsmType::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Long(pFX, _T("[FatherNum]"), m_FatherNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Code]"), m_Code);

}
/////////////////////////////////////////////////////////////////////////////
// CNAsmType ���

#ifdef _DEBUG
void CNAsmType::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNAsmType::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


