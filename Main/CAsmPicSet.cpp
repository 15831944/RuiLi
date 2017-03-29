// CAsmPicSet.h : CAsmPicSet ���ʵ��



// CAsmPicSet ʵ��

// ���������� 2005��7��11��, 7:49

#include "stdafx.h"
#include "CAsmPicSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmPicSet, CRecordset)

CAsmPicSet::CAsmPicSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
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
CString CAsmPicSet::GetDefaultConnect()
{
	return _T("dbLinkInfo");
}

CString CAsmPicSet::GetDefaultSQL()
{
	return _T("[dbo].[NAsmPic]");
}

void CAsmPicSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Long(pFX, _T("[IsMain]"), m_IsMain);
	RFX_LongBinary(pFX,_T("[File]"),m_File);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmPicSet ���

#ifdef _DEBUG
void CAsmPicSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmPicSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


