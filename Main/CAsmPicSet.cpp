// CAsmPicSet.h : CAsmPicSet 类的实现



// CAsmPicSet 实现

// 代码生成在 2005年7月11日, 7:49

#include "stdafx.h"
#include "CAsmPicSet.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmPicSet, CRecordset)

CAsmPicSet::CAsmPicSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_Number = 0;
	m_Name = "";
	m_Note = "";
	m_IsMain = 0;
	m_File;
	m_nFields = 6;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CAsmPicSet::GetDefaultConnect()
{
	return _T("dbLinkInfo");
}

CString CAsmPicSet::GetDefaultSQL()
{
	return _T("[dbo].[NAsmPic]");
}

void CAsmPicSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Note]"), m_Note);
	RFX_Long(pFX, _T("[IsMain]"), m_IsMain);
	RFX_LongBinary(pFX,_T("[File]"),m_File);

}
/////////////////////////////////////////////////////////////////////////////
// CAsmPicSet 诊断

#ifdef _DEBUG
void CAsmPicSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmPicSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


