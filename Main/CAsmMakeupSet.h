// CAsmMakeupSet.h : CAsmMakeupSet ������

#pragma once

// ���������� 2005��7��8��, 13:00

class CAsmMakeupSet : public CRecordset
{
public:
	CAsmMakeupSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CAsmMakeupSet)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	long	m_AsmNum;
	long	m_PartNum;
	long	m_PartMdlNum;
	long    m_DefInsNum;
	CStringA	m_Type;
	long	m_UpMdlNum;
	CStringA	m_Name;
	CStringA	m_FileName;
	CStringA	m_Note;
	CStringA    m_Code;
	

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


