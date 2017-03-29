// ffa.h : Cffa ������

#pragma once

// ���������� 2005��8��17��, 10:31

class Cffa : public CRecordset
{
public:
	Cffa(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(Cffa)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	long	m_index;
	double	m_d1;
	double	m_d2;
	double	m_m1;
	double	m_m2;
	double	m_i0;
	double	m_i1;
	double	m_i2;
	double	m_i3;
	double	m_i4;
	double	m_i5;
	double	m_i6;
	double	m_i7;
	double	m_i8;
	double	m_i9;
	double	m_i10;
	double	m_i11;
	double	m_i12;

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


