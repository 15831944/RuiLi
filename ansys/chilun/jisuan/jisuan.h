// jisuan.h : jisuan Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // ������


// CjisuanApp:
// �йش����ʵ�֣������ jisuan.cpp
//

class CjisuanApp : public CWinApp
{
public:
	CjisuanApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CjisuanApp theApp;
