// NProductBOMTable.h : CNProductBOMTable 类的实现



// CNProductBOMTable 实现

// 代码生成在 2005年9月22日, 12:42

#include "stdafx.h"
#include "NProductBOMTable.h"
IMPLEMENT_DYNAMIC(CNProductBOMTable, CRecordset)

CNProductBOMTable::CNProductBOMTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PdtNum = 0;
	m_Num = 0;
	m_UpNum = 0;
	m_MdlComNum=0;
	m_PrtNum = 0;
	m_Type = "";
	m_nFields = 6;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CNProductBOMTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CNProductBOMTable::GetDefaultSQL()
{
	return _T("[dbo].[NProductBOMTable]");
}

void CNProductBOMTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[PdtNum]"), m_PdtNum);
	RFX_Long(pFX, _T("[Num]"), m_Num);
	RFX_Long(pFX, _T("[MdlComNum]"), m_MdlComNum);
	RFX_Long(pFX, _T("[UpNum]"), m_UpNum);
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Text(pFX, _T("[Type]"), m_Type);

}
/////////////////////////////////////////////////////////////////////////////
// CNProductBOMTable 诊断

#ifdef _DEBUG
void CNProductBOMTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CNProductBOMTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


