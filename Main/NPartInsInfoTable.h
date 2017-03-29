// NPartInsInfoTable.h : CNPartInsInfoTable ������

#pragma once

// ���������� 2005��9��15��, 13:22

class CNPartInsInfoTable : public CRecordset
{
public:
	CNPartInsInfoTable(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CNPartInsInfoTable)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	long	m_PrtNum;
	CStringA	m_Name;
	CStringA	m_InsCode;
	long	m_TypeNum;
	long	m_MdlNum;
	CStringA	m_Material;
	CStringA	m_Unit;
	CStringA	m_Designer;
	CTime	m_FinishTime;
	CStringA	m_Auditor;
	CTime	m_AuditTime;
	CStringA	m_SolidFileName;
	CStringA	m_SolidPlace;
	CStringA	m_DrawingName;
	CStringA	m_DrawingPlace;
	CStringA	m_FinishState;
	CStringA	m_AuditState;
	CStringA	m_PDMState;
	CStringA	m_PDMNum;
	CStringA	m_Note;
	CStringA	m_LocalState;
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


