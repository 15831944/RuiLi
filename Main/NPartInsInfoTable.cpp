// NPartInsInfoTable.h : CNPartInsInfoTable ���ʵ��



// CNPartInsInfoTable ʵ��

// ���������� 2005��9��15��, 13:22

#include "stdafx.h"
#include "NPartInsInfoTable.h"
IMPLEMENT_DYNAMIC(CNPartInsInfoTable, CRecordset)

CNPartInsInfoTable::CNPartInsInfoTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_Name = "";
	m_InsCode = "";
	m_TypeNum = 0;
	m_MdlNum = 0;
	m_Material = "";
	m_Unit = "";
	m_Designer = "";
	m_FinishTime;
	m_Auditor = "";
	m_AuditTime;
	m_SolidFileName = "";
	m_SolidPlace = "";
	m_DrawingName = "";
	m_DrawingPlace = "";
	m_FinishState = "";
	m_AuditState = "";
	m_PDMState = "";
	m_PDMNum = "";
	m_Note = "";
	m_LocalState = "";
	m_nFields = 21;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CNPartInsInfoTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNPartInsInfoTable::GetDefaultSQL()
{
	return _T("[dbo].[NPartInsInfoTable]");
}

void CNPartInsInfoTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[InsCode]"), m_InsCode);
	RFX_Long(pFX, _T("[TypeNum]"), m_TypeNum);
	RFX_Long(pFX, _T("[MdlNum]"), m_MdlNum);
	RFX_Text(pFX, _T("[Material]"), m_Material);
	RFX_Text(pFX, _T("[Unit]"), m_Unit);
	RFX_Text(pFX, _T("[Designer]"), m_Designer);
	RFX_Date(pFX, _T("[FinishTime]"), m_FinishTime);
	RFX_Text(pFX, _T("[Auditor]"), m_Auditor);
	RFX_Date(pFX, _T("[AuditTime]"), m_AuditTime);
	RFX_Text(pFX, _T("[SolidFileName]"), m_SolidFileName);
	RFX_Text(pFX, _T("[SolidPlace]"), m_SolidPlace);
	RFX_Text(pFX, _T("[DrawingName]"), m_DrawingName);
	RFX_Text(pFX, _T("[DrawingPlace]"), m_DrawingPlace);
	RFX_Text(pFX, _T("[FinishState]"), m_FinishState);
	RFX_Text(pFX, _T("[AuditState]"), m_AuditState);
	RFX_Text(pFX, _T("[PDMState]"), m_PDMState);
	RFX_Text(pFX, _T("[PDMNum]"), m_PDMNum);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Text(pFX, _T("[LocalState]"), m_LocalState);

}
/////////////////////////////////////////////////////////////////////////////
// CNPartInsInfoTable ���

#ifdef _DEBUG
void CNPartInsInfoTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNPartInsInfoTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


