// NAsmComListTable.h : CNAsmComListTable 类的实现



// CNAsmComListTable 实现

// 代码生成在 2005年9月15日, 16:59

#include "stdafx.h"
#include "NAsmComListTable.h"
IMPLEMENT_DYNAMIC(CNAsmComListTable, CRecordset)

CNAsmComListTable::CNAsmComListTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_PartNum = 0;
	m_Count = 0;
	m_nFields = 3;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CNAsmComListTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNAsmComListTable::GetDefaultSQL()
{
	return _T("[dbo].[NAsmComListTable]");
}

void CNAsmComListTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[PartNum]"), m_PartNum);
	RFX_Long(pFX, _T("[Count]"), m_Count);

}
/////////////////////////////////////////////////////////////////////////////
// CNAsmComListTable 诊断

#ifdef _DEBUG
void CNAsmComListTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNAsmComListTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


