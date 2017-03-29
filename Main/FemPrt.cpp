// FemPrt.h : CFemPrt 类的实现



// CFemPrt 实现

// 代码生成在 2006年1月11日, 23:09

#include "stdafx.h"
#include "FemPrt.h"
IMPLEMENT_DYNAMIC(CFemPrt, CRecordset)

CFemPrt::CFemPrt(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_Number = 0;
	m_FatherNum = 0;
	m_Name = "";
	m_Type = "";
	m_ConnPrtMdlNum = 0;
	m_FileDirectory = "";
	m_Note = "";
	m_DateTime;
	m_nFields = 8;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CFemPrt::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CFemPrt::GetDefaultSQL()
{
	return _T("[dbo].[FemPrt]");
}

void CFemPrt::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Long(pFX, _T("[FatherNum]"), m_FatherNum);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Long(pFX, _T("[ConnPrtMdlNum]"), m_ConnPrtMdlNum);
	RFX_Text(pFX, _T("[FileDirectory]"), m_FileDirectory);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Date(pFX, _T("[DateTime]"), m_DateTime);

}
/////////////////////////////////////////////////////////////////////////////
// CFemPrt 诊断

#ifdef _DEBUG
void CFemPrt::AssertValid() const
{
	CRecordset::AssertValid();
}

void CFemPrt::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


