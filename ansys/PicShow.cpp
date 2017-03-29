// PicShow.cpp : 实现文件
//

#include "stdafx.h"
#include "ansys.h"
#include "PicShow.h"


// CPicShow

IMPLEMENT_DYNAMIC(CPicShow, CStatic)
CPicShow::CPicShow()
: file(_T(""))
{
}

CPicShow::~CPicShow()
{
}


BEGIN_MESSAGE_MAP(CPicShow, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT(STN_DBLCLK, OnStnDblclick)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CPicShow 消息处理程序


void CPicShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()
	CDC * pControlDC=this->GetWindowDC();
	CWnd *pWnd=this;

	if(file=="") 
	{

		CFont NewFont;
		NewFont.CreateFont(50,0,0,0,
			FW_NORMAL,
			0,0,0,
			ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
			DEFAULT_PITCH&FF_SWISS,
			"Arial");
		CFont *pOldFont=(CFont*)pControlDC->SelectObject(&NewFont);
		//pWnd->Invalidate();
		//pWnd->UpdateWindow();
		pControlDC->TextOut(5,80,"无图片！");
		pControlDC->SelectObject(pOldFont);
		pWnd->ReleaseDC(pControlDC);
		return;
	}
/////////////////////////////////////////////////////////////////////////////////
   
	//CSize sz = pic.GetSize();
	//CSize sz=this->getwiGetWindowSize();
	//pWnd->Invalidate();
	CRect rect;
	GetWindowRect(&rect);
	CSize sz=pic.GetSize();
	pic.Draw(pControlDC,0,0,rect.Width(),rect.Height(),0,0,sz.cx,sz.cy);
    pWnd->ReleaseDC(pControlDC);
///////////////////////////////////////////////////////////////////////////////////



	//CBitmap bitmap;
	//   HBITMAP hbitmap=(HBITMAP)LoadImage(NULL,file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	//   bitmap.Attach(hbitmap);
	////pWnd->Invalidate();
	////pWnd->UpdateWindow();
	//CDC dcComp;
	//dcComp.CreateCompatibleDC(pControlDC);
	//dcComp.SelectObject(&bitmap);
	//BITMAP bmInfo;
	//bitmap.GetObject(sizeof(bmInfo),&bmInfo);

	//CRect rect;
	//GetWindowRect(&rect);
	//pControlDC->StretchBlt(2,2,rect.Width()-4,rect.Height()-4,&dcComp,0,0,bmInfo.bmWidth,bmInfo.bmHeight,SRCCOPY);
	////pControlDC->BitBlt(10,20,bmInfo.bmWidth,bmInfo.bmHeight,&dcComp,0,0,SRCCOPY);
	//pWnd->ReleaseDC(pControlDC);
}

int CPicShow::SetFile(CString filename)
{
	file=filename;
	pic.LoadPictureFromFile(file);
	//file="G:\\Pictures\\2.jpg";
	return 0;
}

BOOL CPicShow::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CStatic::OnEraseBkgnd(pDC);
}

void CPicShow::OnStnDblclick()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CPicShow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//dlg.Create(IDD_DIALOG5);
	//dlg.SetFile(file);
	//dlg.ShowWindow(SW_SHOW);

	CStatic::OnLButtonDblClk(nFlags, point);
}
