// Part_Temp_Para_Info.h : CPart_Temp_Para_Info 类的实现



// CPart_Temp_Para_Info 实现

// 代码生成在 2005年6月25日, 9:38

#include "stdafx.h"
#include "Part_Temp_Para_Info.h"
//extern CString dbLinkInfo;
IMPLEMENT_DYNAMIC(CPart_Temp_Para_Info, CRecordset)

CPart_Temp_Para_Info::CPart_Temp_Para_Info(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_PrtNum = 0;
	m_Number = 0;
	m_Name = "";
	m_Type = "";
	m_IdNum = "";
	m_DimSymbol = "";
	m_ParaFeatName = "";
	m_SubType = "";
	m_Value = "";
	m_UpLimit = "";
	m_DwLimit = "";
	m_Note = "";
	m_nFields = 12;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CPart_Temp_Para_Info::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CPart_Temp_Para_Info::GetDefaultSQL()
{
	return _T("[dbo].[Part_Temp_Para_Info]");
}

void CPart_Temp_Para_Info::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[PrtNum]"), m_PrtNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Text(pFX, _T("[Type]"), m_Type);
	RFX_Text(pFX, _T("[IdNum]"), m_IdNum);
	RFX_Text(pFX, _T("[DimSymbol]"), m_DimSymbol);
	RFX_Text(pFX, _T("[ParaFeatName]"), m_ParaFeatName);
	RFX_Text(pFX, _T("[SubType]"), m_SubType);
	RFX_Text(pFX, _T("[Value]"), m_Value);
	RFX_Text(pFX, _T("[UpLimit]"), m_UpLimit);
	RFX_Text(pFX, _T("[DwLimit]"), m_DwLimit);
	RFX_Text(pFX, _T("[Note]"), m_Note);

}
/////////////////////////////////////////////////////////////////////////////
// CPart_Temp_Para_Info 诊断

#ifdef _DEBUG
void CPart_Temp_Para_Info::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPart_Temp_Para_Info::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


