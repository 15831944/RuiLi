// ansys.h : ansys DLL ����ͷ�ļ�
//

#pragma once
#include "comdef.h"

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������

// CansysApp
// �йش���ʵ�ֵ���Ϣ������� ansys.cpp
//

class CansysApp : public CWinApp
{
public:
	CansysApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
