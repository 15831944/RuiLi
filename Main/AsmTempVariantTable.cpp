// AsmTempVariantTable.h : CAsmTempVariantTable 类的实现



// CAsmTempVariantTable 实现

// 代码生成在 2005年7月27日, 20:58

#include "stdafx.h"
#include "AsmTempVariantTable.h"
//extern CString	dbLinkInfo;
IMPLEMENT_DYNAMIC(CAsmTempVariantTable, CRecordset)

CAsmTempVariantTable::CAsmTempVariantTable(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_AsmNum = 0;
	m_Number = 0;
	m_Name = "";
	m_PostVariant = 0;
	m_Type = "";
	m_IdNum = "";
	m_DimSymbol = "";
	m_ParaFeatName = "";
	m_SubType = "";
	m_Value = "";
	m_UpLimit = "";
	m_DwLimit = "";
	m_Note = "";
	m_nFields = 13;
	m_nDefaultType = dynaset;
}
//#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CAsmTempVariantTable::GetDefaultConnect()
{
	return _T(dbLinkInfo);
}

CString CAsmTempVariantTable::GetDefaultSQL()
{
	return _T("[dbo].[AsmTempVariantTable]");
}

void CAsmTempVariantTable::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Long(pFX, _T("[AsmNum]"), m_AsmNum);
	RFX_Long(pFX, _T("[Number]"), m_Number);
	RFX_Text(pFX, _T("[Name]"), m_Name);
	RFX_Long(pFX, _T("[PostVariant]"), m_PostVariant);
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
// CAsmTempVariantTable 诊断

#ifdef _DEBUG
void CAsmTempVariantTable::AssertValid() const
{
	CRecordset::AssertValid();
}

void CAsmTempVariantTable::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


