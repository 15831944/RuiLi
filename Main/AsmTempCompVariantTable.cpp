// AsmTempCompVariantTable.h : CAsmTempCompVariantTable 类的实现



// CAsmTempCompVariantTable 实现

// 代码生成在 2005年7月27日, 20:56

#include "stdafx.h"
#include "AsmTempCompVariantTable.h"
//extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmTempCompVariantTable, CRecordset)

CAsmTempCompVariantTable::CAsmTempCompVariantTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_CompNum = 0;
	m_CompMdlNum = 0;
	m_CompInsNum = 0;
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CAsmTempCompVariantTable::GetDefaultConnect()
{
	return _T("DSN=ruili;UID=cbs;PWD=cbs;APP=Microsoft\x00ae Visual Studio .NET;WSID=CHENBO;DATABASE=RuiLiMdlDb");

}

CString CAsmTempCompVariantTable::GetDefaultSQL()
{
	return _T("[dbo].[AsmTempCompVariantTable]");
}

void CAsmTempCompVariantTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[CompNum]"), m_CompNum);
	RFX_Long(pFX, _T("[CompMdlNum]"), m_CompMdlNum);
	RFX_Long(pFX, _T("[CompInsNum]"), m_CompInsNum);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmTempCompVariantTable 诊断

#ifdef _DEBUG
void CAsmTempCompVariantTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmTempCompVariantTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


