// VariantFn.h : VariantFn DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CVariantFnApp
// �йش���ʵ�ֵ���Ϣ������� VariantFn.cpp
//

class CVariantFnApp : public CWinApp
{
public:
	CVariantFnApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
