// partDrawingSet.h : CpartDrawingSet ���ʵ��



// CpartDrawingSet ʵ��

// ���������� 2005��7��13��, 18:33

#include "stdafx.h"
#include "partDrawingSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CpartDrawingSet, CRecordset)

CpartDrawingSet::CpartDrawingSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_Number = 0;
	m_FileName = "";
	m_Note="";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CpartDrawingSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CpartDrawingSet::GetDefaultSQL()
{
	return _T("[dbo].[partDrawing]");
}

void CpartDrawingSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[FileName]"), m_FileName);
	RFX_Text(pFX, _T("[Note]"),m_Note);

}
/////////////////////////////////////////////////////////////////////////////
// CpartDrawingSet ���

#ifdef _DEBUG
void CpartDrawingSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CpartDrawingSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


