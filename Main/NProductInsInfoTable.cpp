// NProductInsInfoTable.h : CNProductInsInfoTable ���ʵ��



// CNProductInsInfoTable ʵ��

// ���������� 2005��9��17��, 12:02

#include "stdafx.h"
#include "NProductInsInfoTable.h"
IMPLEMENT_DYNAMIC(CNProductInsInfoTable, CRecordset)

CNProductInsInfoTable::CNProductInsInfoTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PdtNum = 0;
	m_InsCode = "";
	//m_GrpFrm = "";
	m_Name = "";
	m_Unit = "";
	m_MdlNum = 0;
	m_Designer = "";
	m_FinishTime;
	m_Auditor = "";
	m_AuditTime = "";
	m_SolidFileName="";
	m_SolidFilePlace="";
	m_DrawingFileName="";
	m_DrawingFilePlace="";
	m_LocalState="";
	m_FinishState = "";
	m_AuditState = "";
	m_PDMState = "";
	m_PDMNum = "";
	m_Note = "";
	m_TypeNum=0;
	m_nFields = 20;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CNProductInsInfoTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNProductInsInfoTable::GetDefaultSQL()
{
	return _T("[dbo].[NProductInsInfoTable]");
}

void CNProductInsInfoTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PdtNum]"), m_PdtNum);
	RFX_Text(pFX, _T("[InsCode]"), m_InsCode);
	//RFX_Text(pFX, _T("[GrpFrm]"), m_GrpFrm);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Unit]"), m_Unit);
	RFX_Long(pFX, _T("[MdlNum]"), m_MdlNum);
	RFX_Long(pFX, _T("[TypeNum]"), m_TypeNum);
	RFX_Text(pFX, _T("[Designer]"), m_Designer);
	RFX_Date(pFX, _T("[FinishTime]"), m_FinishTime);
	RFX_Text(pFX, _T("[Auditor]"), m_Auditor);
	RFX_Text(pFX, _T("[AuditTime]"), m_AuditTime);
	RFX_Text(pFX, _T("[SolidFileName]"), m_SolidFileName);
	RFX_Text(pFX, _T("[SolidPlace]"), m_SolidFilePlace);
	RFX_Text(pFX, _T("[DrawingName]"), m_DrawingFileName);
	RFX_Text(pFX, _T("[DrawingPlace]"), m_DrawingFilePlace);
	RFX_Text(pFX, _T("[FinishState]"), m_FinishState);
	RFX_Text(pFX, _T("[AuditState]"), m_AuditState);
	RFX_Text(pFX, _T("[PDMState]"), m_PDMState);
	RFX_Text(pFX, _T("[PDMNum]"), m_PDMNum);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Text(pFX, _T("[LocalState]"), m_LocalState);

}
/////////////////////////////////////////////////////////////////////////////
// CNProductInsInfoTable ���

#ifdef _DEBUG
void CNProductInsInfoTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNProductInsInfoTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


