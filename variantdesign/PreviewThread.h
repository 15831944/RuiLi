#pragma once

#include "PreViewDlg.h"

// CPreviewThread ����Ŀ��

class CPreviewThread : public CObject
{
public:
	CPreviewThread();
	virtual ~CPreviewThread();
public:
	int m_ModalFlag;//1,���ģ�� 2�����ʵ�� 3����Ʒģ�� 4����Ʒʵ��
	int m_2DFlag;//0,û��2Dͼ��1 ��2Dͼ
	CString m_strID;///ģ��id,ʵ��id�ȡ�
	CPreViewDlg * pPreviewDlg;
public:
	int Begin();

};


