// ffa.h : Cffa 类的实现



// Cffa 实现

// 代码生成在 2005年8月17日, 10:31

#include "stdafx.h"
#include "ffa.h"
IMPLEMENT_DYNAMIC(Cffa, CRecordset)

Cffa::Cffa(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_index = 0;
	m_d1 = 0.0;
	m_d2 = 0.0;
	m_m1 = 0.0;
	m_m2 = 0.0;
	m_i0 = 0.0;
	m_i1 = 0.0;
	m_i2 = 0.0;
	m_i3 = 0.0;
	m_i4 = 0.0;
	m_i5 = 0.0;
	m_i6 = 0.0;
	m_i7 = 0.0;
	m_i8 = 0.0;
	m_i9 = 0.0;
	m_i10 = 0.0;
	m_i11 = 0.0;
	m_i12 = 0.0;
	m_nFields = 18;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString Cffa::GetDefaultConnect()
{
	return _T("DSN=jisuan;DBQ=E:\\数据库\\jisuan.mdb;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString Cffa::GetDefaultSQL()
{
	return _T("[齿形公差ffa]");
}

void Cffa::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[index]"), m_index);
	RFX_Double(pFX, _T("[d1]"), m_d1);
	RFX_Double(pFX, _T("[d2]"), m_d2);
	RFX_Double(pFX, _T("[m1]"), m_m1);
	RFX_Double(pFX, _T("[m2]"), m_m2);
	RFX_Double(pFX, _T("[i0]"), m_i0);
	RFX_Double(pFX, _T("[i1]"), m_i1);
	RFX_Double(pFX, _T("[i2]"), m_i2);
	RFX_Double(pFX, _T("[i3]"), m_i3);
	RFX_Double(pFX, _T("[i4]"), m_i4);
	RFX_Double(pFX, _T("[i5]"), m_i5);
	RFX_Double(pFX, _T("[i6]"), m_i6);
	RFX_Double(pFX, _T("[i7]"), m_i7);
	RFX_Double(pFX, _T("[i8]"), m_i8);
	RFX_Double(pFX, _T("[i9]"), m_i9);
	RFX_Double(pFX, _T("[i10]"), m_i10);
	RFX_Double(pFX, _T("[i11]"), m_i11);
	RFX_Double(pFX, _T("[i12]"), m_i12);

}
/////////////////////////////////////////////////////////////////////////////
// Cffa 诊断

#ifdef _DEBUG
void Cffa::AssertValid() const
{
	CRecordset::AssertValid();
}

void Cffa::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


