// jisuanSet.h: CjisuanSet 类的接口
//


#pragma once

// 代码生成在 2005年8月16日, 10:27

class CjisuanSet : public CRecordset
{
public:
	CjisuanSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CjisuanSet)

// 字段/参数数据

// 以下字符串类型（如果存在）反映数据库字段（ANSI 数据类型的 CStringA 和 Unicode
// 数据类型的 CStringW）的实际数据类型。
//  这是为防止 ODBC 驱动程序执行可能
// 不必要的转换。如果希望，可以将这些成员更改为
// CString 类型，ODBC 驱动程序将执行所有必要的转换。
// （注意：必须使用 3.5 版或更高版本的 ODBC 驱动程序
// 以同时支持 Unicode 和这些转换）。

	double	m_fptd1;
	double	m_fptd2;
	double	m_fptm1;
	double	m_fptm2;
	double	m_fpti0;
	double	m_fpti1;
	double	m_fpti2;
	double	m_fpti3;
	double	m_fpti4;
	double	m_fpti5;
	double	m_fpti6;
	double	m_fpti7;
	double	m_fpti8;
	double	m_fpti9;
	double	m_fpti10;
	double	m_fpti11;
	double	m_fpti12;
	double	m_Frd1;
	double	m_Frd2;
	double	m_Frm1;
	double	m_Frm2;
	double	m_Fri0;
	double	m_Fri1;
	double	m_Fri2;
	double	m_Fri3;
	double	m_Fri4;
	double	m_Fri5;
	double	m_Fri6;
	double	m_Fri7;
	double	m_Fri8;
	double	m_Fri9;
	double	m_Fri10;
	double	m_Fri11;
	double	m_Fri12;
	double	m_fhad1;
	double	m_fhad2;
	double	m_fham1;
	double	m_fham2;
	double	m_fhai0;
	double	m_fhai1;
	double	m_fhai2;
	double	m_fhai3;
	double	m_fhai4;
	double	m_fhai5;
	double	m_fhai6;
	double	m_fhai7;
	double	m_fhai8;
	double	m_fhai9;
	double	m_fhai10;
	double	m_fhai11;
	double	m_fhai12;
	double	m_ffad1;
	double	m_ffad2;
	double	m_ffam1;
	double	m_ffam2;
	double	m_ffai0;
	double	m_ffai1;
	double	m_ffai2;
	double	m_ffai3;
	double	m_ffai4;
	double	m_ffai5;
	double	m_ffai6;
	double	m_ffai7;
	double	m_ffai8;
	double	m_ffai9;
	double	m_ffai10;
	double	m_ffai11;
	double	m_ffai12;

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

