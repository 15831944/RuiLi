// CAsmRvRelDimParaSet.h : CAsmRvRelDimParaSet ������

#pragma once

// ���������� 2005��7��8��, 13:02

class CAsmRvRelDimParaSet : public CRecordset
{
public:
	CAsmRvRelDimParaSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CAsmRvRelDimParaSet)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	long	m_ProductMdlNum;
	long	m_Number;
	long	m_PostNum;
	CStringA	m_Type;
	CStringA	m_IdNum;
	CStringA	m_Name;
	CStringA	m_DimSymbol;
	CStringA	m_SubType;
	CStringA	m_Value;

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


