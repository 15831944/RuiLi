// XtPdmDocuments.h : CXtPdmDocuments ������

#pragma once

// ���������� 2005��12��20��, 14:07

class CXtPdmDocuments : public CRecordset
{
public:
	CXtPdmDocuments(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CXtPdmDocuments)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	CStringA	m_ProductDwgCode;
	CStringA	m_Code;
	CStringA	m_DocumentType;
	CStringA	m_Directory;
	CStringA	m_FileName;
	CStringA	m_FileType;
	CStringA	m_FileBulk;
	CStringA	m_FileDate;
	CStringA	m_FileRemark;
	CStringA	m_RevNumber;

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


