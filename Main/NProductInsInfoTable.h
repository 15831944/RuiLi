// NProductInsInfoTable.h : CNProductInsInfoTable ������

#pragma once

// ���������� 2005��9��17��, 12:02

class CNProductInsInfoTable : public CRecordset
{
public:
	CNProductInsInfoTable(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CNProductInsInfoTable)

// �ֶ�/��������

// �����ַ������ͣ�������ڣ���ӳ���ݿ��ֶΣ�ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW����ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// ��ע�⣺����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת������

	long	m_PdtNum;
	CStringA	m_InsCode;
	/*CStringA	m_GrpFrm;*/
	CStringA	m_Name;
	CStringA	m_Unit;
	long	m_MdlNum;
	CStringA	m_Designer;
	CTime	m_FinishTime;
	CStringA	m_Auditor;
	CStringA	m_AuditTime;
	CStringA  m_SolidFileName;
	CStringA  m_SolidFilePlace;
	CStringA  m_DrawingFileName;
	CStringA  m_DrawingFilePlace;
	CStringA	m_FinishState;
	CStringA	m_AuditState;
	CStringA	m_PDMState;
	CStringA	m_PDMNum;
	CStringA	m_Note;
	CStringA    m_LocalState;
	long        m_TypeNum;

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


