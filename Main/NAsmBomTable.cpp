// NAsmBomTable.h : CNAsmBomTable 类的实现



// CNAsmBomTable 实现

// 代码生成在 2005年9月15日, 16:58

#include "stdafx.h"
#include "NAsmBomTable.h"
IMPLEMENT_DYNAMIC(CNAsmBomTable, CRecordset)

CNAsmBomTable::CNAsmBomTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_ComNum = 0;
	m_UpComNum = 0;
	m_Name = "";
	m_PartNum = 0;
	m_PartMdlNum = 0;
	m_Type = "";
	m_FileName = "";
	m_Note = "";
	m_nFields = 9;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CNAsmBomTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNAsmBomTable::GetDefaultSQL()
{
	return _T("[dbo].[NAsmBomTable]");
}

void CNAsmBomTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[ComNum]"), m_ComNum);
	RFX_Long(pFX, _T("[UpComNum]"), m_UpComNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Long(pFX, _T("[PartNum]"), m_PartNum);
	RFX_Long(pFX, _T("[PartMdlNum]"), m_PartMdlNum);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Text(pFX, _T("[FileName]"), m_FileName);
	RFX_Text(pFX, _T("[Note]"), m_Note);

}
/////////////////////////////////////////////////////////////////////////////
// CNAsmBomTable 诊断

#ifdef _DEBUG
void CNAsmBomTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNAsmBomTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


