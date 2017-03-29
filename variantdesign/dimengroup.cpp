// dimengroup.h : Cdimengroup 类的实现



// Cdimengroup 实现

// 代码生成在 2005年6月1日, 9:25

#include "stdafx.h"
#include "dimengroup.h"
IMPLEMENT_DYNAMIC(Cdimengroup, CRecordset)

Cdimengroup::Cdimengroup(CDatabase* pdb)
	: CRecordset(pdb)
{
	column1 = 0;
	m_d35 = L"";
	m_d40 = L"";
	m_d43 = L"";
	m_nFields = 4;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString Cdimengroup::GetDefaultConnect()
{
	return _T("DSN=project;DBQ=E:\\project\\ruili.mdb;DefaultDir=E:\\project;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString Cdimengroup::GetDefaultSQL()
{
	return _T("[dimengroup]");
}

void Cdimengroup::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[编号]"), column1);
	RFX_Text(pFX, _T("[d35]"), m_d35);
	RFX_Text(pFX, _T("[d40]"), m_d40);
	RFX_Text(pFX, _T("[d43]"), m_d43);

}
/////////////////////////////////////////////////////////////////////////////
// Cdimengroup 诊断

#ifdef _DEBUG
void Cdimengroup::AssertValid() const
{
	CRecordset::AssertValid();
}

void Cdimengroup::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


