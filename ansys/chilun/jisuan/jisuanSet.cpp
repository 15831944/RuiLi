// jisuanSet.cpp : CjisuanSet 类的实现
//

#include "stdafx.h"
#include "jisuan.h"
#include "jisuanSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CjisuanSet 实现

// 代码生成在 2005年8月16日, 10:27

IMPLEMENT_DYNAMIC(CjisuanSet, CRecordset)

CjisuanSet::CjisuanSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_fptd1 = 0.0;
	m_fptd2 = 0.0;
	m_fptm1 = 0.0;
	m_fptm2 = 0.0;
	m_fpti0 = 0.0;
	m_fpti1 = 0.0;
	m_fpti2 = 0.0;
	m_fpti3 = 0.0;
	m_fpti4 = 0.0;
	m_fpti5 = 0.0;
	m_fpti6 = 0.0;
	m_fpti7 = 0.0;
	m_fpti8 = 0.0;
	m_fpti9 = 0.0;
	m_fpti10 = 0.0;
	m_fpti11 = 0.0;
	m_fpti12 = 0.0;
	m_Frd1 = 0.0;
	m_Frd2 = 0.0;
	m_Frm1 = 0.0;
	m_Frm2 = 0.0;
	m_Fri0 = 0.0;
	m_Fri1 = 0.0;
	m_Fri2 = 0.0;
	m_Fri3 = 0.0;
	m_Fri4 = 0.0;
	m_Fri5 = 0.0;
	m_Fri6 = 0.0;
	m_Fri7 = 0.0;
	m_Fri8 = 0.0;
	m_Fri9 = 0.0;
	m_Fri10 = 0.0;
	m_Fri11 = 0.0;
	m_Fri12 = 0.0;
	m_fhad1 = 0.0;
	m_fhad2 = 0.0;
	m_fham1 = 0.0;
	m_fham2 = 0.0;
	m_fhai0 = 0.0;
	m_fhai1 = 0.0;
	m_fhai2 = 0.0;
	m_fhai3 = 0.0;
	m_fhai4 = 0.0;
	m_fhai5 = 0.0;
	m_fhai6 = 0.0;
	m_fhai7 = 0.0;
	m_fhai8 = 0.0;
	m_fhai9 = 0.0;
	m_fhai10 = 0.0;
	m_fhai11 = 0.0;
	m_fhai12 = 0.0;
	m_ffad1 = 0.0;
	m_ffad2 = 0.0;
	m_ffam1 = 0.0;
	m_ffam2 = 0.0;
	m_ffai0 = 0.0;
	m_ffai1 = 0.0;
	m_ffai2 = 0.0;
	m_ffai3 = 0.0;
	m_ffai4 = 0.0;
	m_ffai5 = 0.0;
	m_ffai6 = 0.0;
	m_ffai7 = 0.0;
	m_ffai8 = 0.0;
	m_ffai9 = 0.0;
	m_ffai10 = 0.0;
	m_ffai11 = 0.0;
	m_ffai12 = 0.0;
	m_nFields = 68;
	m_nDefaultType = dynaset;
}
///#error Security Issue: The connection string may contain a password
// 此连接字符串中可能包含密码
// 下面的连接字符串中可能包含明文密码和/或
// 其他重要信息。请在查看完
// 此连接字符串并找到所有与安全有关的问题后移除 #error。可能需要
// 将此密码存储为其他格式或使用其他的用户身份验证。
CString CjisuanSet::GetDefaultConnect()
{
	//return _T("DSN=jisuan;DBQ=E:\\\x6570\x636e\x5e93\\jisuan.mdb;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
    return _T("DSN=jisuan;DBQ=E:\\数据库\\jisuan.mdb;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString CjisuanSet::GetDefaultSQL()
{
	return _T("[极限偏差fpt],[跳动公差Fr],[齿向公差fha],[齿形公差ffa]");
}

void CjisuanSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() 和 RFX_Int() 这类宏依赖的是
// 成员变量的类型，而不是数据库字段的类型。
// ODBC 尝试自动将列值转换为所请求的类型
	RFX_Double(pFX, _T("[极限偏差fpt].[d1]"), m_fptd1);
	RFX_Double(pFX, _T("[极限偏差fpt].[d2]"), m_fptd2);
	RFX_Double(pFX, _T("[极限偏差fpt].[m1]"), m_fptm1);
	RFX_Double(pFX, _T("[极限偏差fpt].[m2]"), m_fptm2);
	RFX_Double(pFX, _T("[极限偏差fpt].[i0]"), m_fpti0);
	RFX_Double(pFX, _T("[极限偏差fpt].[i1]"), m_fpti1);
	RFX_Double(pFX, _T("[极限偏差fpt].[i2]"), m_fpti2);
	RFX_Double(pFX, _T("[极限偏差fpt].[i3]"), m_fpti3);
	RFX_Double(pFX, _T("[极限偏差fpt].[i4]"), m_fpti4);
	RFX_Double(pFX, _T("[极限偏差fpt].[i5]"), m_fpti5);
	RFX_Double(pFX, _T("[极限偏差fpt].[i6]"), m_fpti6);
	RFX_Double(pFX, _T("[极限偏差fpt].[i7]"), m_fpti7);
	RFX_Double(pFX, _T("[极限偏差fpt].[i8]"), m_fpti8);
	RFX_Double(pFX, _T("[极限偏差fpt].[i9]"), m_fpti9);
	RFX_Double(pFX, _T("[极限偏差fpt].[i10]"), m_fpti10);
	RFX_Double(pFX, _T("[极限偏差fpt].[i11]"), m_fpti11);
	RFX_Double(pFX, _T("[极限偏差fpt].[i12]"), m_fpti12);
	RFX_Double(pFX, _T("[跳动公差Fr].[d1]"), m_Frd1);
	RFX_Double(pFX, _T("[跳动公差Fr].[d2]"), m_Frd2);
	RFX_Double(pFX, _T("[跳动公差Fr].[m1]"), m_Frm1);
	RFX_Double(pFX, _T("[跳动公差Fr].[m2]"), m_Frm2);
	RFX_Double(pFX, _T("[跳动公差Fr].[i0]"), m_Fri0);
	RFX_Double(pFX, _T("[跳动公差Fr].[i1]"), m_Fri1);
	RFX_Double(pFX, _T("[跳动公差Fr].[i2]"), m_Fri2);
	RFX_Double(pFX, _T("[跳动公差Fr].[i3]"), m_Fri3);
	RFX_Double(pFX, _T("[跳动公差Fr].[i4]"), m_Fri4);
	RFX_Double(pFX, _T("[跳动公差Fr].[i5]"), m_Fri5);
	RFX_Double(pFX, _T("[跳动公差Fr].[i6]"), m_Fri6);
	RFX_Double(pFX, _T("[跳动公差Fr].[i7]"), m_Fri7);
	RFX_Double(pFX, _T("[跳动公差Fr].[i8]"), m_Fri8);
	RFX_Double(pFX, _T("[跳动公差Fr].[i9]"), m_Fri9);
	RFX_Double(pFX, _T("[跳动公差Fr].[i10]"), m_Fri10);
	RFX_Double(pFX, _T("[跳动公差Fr].[i11]"), m_Fri11);
	RFX_Double(pFX, _T("[跳动公差Fr].[i12]"), m_Fri12);
	RFX_Double(pFX, _T("[齿向公差fha].[d1]"), m_fhad1);
	RFX_Double(pFX, _T("[齿向公差fha].[d2]"), m_fhad2);
	RFX_Double(pFX, _T("[齿向公差fha].[m1]"), m_fham1);
	RFX_Double(pFX, _T("[齿向公差fha].[m2]"), m_fham2);
	RFX_Double(pFX, _T("[齿向公差fha].[i0]"), m_fhai0);
	RFX_Double(pFX, _T("[齿向公差fha].[i1]"), m_fhai1);
	RFX_Double(pFX, _T("[齿向公差fha].[i2]"), m_fhai2);
	RFX_Double(pFX, _T("[齿向公差fha].[i3]"), m_fhai3);
	RFX_Double(pFX, _T("[齿向公差fha].[i4]"), m_fhai4);
	RFX_Double(pFX, _T("[齿向公差fha].[i5]"), m_fhai5);
	RFX_Double(pFX, _T("[齿向公差fha].[i6]"), m_fhai6);
	RFX_Double(pFX, _T("[齿向公差fha].[i7]"), m_fhai7);
	RFX_Double(pFX, _T("[齿向公差fha].[i8]"), m_fhai8);
	RFX_Double(pFX, _T("[齿向公差fha].[i9]"), m_fhai9);
	RFX_Double(pFX, _T("[齿向公差fha].[i10]"), m_fhai10);
	RFX_Double(pFX, _T("[齿向公差fha].[i11]"), m_fhai11);
	RFX_Double(pFX, _T("[齿向公差fha].[i12]"), m_fhai12);
	RFX_Double(pFX, _T("[齿形公差ffa].[d1]"), m_ffad1);
	RFX_Double(pFX, _T("[齿形公差ffa].[d2]"), m_ffad2);
	RFX_Double(pFX, _T("[齿形公差ffa].[m1]"), m_ffam1);
	RFX_Double(pFX, _T("[齿形公差ffa].[m2]"), m_ffam2);
	RFX_Double(pFX, _T("[齿形公差ffa].[i0]"), m_ffai0);
	RFX_Double(pFX, _T("[齿形公差ffa].[i1]"), m_ffai1);
	RFX_Double(pFX, _T("[齿形公差ffa].[i2]"), m_ffai2);
	RFX_Double(pFX, _T("[齿形公差ffa].[i3]"), m_ffai3);
	RFX_Double(pFX, _T("[齿形公差ffa].[i4]"), m_ffai4);
	RFX_Double(pFX, _T("[齿形公差ffa].[i5]"), m_ffai5);
	RFX_Double(pFX, _T("[齿形公差ffa].[i6]"), m_ffai6);
	RFX_Double(pFX, _T("[齿形公差ffa].[i7]"), m_ffai7);
	RFX_Double(pFX, _T("[齿形公差ffa].[i8]"), m_ffai8);
	RFX_Double(pFX, _T("[齿形公差ffa].[i9]"), m_ffai9);
	RFX_Double(pFX, _T("[齿形公差ffa].[i10]"), m_ffai10);
	RFX_Double(pFX, _T("[齿形公差ffa].[i11]"), m_ffai11);
	RFX_Double(pFX, _T("[齿形公差ffa].[i12]"), m_ffai12);

}
/////////////////////////////////////////////////////////////////////////////
// CjisuanSet 诊断

#ifdef _DEBUG
void CjisuanSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CjisuanSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

