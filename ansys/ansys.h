// ansys.h : ansys DLL 的主头文件
//

#pragma once
#include "comdef.h"

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主符号

// CansysApp
// 有关此类实现的信息，请参阅 ansys.cpp
//

class CansysApp : public CWinApp
{
public:
	CansysApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
