// AsmTempPerformanceTable.h : CAsmTempPerformanceTable ������

#pragma once

// ���������� 2005��7��27��, 20:57

class CAsmTempPerformanceTable : public CRecordset
{
public:
	CAsmTempPerformanceTable(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CAsmTempPerformanceTable)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	long	m_AsmNum;
	long	m_Number;
	CStringA	m_Name;
	CStringA	m_Value;
	CStringA	m_Note;
	CStringA    m_Type;
	double      m_MaxVal;
	double      m_MinVal;
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


