// PicViewDlg.cpp : ʵ���ļ�
//ͼƬԤ���ĶԻ���

#include "stdafx.h"
#include "Main.h"
#include "PicViewDlg.h"
#include <ocidl.h> 
#include <olectl.h> 
extern int DlgIcon;

//extern HRESULT ShowPic(char *lpstrFile,HWND hWnd,int nScrWidth,int nScrHeight); 

// CPicViewDlg �Ի���

IMPLEMENT_DYNAMIC(CPicViewDlg, CDialog)
CPicViewDlg::CPicViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicViewDlg::IDD, pParent)
	, file(_T(""))
	, p_Picture(NULL)
	, mode(-1)
{
}

CPicViewDlg::~CPicViewDlg()
{
}

void CPicViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_JPGSHOWCTLCTRL1, m_pic);
	//DDX_Control(pDX, IDC_MYBITMAP, m_pic);
	DDX_Control(pDX, IDC_MYBITMAP, m_pic);
}


BEGIN_MESSAGE_MAP(CPicViewDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CPicViewDlg ��Ϣ�������

BOOL CPicViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}


void CPicViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ������Ϣ����������
//////////////////////////////////////////////////////////////////////////////////////////
	/////3.������
	CSize	sz;
	CRect	rect;
	switch(mode) {
	case 0:
		sz=pic.GetSize();
		GetWindowRect(&rect);
		rect.right=rect.left+sz.cx+18;
		rect.bottom=rect.top+sz.cy+30;
		MoveWindow(&rect,TRUE);
		GetClientRect(&rect);
		m_pic.MoveWindow(&rect,TRUE);
		pic.Draw(&dc,0,0,rect.Width(),rect.Height(),0,0,sz.cx,sz.cy);
		break;
	case 1:
		if (p_Picture!=NULL)
		{
			GetWindowRect(&rect);
			p_Picture->get_Width(&sz.cx);
			p_Picture->get_Height(&sz.cy);
			dc.HIMETRICtoDP(&sz);
			rect.right=rect.left+sz.cx+18;
			rect.bottom=rect.top+sz.cy+30;
			MoveWindow(&rect,TRUE);
			//m_pic.MoveWindow(rect.left,rect.top,rect.left+sz.cx+10,rect.top+sz.cy+10,TRUE);
			GetClientRect(&rect);
			m_pic.MoveWindow(&rect,TRUE);
			dc.DPtoHIMETRIC(&sz);
			p_Picture->Render(dc,0,rect.Height(),rect.Width(),-rect.Height(),0,0,sz.cx,sz.cy,NULL);
		}
		break;
	default:
		break;
	}

/////////////////////////////////////////////////////////////////////////////////////////////
	//////////1.����һ
	 //CDC * pControlDC=this->GetWindowDC();
	 //CWnd *pWnd=this;
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
	 //pControlDC->BitBlt(10,20,bmInfo.bmWidth,bmInfo.bmHeight,&dcComp,0,0,SRCCOPY);
	 //pWnd->ReleaseDC(pControlDC);
/////////////////////////////////////////////////////////////////////////////////////////////////////
	////////2.������
	//CWnd * pWnd=GetDlgItem(IDC_MYBITMAP);
	//CDC * pControlDC=pWnd->GetDC();
 //  
	//CBitmap bitmap;
	//if(file=="") 
	//{
	//	AfxMessageBox("·��û�����ã�");
	//	return;
	//}
	//HBITMAP hbitmap=(HBITMAP)LoadImage(NULL,file,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	//if(hbitmap==NULL) 
	//{
	//	CFont NewFont;
	//	NewFont.CreateFont(50,0,0,0,
	//		FW_NORMAL,
	//		0,0,0,
	//		ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
	//		DEFAULT_PITCH&FF_SWISS,
	//		"Arial");
	//	CFont *pOldFont=(CFont*)pControlDC->SelectObject(&NewFont);
	//	pControlDC->TextOut(0,80,"ͼƬ������,�����ѱ�ɾ����");
	//	pControlDC->SelectObject(pOldFont);
	//	return;
	//}
	//bitmap.Attach(hbitmap);
	//pWnd->Invalidate();
	//pWnd->UpdateWindow();
	//CDC dcComp;
	//dcComp.CreateCompatibleDC(pControlDC);
	//dcComp.SelectObject(&bitmap);
	//BITMAP bmInfo;
	//bitmap.GetObject(sizeof(bmInfo),&bmInfo);
	//MoveWindow(0,0,bmInfo.bmWidth+20,bmInfo.bmHeight+35,TRUE);
	//m_pic.MoveWindow(0,0,bmInfo.bmWidth+13,bmInfo.bmHeight+10,TRUE);
	//pControlDC->BitBlt(0,0,bmInfo.bmWidth,bmInfo.bmHeight,&dcComp,0,0,SRCCOPY);
 //   pWnd->ReleaseDC(pControlDC);
//////////////////////////////////////////////////////////////////////////////////////////////////
}

void CPicViewDlg::SetFile(CString filePathName)
{
	file=filePathName;
	mode=0;
	if(!pic.LoadPictureFromFile(filePathName))		
	{
		AfxMessageBox("����ͼƬʧ�ܣ�");
		CDialog::OnClose();
	}
}
void CPicViewDlg::SetIpicture(IPicture * pIpitcure)
{
	p_Picture=pIpitcure;
	mode=1;
}
void CPicViewDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	UsrRepaintWindow();
	// TODO: �ڴ������Ϣ����������
}
