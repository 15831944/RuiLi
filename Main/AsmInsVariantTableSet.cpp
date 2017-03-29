// AsmInsVariantTableSet.h : CAsmInsVariantTableSet 类的实现



// CAsmInsVariantTableSet 实现

// 代码生成在 2005年9月7日, 9:47

#include "stdafx.h"
#include "AsmInsVariantTableSet.h"
IMPLEMENT_DYNAMIC(CAsmInsVariantTableSet, CRecordset)

CAsmInsVariantTableSet::CAsmInsVariantTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_InsNum = 0;
	m_Num = 0;
	m_Value = "";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CAsmInsVariantTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmInsVariantTableSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmInsVariantTable]");
}

void CAsmInsVariantTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[InsNum]"), m_InsNum);
	RFX_Long(pFX, _T("[Num]"), m_Num);
	RFX_Text(pFX, _T("[Value]"), m_Value);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmInsVariantTableSet 诊断

#ifdef _DEBUG
void CAsmInsVariantTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmInsVariantTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


