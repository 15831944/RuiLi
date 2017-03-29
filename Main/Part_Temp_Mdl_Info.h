// Part_Temp_Mdl_Info.h : CPart_Temp_Mdl_Info ������

#pragma once

// ���������� 2005��6��24��, 15:14

class CPart_Temp_Mdl_Info : public CRecordset
{
public:
	CPart_Temp_Mdl_Info(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPart_Temp_Mdl_Info)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	long	m_Number;
	long	m_FatherNum;
	CStringA	m_Name;
	CStringA	m_Type;
	CTime	m_Datatime;
	CStringA	m_Note;
	CStringA	m_FileName;
	CStringA	m_FileDirectory;
	CStringA	m_DrawFileName;
	CStringA	m_DrawFileDirectory;
	CStringA	m_MdlerName;
	CStringA	m_MdlerDepartment;
	long     m_MdlFatherNum;
	CStringA  m_MdlCode;
	long     m_MdlTypeNum;
	CStringA m_MdlTypeName;
	CStringA     m_Status;
	long		 m_MdlerId;
	CStringA     m_Material;

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


