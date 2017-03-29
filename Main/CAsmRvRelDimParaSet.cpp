// CAsmRvRelDimParaSet.h : CAsmRvRelDimParaSet 类的实现



// CAsmRvRelDimParaSet 实现

// 代码生成在 2005年7月8日, 13:02

#include "stdafx.h"
#include "CAsmRvRelDimParaSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmRvRelDimParaSet, CRecordset)

CAsmRvRelDimParaSet::CAsmRvRelDimParaSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_ProductMdlNum = 0;
	m_Number = 0;
	m_PostNum = 0;
	m_Type = "";
	m_IdNum = "";
	m_Name = "";
	m_DimSymbol = "";
	m_SubType = "";
	m_Value = "";
	m_nFields = 9;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CAsmRvRelDimParaSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmRvRelDimParaSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmRelativeDimParaTable]");
}

void CAsmRvRelDimParaSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[ProductMdlNum]"), m_ProductMdlNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Long(pFX, _T("[PostNum]"), m_PostNum);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Text(pFX, _T("[IdNum]"), m_IdNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[DimSymbol]"), m_DimSymbol);
	RFX_Text(pFX, _T("[SubType]"), m_SubType);
	RFX_Text(pFX, _T("[Value]"), m_Value);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmRvRelDimParaSet 诊断

#ifdef _DEBUG
void CAsmRvRelDimParaSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmRvRelDimParaSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


