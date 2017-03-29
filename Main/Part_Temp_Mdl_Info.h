// Part_Temp_Mdl_Info.h : CPart_Temp_Mdl_Info 的声明

#pragma once

// 代码生成在 2005年6月24日, 15:14

class CPart_Temp_Mdl_Info : public CRecordset
{
public:
	CPart_Temp_Mdl_Info(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPart_Temp_Mdl_Info)

// 字段/参数数据

// 以下字符串类型（如果存在）反映数据库字段（ANSI 数据类型的 CStringA 和 Unicode
// 数据类型的 CStringW）的实际数据类型。
//  这是为防止 ODBC 驱动程序执行可能
// 不必要的转换。如果希望，可以将这些成员更改为
// CString 类型，ODBC 驱动程序将执行所有必要的转换。
// （注意：必须使用 3.5 版或更高版本的 ODBC 驱动程序
// 以同时支持 Unicode 和这些转换）。

	long	m_Number;
	long	m_FatherNum;
	CStringA	m_Name;
	CStringA	m_Type;
	CTime	m_Datatime;
	CStringA	m_Note;
	CStringA	m_FileName;
	CStringA	m_FileDirectory;
	CStringA	m_DrawFileName;
	CStringA	m_DrawFileDirectory;
	CStringA	m_MdlerName;
	CStringA	m_MdlerDepartment;
	long     m_MdlFatherNum;
	CStringA  m_MdlCode;
	long     m_MdlTypeNum;
	CStringA m_MdlTypeName;
	CStringA     m_Status;
	long		 m_MdlerId;
	CStringA     m_Material;

// 重写
	// 向导生成的虚函数重写
	public:
	virtual CString GetDefaultConnect();	// 默认连接字符串

	virtual CString GetDefaultSQL(); 	// 记录集的默认 SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX 支持

// 实现
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


