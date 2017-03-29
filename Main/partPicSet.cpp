// partPicSet.h : CpartPicSet ���ʵ��



// CpartPicSet ʵ��

// ���������� 2005��6��16��, 14:18

#include "stdafx.h"
#include "partPicSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CpartPicSet, CRecordset)

CpartPicSet::CpartPicSet(CDatabase* pdb)
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
CString CpartPicSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CpartPicSet::GetDefaultSQL()
{
	return _T("[dbo].[NPartPic]");
}

void CpartPicSet::DoFieldExchange(CFieldExchange* pFX)
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
// CpartPicSet ���

#ifdef _DEBUG
void CpartPicSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CpartPicSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


