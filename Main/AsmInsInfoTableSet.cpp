// AsmInsInfoTableSet.h : CAsmInsInfoTableSet 类的实现



// CAsmInsInfoTableSet 实现

// 代码生成在 2005年9月7日, 9:46

#include "stdafx.h"
#include "AsmInsInfoTableSet.h"
IMPLEMENT_DYNAMIC(CAsmInsInfoTableSet, CRecordset)

CAsmInsInfoTableSet::CAsmInsInfoTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_InsNum = 0;
	m_InsName = "";
	m_InsCode = "";
	m_SavePlace = "";
	m_InsCreateTime;
	m_InsCreaterName = "";
	m_InsCreateDpt = "";
	m_InsNote = "";
	m_nFields = 9;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CAsmInsInfoTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmInsInfoTableSet::GetDefaultSQL()
{
	return _T("[dbo].[AsmInsInfoTable]");
}

void CAsmInsInfoTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[InsNum]"), m_InsNum);
	RFX_Text(pFX, _T("[InsName]"), m_InsName);
	RFX_Text(pFX, _T("[InsCode]"), m_InsCode);
	RFX_Text(pFX, _T("[SavePlace]"), m_SavePlace);
	RFX_Date(pFX, _T("[InsCreateTime]"), m_InsCreateTime);
	RFX_Text(pFX, _T("[InsCreaterName]"), m_InsCreaterName);
	RFX_Text(pFX, _T("[InsCreateDpt]"), m_InsCreateDpt);
	RFX_Text(pFX, _T("[InsNote]"), m_InsNote);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmInsInfoTableSet 诊断

#ifdef _DEBUG
void CAsmInsInfoTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmInsInfoTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


