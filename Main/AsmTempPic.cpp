// AsmTempPic.h : CAsmTempPic ���ʵ��



// CAsmTempPic ʵ��

// ���������� 2005��7��27��, 20:57

#include "stdafx.h"
#include "AsmTempPic.h"
//extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmTempPic, CRecordset)

CAsmTempPic::CAsmTempPic(CDatabase* pdb)
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
CString CAsmTempPic::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmTempPic::GetDefaultSQL()
{
	return _T("[dbo].[AsmTempPic]");
}

void CAsmTempPic::DoFieldExchange(CFieldExchange* pFX)
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
	RFX_LongBinary(pFX ,_T("[File]"),m_File);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmTempPic ���

#ifdef _DEBUG
void CAsmTempPic::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmTempPic::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


