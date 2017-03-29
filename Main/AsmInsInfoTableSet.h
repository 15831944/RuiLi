// AsmInsInfoTableSet.h : CAsmInsInfoTableSet ������

#pragma once

// ���������� 2005��9��7��, 9:46

class CAsmInsInfoTableSet : public CRecordset
{
public:
	CAsmInsInfoTableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CAsmInsInfoTableSet)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	long	m_AsmNum;
	long	m_InsNum;
	CStringA	m_InsName;
	CStringA	m_InsCode;
	CStringA	m_SavePlace;
	CTime	m_InsCreateTime;
	CStringA	m_InsCreaterName;
	CStringA	m_InsCreateDpt;
	CStringA	m_InsNote;

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


