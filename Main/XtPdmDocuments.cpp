// XtPdmDocuments.h : CXtPdmDocuments ���ʵ��



// CXtPdmDocuments ʵ��

// ���������� 2005��12��20��, 14:07

#include "stdafx.h"
#include "XtPdmDocuments.h"
IMPLEMENT_DYNAMIC(CXtPdmDocuments, CRecordset)

CXtPdmDocuments::CXtPdmDocuments(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_ProductDwgCode = "";
	m_Code = "";
	m_DocumentType = "";
	m_Directory = "";
	m_FileName = "";
	m_FileType = "";
	m_FileBulk = "";
	m_FileDate = "";
	m_FileRemark = "";
	m_RevNumber = "";
	m_nFields = 10;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CXtPdmDocuments::GetDefaultConnect()
{
	return _T(dbPdmLinkInfo);
}

CString CXtPdmDocuments::GetDefaultSQL()
{
	return _T("[dbo].[Documents]");
}

void CXtPdmDocuments::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Text(pFX, _T("[ProductDwgCode]"), m_ProductDwgCode);
	RFX_Text(pFX, _T("[Code]"), m_Code);
	RFX_Text(pFX, _T("[DocumentType]"), m_DocumentType);
	RFX_Text(pFX, _T("[Directory]"), m_Directory);
	RFX_Text(pFX, _T("[FileName]"), m_FileName);
	RFX_Text(pFX, _T("[FileType]"), m_FileType);
	RFX_Text(pFX, _T("[FileBulk]"), m_FileBulk);
	RFX_Text(pFX, _T("[FileDate]"), m_FileDate);
	RFX_Text(pFX, _T("[FileRemark]"), m_FileRemark);
	RFX_Text(pFX, _T("[RevNumber]"), m_RevNumber);

}
/////////////////////////////////////////////////////////////////////////////
// CXtPdmDocuments ���

#ifdef _DEBUG
void CXtPdmDocuments::AssertValid() const
{
	CRecordset::AssertValid();
}

void CXtPdmDocuments::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


