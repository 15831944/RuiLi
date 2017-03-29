// jisuanSet.cpp : CjisuanSet ���ʵ��
//

#include "stdafx.h"
#include "jisuan.h"
#include "jisuanSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CjisuanSet ʵ��

// ���������� 2005��8��16��, 10:27

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
// �������ַ����п��ܰ�������
// ����������ַ����п��ܰ������������/��
// ������Ҫ��Ϣ�����ڲ鿴��
// �������ַ������ҵ������밲ȫ�йص�������Ƴ� #error��������Ҫ
// ��������洢Ϊ������ʽ��ʹ���������û������֤��
CString CjisuanSet::GetDefaultConnect()
{
	//return _T("DSN=jisuan;DBQ=E:\\\x6570\x636e\x5e93\\jisuan.mdb;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
    return _T("DSN=jisuan;DBQ=E:\\���ݿ�\\jisuan.mdb;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString CjisuanSet::GetDefaultSQL()
{
	return _T("[����ƫ��fpt],[��������Fr],[���򹫲�fha],[���ι���ffa]");
}

void CjisuanSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Double(pFX, _T("[����ƫ��fpt].[d1]"), m_fptd1);
	RFX_Double(pFX, _T("[����ƫ��fpt].[d2]"), m_fptd2);
	RFX_Double(pFX, _T("[����ƫ��fpt].[m1]"), m_fptm1);
	RFX_Double(pFX, _T("[����ƫ��fpt].[m2]"), m_fptm2);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i0]"), m_fpti0);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i1]"), m_fpti1);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i2]"), m_fpti2);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i3]"), m_fpti3);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i4]"), m_fpti4);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i5]"), m_fpti5);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i6]"), m_fpti6);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i7]"), m_fpti7);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i8]"), m_fpti8);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i9]"), m_fpti9);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i10]"), m_fpti10);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i11]"), m_fpti11);
	RFX_Double(pFX, _T("[����ƫ��fpt].[i12]"), m_fpti12);
	RFX_Double(pFX, _T("[��������Fr].[d1]"), m_Frd1);
	RFX_Double(pFX, _T("[��������Fr].[d2]"), m_Frd2);
	RFX_Double(pFX, _T("[��������Fr].[m1]"), m_Frm1);
	RFX_Double(pFX, _T("[��������Fr].[m2]"), m_Frm2);
	RFX_Double(pFX, _T("[��������Fr].[i0]"), m_Fri0);
	RFX_Double(pFX, _T("[��������Fr].[i1]"), m_Fri1);
	RFX_Double(pFX, _T("[��������Fr].[i2]"), m_Fri2);
	RFX_Double(pFX, _T("[��������Fr].[i3]"), m_Fri3);
	RFX_Double(pFX, _T("[��������Fr].[i4]"), m_Fri4);
	RFX_Double(pFX, _T("[��������Fr].[i5]"), m_Fri5);
	RFX_Double(pFX, _T("[��������Fr].[i6]"), m_Fri6);
	RFX_Double(pFX, _T("[��������Fr].[i7]"), m_Fri7);
	RFX_Double(pFX, _T("[��������Fr].[i8]"), m_Fri8);
	RFX_Double(pFX, _T("[��������Fr].[i9]"), m_Fri9);
	RFX_Double(pFX, _T("[��������Fr].[i10]"), m_Fri10);
	RFX_Double(pFX, _T("[��������Fr].[i11]"), m_Fri11);
	RFX_Double(pFX, _T("[��������Fr].[i12]"), m_Fri12);
	RFX_Double(pFX, _T("[���򹫲�fha].[d1]"), m_fhad1);
	RFX_Double(pFX, _T("[���򹫲�fha].[d2]"), m_fhad2);
	RFX_Double(pFX, _T("[���򹫲�fha].[m1]"), m_fham1);
	RFX_Double(pFX, _T("[���򹫲�fha].[m2]"), m_fham2);
	RFX_Double(pFX, _T("[���򹫲�fha].[i0]"), m_fhai0);
	RFX_Double(pFX, _T("[���򹫲�fha].[i1]"), m_fhai1);
	RFX_Double(pFX, _T("[���򹫲�fha].[i2]"), m_fhai2);
	RFX_Double(pFX, _T("[���򹫲�fha].[i3]"), m_fhai3);
	RFX_Double(pFX, _T("[���򹫲�fha].[i4]"), m_fhai4);
	RFX_Double(pFX, _T("[���򹫲�fha].[i5]"), m_fhai5);
	RFX_Double(pFX, _T("[���򹫲�fha].[i6]"), m_fhai6);
	RFX_Double(pFX, _T("[���򹫲�fha].[i7]"), m_fhai7);
	RFX_Double(pFX, _T("[���򹫲�fha].[i8]"), m_fhai8);
	RFX_Double(pFX, _T("[���򹫲�fha].[i9]"), m_fhai9);
	RFX_Double(pFX, _T("[���򹫲�fha].[i10]"), m_fhai10);
	RFX_Double(pFX, _T("[���򹫲�fha].[i11]"), m_fhai11);
	RFX_Double(pFX, _T("[���򹫲�fha].[i12]"), m_fhai12);
	RFX_Double(pFX, _T("[���ι���ffa].[d1]"), m_ffad1);
	RFX_Double(pFX, _T("[���ι���ffa].[d2]"), m_ffad2);
	RFX_Double(pFX, _T("[���ι���ffa].[m1]"), m_ffam1);
	RFX_Double(pFX, _T("[���ι���ffa].[m2]"), m_ffam2);
	RFX_Double(pFX, _T("[���ι���ffa].[i0]"), m_ffai0);
	RFX_Double(pFX, _T("[���ι���ffa].[i1]"), m_ffai1);
	RFX_Double(pFX, _T("[���ι���ffa].[i2]"), m_ffai2);
	RFX_Double(pFX, _T("[���ι���ffa].[i3]"), m_ffai3);
	RFX_Double(pFX, _T("[���ι���ffa].[i4]"), m_ffai4);
	RFX_Double(pFX, _T("[���ι���ffa].[i5]"), m_ffai5);
	RFX_Double(pFX, _T("[���ι���ffa].[i6]"), m_ffai6);
	RFX_Double(pFX, _T("[���ι���ffa].[i7]"), m_ffai7);
	RFX_Double(pFX, _T("[���ι���ffa].[i8]"), m_ffai8);
	RFX_Double(pFX, _T("[���ι���ffa].[i9]"), m_ffai9);
	RFX_Double(pFX, _T("[���ι���ffa].[i10]"), m_ffai10);
	RFX_Double(pFX, _T("[���ι���ffa].[i11]"), m_ffai11);
	RFX_Double(pFX, _T("[���ι���ffa].[i12]"), m_ffai12);

}
/////////////////////////////////////////////////////////////////////////////
// CjisuanSet ���

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

