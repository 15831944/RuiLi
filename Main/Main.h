// Main.h : Main DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CMainApp
// �йش���ʵ�ֵ���Ϣ������� Main.cpp
//

class CMainApp : public CWinApp
{
public:
	CMainApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTypeRename();
};
