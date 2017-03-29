#pragma once

#include "PreViewDlg.h"

// CPreviewThread 命令目标

class CPreviewThread : public CObject
{
public:
	CPreviewThread();
	virtual ~CPreviewThread();
public:
	int m_ModalFlag;//1,零件模型 2，零件实例 3，产品模型 4，产品实例
	int m_2DFlag;//0,没有2D图，1 有2D图
	CString m_strID;///模型id,实例id等。
	CPreViewDlg * pPreviewDlg;
public:
	int Begin();

};


