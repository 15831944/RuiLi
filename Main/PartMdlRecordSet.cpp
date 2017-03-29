// PartMdlRecordSet.h : CPartMdlRecordSet ���ʵ��
//���Ŀ¼���ݿ��ļ�¼��


// CPartMdlRecordSet ʵ��

// ���������� 2005��5��31��, 9:39

#include "stdafx.h"
#include "PartMdlRecordSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CPartMdlRecordSet, CRecordset)

CPartMdlRecordSet::CPartMdlRecordSet(CDatabase* pdb)
	: CRecordset(pdb)
	
{
	m_Number=0;
	m_Name="";
	m_Code="";
	m_TypeNum=0;
    m_SolidFileName="";
	m_SolidFilePlace="";
    m_DrawingFileName="";
	m_DrawingFilePlace="";
	m_Datatime;
	m_Material="";
	m_Note="";
	m_Designer="";

	m_nFields = 12;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
//ԭʼֵ��DSN=ruili;UID=cbs;PWD=cbs;APP=Microsoft\x00ae Visual Studio .NET;WSID=CHENBO;DATABASE=RuiLiMdlDb;Network=DBMSSOCN"

CString CPartMdlRecordSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
	
}
CString CPartMdlRecordSet::GetDefaultSQL()
{
	return _T("[dbo].[NPartManageTable]");
}

void CPartMdlRecordSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Long(pFX, _T("[TypeNum]"), m_TypeNum);
	RFX_Date(pFX, _T("[Datatime]"), m_Datatime);
	RFX_Text(pFX, _T("[Code]"), m_Code);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Text(pFX, _T("[SolidFileName]"), m_SolidFileName);
	RFX_Text(pFX, _T("[SolidFilePlace]"), m_SolidFilePlace);
	RFX_Text(pFX, _T("[DrawingFileName]"), m_DrawingFileName);
	RFX_Text(pFX, _T("[DrawingFilePlace]"), m_DrawingFilePlace);
	RFX_Text(pFX, _T("[Designer]"), m_Designer);
	RFX_Text(pFX, _T("[Material]"), m_Material);



}
/////////////////////////////////////////////////////////////////////////////
// CPartMdlRecordSet ���

#ifdef _DEBUG
void CPartMdlRecordSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPartMdlRecordSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG




