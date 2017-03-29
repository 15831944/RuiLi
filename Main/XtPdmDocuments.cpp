// XtPdmDocuments.h : CXtPdmDocuments 类的实现



// CXtPdmDocuments 实现

// 代码生成在 2005年12月20日, 14:07

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
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
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
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
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
// CXtPdmDocuments 诊断

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


