// variantdesign.h : variantdesign DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������
#include "ProMenu.h"
#include "ProUtil.h"
#include "ProMenubar.h"


// CvariantdesignApp
// �йش���ʵ�ֵ���Ϣ������� variantdesign.cpp
//

class CvariantdesignApp : public CWinApp
{
public:
	CvariantdesignApp();
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
