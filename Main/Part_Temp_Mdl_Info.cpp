// Part_Temp_Mdl_Info.h : CPart_Temp_Mdl_Info ���ʵ��



// CPart_Temp_Mdl_Info ʵ��

// ���������� 2005��6��24��, 15:14

#include "stdafx.h"
#include "Part_Temp_Mdl_Info.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CPart_Temp_Mdl_Info, CRecordset)

CPart_Temp_Mdl_Info::CPart_Temp_Mdl_Info(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Number = 0;
	m_FatherNum = 0;
	m_Name = "";
	m_Type = "";
	m_Datatime;
	m_Note = "";
	m_FileName = "";
	m_FileDirectory = "";
    m_DrawFileName="";
	m_DrawFileDirectory="";
	m_MdlerName = "";
	m_MdlerDepartment = "";
	m_MdlFatherNum=-1;
	m_MdlCode="";
	m_MdlTypeNum=0;
	m_MdlTypeName="";
	m_Status="";
	m_Material="";
	m_MdlerId=0;
	
	m_nFields = 19;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CPart_Temp_Mdl_Info::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CPart_Temp_Mdl_Info::GetDefaultSQL()
{
	return _T("[dbo].[Part_Temp_Mdl_Info]");
}

void CPart_Temp_Mdl_Info::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Long(pFX, _T("[FatherNum]"), m_FatherNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Date(pFX, _T("[Datatime]"), m_Datatime);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Text(pFX, _T("[FileName]"), m_FileName);
	RFX_Text(pFX, _T("[FileDirectory]"), m_FileDirectory);
	RFX_Text(pFX, _T("[DrawFileName]"), m_DrawFileName);
	RFX_Text(pFX, _T("[DrawFileDirectory]"), m_DrawFileDirectory);
	RFX_Text(pFX, _T("[MdlerName]"), m_MdlerName);
	RFX_Text(pFX, _T("[MdlerDepartment]"), m_MdlerDepartment);
    RFX_Long(pFX, _T("[MdlFatherNum]"), m_MdlFatherNum);
	RFX_Text(pFX, _T("[MdlCode]"), m_MdlCode);
	RFX_Long(pFX, _T("[MdlTypeNum]"), m_MdlTypeNum);
	RFX_Text(pFX, _T("[MdlTypeName]"), m_MdlTypeName);
	RFX_Text(pFX, _T("[Status]"), m_Status);
	RFX_Long(pFX, _T("[MdlerId]"), m_MdlerId);
	RFX_Text(pFX, _T("[Material]"), m_Material);

}
/////////////////////////////////////////////////////////////////////////////
// CPart_Temp_Mdl_Info ���

#ifdef _DEBUG
void CPart_Temp_Mdl_Info::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPart_Temp_Mdl_Info::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


