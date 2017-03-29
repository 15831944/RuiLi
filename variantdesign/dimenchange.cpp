// dimenchange.h : Cdimenchange 类的实现



// Cdimenchange 实现

// 代码生成在 2005年5月31日, 18:48

#include "stdafx.h"
#include "dimenchange.h"
IMPLEMENT_DYNAMIC(Cdimenchange, CRecordset)

Cdimenchange::Cdimenchange(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_dimension = L"";
	m_value =L"";
	m_nFields = 2;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString Cdimenchange::GetDefaultConnect()
{
	return _T("DSN=project;DBQ=E:\\project\\ruili.mdb;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString Cdimenchange::GetDefaultSQL()
{
	return _T("[dimenchange]");
}

void Cdimenchange::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Text(pFX, _T("[dimension]"), m_dimension);
	RFX_Text(pFX, _T("[value]"), m_value);

}
/////////////////////////////////////////////////////////////////////////////
// Cdimenchange 诊断

#ifdef _DEBUG
void Cdimenchange::AssertValid() const
{
	CRecordset::AssertValid();
}

void Cdimenchange::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


