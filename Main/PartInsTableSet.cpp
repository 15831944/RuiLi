// PartInsTableSet.h : CPartInsTableSet 类的实现



// CPartInsTableSet 实现

// 代码生成在 2005年6月20日, 10:43

#include "stdafx.h"
#include "PartInsTableSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CPartInsTableSet, CRecordset)

CPartInsTableSet::CPartInsTableSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_InsNum = 0;
	m_InsName = "";
	m_InsCode = "";
	m_InsNote = "";
	m_InsCreateTime;
	m_InsCreaterName = "";
	m_InsCreateDpt = "";
	m_nFields = 8;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CPartInsTableSet::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CPartInsTableSet::GetDefaultSQL()
{
	return _T("[dbo].[PartInsTable]");
}

void CPartInsTableSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[InsNum]"), m_InsNum);
	RFX_Text(pFX, _T("[InsName]"), m_InsName);
	RFX_Text(pFX, _T("[InsCode]"), m_InsCode);
	RFX_Text(pFX, _T("[InsNote]"), m_InsNote);
	RFX_Date(pFX, _T("[InsCreateTime]"), m_InsCreateTime);
	RFX_Text(pFX, _T("[InsCreaterName]"), m_InsCreaterName);
	RFX_Text(pFX, _T("[InsCreateDpt]"), m_InsCreateDpt);

}
/////////////////////////////////////////////////////////////////////////////
// CPartInsTableSet 诊断

#ifdef _DEBUG
void CPartInsTableSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPartInsTableSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


