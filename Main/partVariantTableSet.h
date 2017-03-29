// partVariantTableSet.h : CpartVariantTableSet ������

#pragma once

// ���������� 2005��6��15��, 21:12

class CpartVariantTableSet : public CRecordset
{
public:
	CpartVariantTableSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CpartVariantTableSet)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	long	m_PrtNum;
	long	m_Number;
	CStringA	m_Name;
	CStringA	m_Type;
	CStringA	m_IdNum;
	CStringA	m_DimSymbol;
	CStringA	m_ParaFeatName;
	CStringA	m_SubType;
	CStringA	m_Value;
	CStringA	m_UpLimit;
	CStringA	m_DwLimit;
	CStringA	m_Note;

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


