// AsmTempManageTableSet.h : CAsmTempManageTableSet ������

#pragma once

// ���������� 2005��7��25��, 9:58

class CAsmTempManageTableSet : public CRecordset
{
public:
	CAsmTempManageTableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CAsmTempManageTableSet)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	long	m_Number;
	long	m_FatherNumber;
	CStringA	m_Name;
	CStringA	m_Type;
	CStringA	m_MdlCode;
	CTime	m_Datatime;
	CStringA	m_FileName;
	CStringA	m_FileDirectory;
	CStringA	m_MdlerName;
	CStringA	m_MdlerDepartment;
	CStringA	m_Note;
	long        m_MdlTypeNum;
	CStringA    m_MdlTypeName;
	CStringA   m_DrawFileName;
	CStringA   m_DrawFileDirectory;

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


