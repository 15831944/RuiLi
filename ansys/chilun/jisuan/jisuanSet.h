// jisuanSet.h: CjisuanSet ��Ľӿ�
//


#pragma once

// ���������� 2005��8��16��, 10:27

class CjisuanSet : public CRecordset
{
public:
	CjisuanSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CjisuanSet)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

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

// ��д
	// �����ɵ��麯����д
	public:
	virtual CString GetDefaultConnect();	// Ĭ�������ַ���

	virtual CString GetDefaultSQL(); 	// ��¼����Ĭ�� SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

