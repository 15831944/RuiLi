// PicMakgeDlg.cpp : 实现文件
//生成浏览图片的主对话框

//内部的动态连接库的地址需要修改
#include "stdafx.h"
#include "Main.h"
#include "PicMakgeDlg.h"
#include "PicEditBoxDlg.h"
#include "PicViewDlg.h"
#include "PicEditDlg.h"

//extern int DlgIcon;
//extern CArray<UsrPicture,UsrPicture>   p_UsrPicture; 
typedef  bool(__stdcall CHANGE)(char *,char *);
// CPicMakgeDlg 对话框
extern "C" __declspec(dllimport) BOOL SaveBitmapAsJpeg(CDC * dc/*tdc*/,CBitmap & bitmap,int nQuality,CString filename);

IMPLEMENT_DYNAMIC(CPicMakgeDlg, CDialog)
CPicMakgeDlg::CPicMakgeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicMakgeDlg::IDD, pParent)
	, m_flag(0)
	, m_nSetp(0)
	, m_nStartX(0)
	, m_nStartY(0)
	, m_nEndX(0)
	, m_nEndY(0)
	, csPath(_T("c:\\"))  //默认路径
	, fileName(_T(""))
	, m_Width(0)
	, m_Height(0)
	, m_pic_width(0)
	, m_pic_height(0)
	, list_dlg(NULL)
{
}

CPicMakgeDlg::~CPicMakgeDlg()
{
	if(list_dlg!=NULL)
		delete list_dlg;
}

void CPicMakgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON2, m_CreatButton);
	DDX_Control(pDX, IDC_BUTTON1, m_SelectButton);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_pic_width);
	DDX_Text(pDX, IDC_EDIT2, m_pic_height);
	DDX_Control(pDX, IDC_SLIDER1, m_slidercrtrl);
	DDX_Control(pDX, IDC_COMBO1, m_cobGuiGe);
}


BEGIN_MESSAGE_MAP(CPicMakgeDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_WM_LBUTTONUP()
//	ON_WM_PAINT()
	ON_COMMAND(ID_12089, OnPicEdit)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_12090, OnDelPic)
	ON_COMMAND(ID_12091, OnPicView)
	ON_WM_CLOSE()
	ON_COMMAND(ID_12175, OnBiaozhu)
	ON_COMMAND(ID_12179, OnViewPicOfDrawing)
	ON_MESSAGE(WM_LIST_OK,OnListDlgOk)
END_MESSAGE_MAP()

BOOL CPicMakgeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	USR_SET_DLG_ICON()
	//初始化列表框
	FreshList();  
	m_slidercrtrl.SetRange(0,100);
	m_slidercrtrl.SetPos(95);
	m_CreatButton.EnableWindow(FALSE);   //生成图片按钮无效
	m_cobGuiGe.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
// 设置存储路进
void CPicMakgeDlg::SetPath(CString filePath)
{
	if (!GetCurrentMdlPath().IsEmpty())
	{
		csPath=GetCurrentMdlPath();
	}
	else
	{
		csPath=filePath;
	}
	//fileName=filePath+fileName1+".jpg";
}
// CPicMakgeDlg 消息处理程序
/************************************************************************/
/* 选取区域                                                             */
/************************************************************************/
void CPicMakgeDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
 	m_flag=1;                              //表示进入选取状态
	//如果已经画过一次了则先把上次这个先消除
	if(m_nStartX!=0)
	{
		CWindowDC pDC(this->GetOwner());
		CPen NewPen(PS_SOLID,1,RGB(0,0,255));
		CPen *OldPen=pDC.SelectObject(&NewPen);
        pDC.SetROP2(R2_NOT);
		pDC.MoveTo(m_nStartX,m_nStartY);	
		pDC.LineTo(m_nEndX,m_nStartY);
		pDC.LineTo(m_nEndX,m_nEndY);
		pDC.LineTo(m_nStartX,m_nEndY);
		pDC.LineTo(m_nStartX,m_nStartY);
        pDC.SelectObject(OldPen);
	}
	//全部设置为0
	m_nSetp=0;
	m_nStartX=0;
	m_nStartY=0;
	m_nEndX=0;
	m_nEndY=0;
	//捕捉鼠标
    SetCapture();
	//禁用按钮	
	m_CreatButton.EnableWindow(FALSE); 
	m_SelectButton.EnableWindow(FALSE);
}
/************************************************************************/
/* 鼠标左键下去之后及按下shift之后开始                                  */
/************************************************************************/
void CPicMakgeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	                                    //首先只有在同时按下左键和SHIFT键才有效
	if(!(nFlags&MK_SHIFT)) return;
	                                    //还有是已经进入点取状态
	if(m_flag!=1) 	return;

	//把客户区坐标转换成屏幕坐标
	ClientToScreen(&point);
	if(m_nSetp==0)                      //如果是起始点，则记录起点坐标
	{
		m_nStartX=point.x;
		m_nStartY=point.y;
		m_nSetp=1;
	}	


	CDialog::OnLButtonDown(nFlags, point);
}
int CPicMakgeDlg::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
/************************************************************************/
/* 移动过程中的橡皮差                                                   */
/************************************************************************/
void CPicMakgeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//首先只有在同时按下左键和SHIFT键才有效
	if (m_flag==1)
	{
		switch(m_cobGuiGe.GetCurSel()) {
	case 0:
		TypeBiaoZhun(&point,nFlags,200,200);
		break;
	case 1:
		TypeBiaoZhun(&point,nFlags,300,300);
		break;
	case 2:
		TypeBiaoZhun(&point,nFlags,400,400);
		break;
	case 3:
		TypeDIY(&point,nFlags);
		break;
	default:
		break;
		} 
	}
	CDialog::OnMouseMove(nFlags, point);
}
void CPicMakgeDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
    
	//定义存储位置
	CPoint point;
	point=CPoint(m_nStartX,m_nStartY);
	MakePic(&point,abs(m_nStartX-m_nEndX),abs(m_nStartY-m_nEndY));
	m_nStartX=0;
	m_nStartY=0;
	m_nEndX=0;
	m_nEndY=0;
	 m_CreatButton.EnableWindow(FALSE);
}
//	CDC dc;
//	dc.CreateDC("DISPLAY",NULL,NULL,NULL);    //DISPLAY   代表的是屏幕
//	//创建位图，
//	CBitmap bm;
//	int Width=abs(m_nStartX-m_nEndX);      /*GetSystemMetrics(SM_CXSCREEN);*/    //截取的高度和宽度
//	int Height=abs(m_nStartY-m_nEndY);             /*GetSystemMetrics(SM_CYSCREEN);*/
//	                                       //使创建的DDB的格式与输出设备的结构紧密相关，设备兼容
//	bm.CreateCompatibleBitmap(&dc,Width-5,Height-5);
//	CDC tdc;
//	tdc.CreateCompatibleDC(&dc);
//	CBitmap*pOld=tdc.SelectObject(&bm);
//    tdc.BitBlt(0,0,Width,Height,&dc,m_nStartX,m_nStartY,SRCCOPY);         //的6，7个参数是屏幕的起始坐标
//     //tdc.StretchBlt(0,0,Width*0.8,Height*0.8,&dc,m_nStartX,m_nStartY,Width,Height,SRCCOPY);
//
//
//	tdc.SelectObject(pOld);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	for (int i=1;;i++)
//	{
//		fileName.Format("pict%04d.jpg",i);
//		int  Flag=0;
//		for (int j=0;j<p_UsrPicture.GetCount();j++)
//		{
//			if (fileName==p_UsrPicture[j].fileName)
//			{
//				Flag=1;
//				break;
//			}
//		}
//		if(Flag==0) break;
//		if(i>100) return;
//	}
//	UsrPicture	temp;                      //临时结构
//	ProSolid	solid;
//	ProMdlCurrentGet((ProMdl*)&solid);
//	temp.owner=solid;
//
//	temp.fileName=fileName;
//	temp.isCheck=1;
//	temp.filePath=csPath;
//	temp.Note="";
//	temp.isMain=0;                  //用来申明是否为主要简图，即可以看到整个模型
//	p_UsrPicture.Add(temp);
//	int indexNo=0;
//	indexNo=m_List.InsertItem(m_List.GetItemCount(),fileName);
//	m_List.SetItemText(indexNo,1,"");
//    fileName=csPath+fileName;
//
//    SaveBitmapAsJpeg(&tdc,bm,m_slidercrtrl.GetPos(),_T(fileName));
	//HINSTANCE hInstance;
	//typedef UINT (STR_FUNC)(CDC *,CBitmap &,int,CString );
	////CDC * dc/*tdc*/,CBitmap & bitmap,int nQuality,CString filename
	//STR_FUNC * pFunction;
	////需要修改
	//VERIFY(hInstance=::LoadLibrary("E:\\网络编程\\Main\\SaveAsJpeg.dll"));
	//VERIFY(pFunction=(STR_FUNC*)::GetProcAddress(hInstance,"SaveBitmapAsJpeg"));
	//(*pFunction)(&tdc,bm,100,_T(fileName));
 //   VERIFY(FreeLibrary(hInstance));
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//BITMAP btm;
	//bm.GetBitmap(&btm);
	//DWORD size=btm.bmWidthBytes*btm.bmHeight;
	//LPSTR lpData=(LPSTR)GlobalAlloc(GPTR,size);
	///////////////////////////////////////////////
	//BITMAPINFOHEADER bih;
	//bih.biBitCount=btm.bmBitsPixel;
	//bih.biClrImportant=0;
	//bih.biClrUsed=0;
	//bih.biCompression=0;
	//bih.biHeight=btm.bmHeight;
	//bih.biPlanes=1;
	//bih.biSize=sizeof(BITMAPINFOHEADER);
	//bih.biSizeImage=size;
	//bih.biWidth=btm.bmWidth;
	//bih.biXPelsPerMeter=0;
	//bih.biYPelsPerMeter=0;

	//	///////////////////////////////////
	//GetDIBits(dc,bm,0,bih.biHeight,lpData,(BITMAPINFO*)&bih,DIB_RGB_COLORS);

	////	bm.GetBitmapBits(size,lpData);	//此函数在处理5-5-5模式的16位色下会出现颜色混乱
	////////////////////////////////
	////static int filecount=0;
	//int num=(int)p_UsrPicture.GetCount();
	//UsrPicture temp;                      //临时结构
	//ProSolid solid;
	//ProMdlCurrentGet((ProMdl*)&solid);
	//temp.owner=solid;
	//
	//fileName.Format("pict%04d.bmp",num+1);           //定义文件名
 //    ////
	////CString fileJPG;
	////fileJPG.Format("pict%04d.jpg",num+1);
 //   /////
	//temp.fileName=fileName;
	//temp.isCheck=1;
 //   temp.filePath=csPath;
	//temp.Note="";
	//temp.isMain=0;                  //用来申明是否为主要简图，即可以看到整个模型
	//p_UsrPicture.Add(temp);
	//int indexNo=0;
	//indexNo=m_List.InsertItem(m_List.GetItemCount(),fileName);
	//m_List.SetItemText(indexNo,1,"");

	//fileName=csPath+fileName;
	//////
 ///*   fileJPG=csPath+fileJPG;*/

	//BITMAPFILEHEADER bfh;
	//bfh.bfReserved1=bfh.bfReserved2=0;
	//bfh.bfType=((WORD)('M'<< 8)|'B');
	//bfh.bfSize=54+size;
	//bfh.bfOffBits=54;
	//CFile bf;
	//if(bf.Open(fileName,CFile::modeCreate|CFile::modeWrite)){

	//	bf.Write(&bfh,sizeof(BITMAPFILEHEADER));
	//	bf.Write(&bih,sizeof(BITMAPINFOHEADER));
	//	bf.Write(lpData,size);
	//	bf.Close();		
	//}
	//GlobalFree(lpData);
/////////////////////////////////////////////////////////////////////////////////////////////////////
 //   //把BMP转换成JPG
	//HINSTANCE hInst;
	//CHANGE *pFunc;
	////具体优待修改
	//VERIFY(hInst = ::LoadLibrary("G:\\PTC启动目录\\jpgdll.dll"));

	//VERIFY(pFunc = (CHANGE*)::GetProcAddress(hInst,"BmpToJpg"));
	//pFunc(fileName.GetBuffer(50),fileJPG.GetBuffer(50));
	//fileName.ReleaseBuffer();
	//fileJPG.ReleaseBuffer();
	//::FreeLibrary(hInst);
 //   DeleteFile(fileName);
////////////////////////////////////////////////////////////////////////////////////////////////

 //   //清楚选择框
	//CWindowDC pDC(this->GetOwner());
	//CPen NewPen(PS_SOLID,1,RGB(0,0,255));
	//CPen *OldPen=pDC.SelectObject(&NewPen);
	//pDC.SetROP2(R2_NOT);
	//pDC.MoveTo(m_nStartX,m_nStartY);	
	//pDC.LineTo(m_nEndX,m_nStartY);
	//pDC.LineTo(m_nEndX,m_nEndY);
	//pDC.LineTo(m_nStartX,m_nEndY);
	//pDC.LineTo(m_nStartX,m_nStartY);
	//pDC.SelectObject(OldPen);
	////所有值置为零
	//m_nStartX=0;
	//m_nStartY=0;
	//m_nEndX=0;
	//m_nEndY=0;

 //   m_CreatButton.EnableWindow(FALSE);
	
//}
/************************************************************************/
/* 鼠标键起来的时候，完成选取工作                                       */
/************************************************************************/
void CPicMakgeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!(nFlags&MK_SHIFT)) return;
	if(m_flag!=1) 	return;
	//ClientToScreen(&point); 
 //   m_nEndX=point.x;
	//m_nEndY=point.y;
	m_flag=0;
	m_nSetp=0;
	ReleaseCapture();
	m_CreatButton.EnableWindow(TRUE);
	m_SelectButton.EnableWindow(TRUE);
	CDialog::OnLButtonUp(nFlags, point);
}



int CPicMakgeDlg::FreshList(void)
{
	m_List.DeleteAllItems();
	for(int i=0;i<2;i++)
	{
		m_List.DeleteColumn(0);
	}
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"文件名",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"说明",LVCFMT_LEFT,100,-1);
	//把当前模型已有的图片都添加上去
	int num=(int)p_UsrPicture.GetCount();
	int indexNo=0;
	ProSolid solid;
	ProMdlCurrentGet((ProMdl*)&solid);
	for(int i=0;i<num;i++)
	{
		if(p_UsrPicture[i].owner!=solid) continue;
		if(p_UsrPicture[i].isCheck!=1) continue;        
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,m_List.GetItemCount(),p_UsrPicture[i].fileName,0,0,0,i);
		m_List.SetItemText(indexNo,1,p_UsrPicture[i].Note);
	}
	return 0;
}




void CPicMakgeDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//如果关闭之间有选择，但没有生成，则清楚框
	if(m_nStartX!=0)
	{
		CWindowDC pDC(this->GetOwner());
		CPen NewPen(PS_SOLID,1,RGB(0,0,255));
		CPen *OldPen=pDC.SelectObject(&NewPen);
		pDC.SetROP2(R2_NOTXORPEN);
		pDC.MoveTo(m_nStartX,m_nStartY);	
		pDC.LineTo(m_nEndX,m_nStartY);
		pDC.LineTo(m_nEndX,m_nEndY);
		pDC.LineTo(m_nStartX,m_nEndY);
		pDC.LineTo(m_nStartX,m_nStartY);
		pDC.SelectObject(OldPen);
	}

	int window_id;
	ProWindowCurrentGet(&window_id);
	ProWindowActivate(window_id);
	
	CDialog::OnClose();
}

/************************************************************************/
/* 采用自定义图像大小时，在移动鼠标过程中的动作                         */
/************************************************************************/
void CPicMakgeDlg::TypeDIY(CPoint * point, UINT nFlags)
{
	CWindowDC	pDC(this->GetOwner());
	CPen		NewPen(PS_SOLID,1,RGB(0,0,255));
	ClientToScreen(point);
	//1.  2.还有是已经进入点取状态,3.必需要有起始点,4.必须从左上角到又下脚
	if(!(nFlags&MK_SHIFT) || m_flag!=1 || m_nSetp!=1 || (m_nStartX>point->x||m_nStartY>point->y)) return;

	CPen *OldPen=pDC.SelectObject(&NewPen);
	//先把上一次的给消除
	pDC.SetROP2(R2_NOT);//设置线条模式
	if(m_nEndX!=0)
	{
		pDC.MoveTo(m_nStartX,m_nStartY);	
		pDC.LineTo(m_nEndX,m_nStartY);
		pDC.LineTo(m_nEndX,m_nEndY);
		pDC.LineTo(m_nStartX,m_nEndY);
		pDC.LineTo(m_nStartX,m_nStartY);
	}
	m_nEndX=point->x;
	m_nEndY=point->y;
	pDC.MoveTo(m_nStartX,m_nStartY);	
	pDC.LineTo(m_nEndX,m_nStartY);
	pDC.LineTo(m_nEndX,m_nEndY);
	pDC.LineTo(m_nStartX,m_nEndY);
	pDC.LineTo(m_nStartX,m_nStartY);

	pDC.SelectObject(OldPen);

	m_pic_width=abs(m_nStartX-m_nEndX);
	m_pic_height=abs(m_nStartY-m_nEndY);
	UpdateData(FALSE);
}
/************************************************************************/
/* 指定大小的框                                                         */
/************************************************************************/
void CPicMakgeDlg::TypeBiaoZhun(CPoint * point, UINT nFlags,int iWidth, int iHeight)
{
	CWindowDC	pDC(this->GetOwner());
	CPen		NewPen(PS_SOLID,1,RGB(0,0,255));
	ClientToScreen(point);

	if(!(nFlags&MK_SHIFT) || m_flag!=1 || m_nSetp!=1 ) return;
	
	CPen *OldPen=pDC.SelectObject(&NewPen);
	//先把上一次的给消除
	pDC.SetROP2(R2_NOT);//设置线条模式
	//如果是第一次画
	if(m_nEndX!=0)
	{
		pDC.MoveTo(m_nStartX,m_nStartY);	
		pDC.LineTo(m_nEndX,m_nStartY);
		pDC.LineTo(m_nEndX,m_nEndY);
		pDC.LineTo(m_nStartX,m_nEndY);
		pDC.LineTo(m_nStartX,m_nStartY);
	}
	m_nStartX=point->x-iWidth/2;
	m_nStartY=point->y-iHeight/2;
	m_nEndX=point->x+iWidth/2;
	m_nEndY=point->y+iHeight/2;
	pDC.MoveTo(m_nStartX,m_nStartY);	
	pDC.LineTo(m_nEndX,m_nStartY);
	pDC.LineTo(m_nEndX,m_nEndY);
	pDC.LineTo(m_nStartX,m_nEndY);
	pDC.LineTo(m_nStartX,m_nStartY);
	pDC.SelectObject(OldPen);

	m_pic_width=abs(m_nStartX-m_nEndX);
	m_pic_height=abs(m_nStartY-m_nEndY);
	UpdateData(FALSE);
}
//////////////////////////////////////////////////////////////////////////
//一下为鼠标右键的操作
void CPicMakgeDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此添加消息处理程序代码
	CPoint point=GetCurrentMessage()->pt;
	ProMdl  mdl;
	ProMdlCurrentGet(&mdl);
	if(WindowFromPoint(point)==&m_List)
	{
		CMenu mTreeRightMenu;
		VERIFY(mTreeRightMenu.LoadMenu(IDR_MENU1));
		CMenu * pPopup=mTreeRightMenu.GetSubMenu(5);
		ASSERT(pPopup!=NULL);
		if(GetMdlType(mdl)==PRO_MDL_DRAWING) 
			pPopup->EnableMenuItem(ID_12179,MF_GRAYED);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);

	}
}
void CPicMakgeDlg::OnPicEdit()
{
	// TODO: 在此添加命令处理程序代码
	int num=0;
	ProSolid solid;
	ProMdlCurrentGet((ProMdl*)&solid);
	num=(int)m_List.GetItemCount();
	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
		CPicEditBoxDlg dlg;
		dlg.m_Note=m_List.GetItemText(i,1);
		if(dlg.DoModal()==IDOK)
		{
			int num1=0;
			num1=(int)p_UsrPicture.GetCount();
			for(int j=0;j<num1;j++)
			{
				if(p_UsrPicture[j].owner!=(ProMdl)solid) continue;
				if(p_UsrPicture[j].isCheck!=1) continue;
				if(p_UsrPicture[j].fileName!=m_List.GetItemText(i,0)) continue;
				p_UsrPicture[j].Note=dlg.m_Note;
				FreshList();
			}

		}
	}
}
/************************************************************************/
/* 右键菜单中对图片进行标注                                             */
/************************************************************************/
void CPicMakgeDlg::OnBiaozhu()
{
	// TODO: 在此添加命令处理程序代码
	CPicEditDlg		dlg;
	int 			num=(int)m_List.GetItemCount();
	CString			file="";
	ProSolid		solid;
	ProMdlCurrentGet((ProMdl*)&solid);
	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;

		int num1=0;
		num1=(int)p_UsrPicture.GetCount();
		for(int j=0;j<num1;j++)
		{
			if(p_UsrPicture[j].owner!=solid) continue;  //当前模型
			if(p_UsrPicture[j].isCheck!=1) continue;            //没有被删除
			if(p_UsrPicture[j].fileName!=m_List.GetItemText(i,0)) continue;
			file=p_UsrPicture[j].filePath+p_UsrPicture[j].fileName;
			break;
		}
	}
	if (!file.IsEmpty())
	{
		dlg.SetFile(file);
		dlg.DoModal();
	}
}
void CPicMakgeDlg::OnPicView()
{
	// TODO: 在此添加命令处理程序代码
	CPicViewDlg		dlg;
	int				 num=0;
	CString			file;
	ProSolid		solid;
	ProMdlCurrentGet((ProMdl*)&solid);
	num=(int)m_List.GetItemCount();

	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
		int j=(int)m_List.GetItemData(i);
        file=p_UsrPicture[(int)m_List.GetItemData(i)].filePath+p_UsrPicture[(int)m_List.GetItemData(i)].fileName;
		break;
		//int num1=0;
		//num1=(int)p_UsrPicture.GetCount();
		//for(int j=0;j<num1;j++)
		//{
		//	if(p_UsrPicture[j].owner!=solid) continue;  //当前模型
		//	if(p_UsrPicture[j].isCheck!=1) continue;            //没有被删除
		//	if(p_UsrPicture[j].fileName!=m_List.GetItemText(i,0)) continue;
		//	file=p_UsrPicture[j].filePath+p_UsrPicture[j].fileName;
		//	break;
		//}
	}
	if (!file.IsEmpty())
	{
		dlg.SetFile(file);
		dlg.DoModal();
	}
}
void CPicMakgeDlg::OnDelPic()
{
	// TODO: 在此添加命令处理程序代码
	int       num=0;
	ProSolid  solid;
	int       mdlIndex=-1; 
	ProMdlCurrentGet((ProMdl*)&solid);
	num=(int)m_List.GetItemCount();
	for(int i=0;i<num;i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
		DeleteFile(_T(p_UsrPicture[(int)m_List.GetItemData(i)].filePath+p_UsrPicture[(int)m_List.GetItemData(i)].fileName));
		//判断该文件是不是主图片，如果是，则删除模型中的定义
		GetMdlProp(solid,&mdlIndex);
		if (mdlIndex>=0)
		{
			if(p_UsrPartPorp[mdlIndex].pic_path==CString(p_UsrPicture[(int)m_List.GetItemData(i)].filePath+p_UsrPicture[(int)m_List.GetItemData(i)].fileName))
			{
				p_UsrPartPorp[mdlIndex].pic_path.Empty();
			}
		}
		p_UsrPicture.RemoveAt((int)m_List.GetItemData(i));
		break;
	}
	FreshList();	  
}
/************************************************************************/
/* 访问工程图中的图片                                                   */
/************************************************************************/
void CPicMakgeDlg::OnViewPicOfDrawing()
{
	// TODO: 在此添加命令处理程序代码
	ProSolid						solid;
	CArray<ProDrawing,ProDrawing>	p_drawlist;
	int								indexNo;
	ProName							name;
	ProMdlCurrentGet((ProMdl*)&solid);	
	GetSolidDrawing(solid,&p_drawlist);
	if(list_dlg==NULL) {list_dlg=new CListDlg();list_dlg->m_strText="查看模型的工程图中的图片";list_dlg->Create(IDD_LIST_DLG,this);}	
	else
	{
		delete list_dlg;
		list_dlg=new CListDlg();list_dlg->m_strText="查看模型的工程图中的图片";list_dlg->Create(IDD_LIST_DLG,this);
	}
	list_dlg->m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	list_dlg->m_List.InsertColumn(0,"文件名",LVCFMT_LEFT,100,-1);
	list_dlg->m_List.InsertColumn(1,"说明",LVCFMT_LEFT,100,-1);
	list_dlg->m_List.InsertColumn(2,"工程图",LVCFMT_LEFT,100,-1);
	for (int i=0;i<p_drawlist.GetCount();i++)
	{
		for(int j=0;j<p_UsrPicture.GetCount();j++)
		{
			if(p_UsrPicture[j].owner!=(ProMdl)p_drawlist[i]) continue;
			if(p_UsrPicture[j].isCheck!=1)           continue;   
			ProMdlNameGet((ProMdl)p_drawlist[i],name);
			indexNo=list_dlg->m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,list_dlg->m_List.GetItemCount(),p_UsrPicture[j].fileName,0,0,0,j);
			list_dlg->m_List.SetItemText(indexNo,1,p_UsrPicture[j].Note);
			list_dlg->m_List.SetItemText(indexNo,2,CString(name)+".DRW");
		}
	}
	list_dlg->ShowWindow(SW_SHOW);

}
LRESULT CPicMakgeDlg::OnListDlgOk(WPARAM wParam,LPARAM lParam)
{
	int					itemdata=0;
	UsrPicture			temp;                      //临时结构
	ProSolid			solid;
	ProMdlCurrentGet((ProMdl*)&solid);
	CString				fileName;
	if(list_dlg==NULL) return S_OK;
	for (int i=0;i<(int)list_dlg->m_List.GetItemCount();i++)
	{
		if (ListView_GetCheckState(list_dlg->m_List.m_hWnd,i))
		{
			itemdata=(int)list_dlg->m_List.GetItemData(i);
			for (int j=1;;j++)
			{
				fileName.Format("pict%04d.jpg",j);
				int  Flag=0;
				for (int m=0;m<p_UsrPicture.GetCount();m++)
				{
					if (fileName==p_UsrPicture[m].fileName)
					{
						Flag=1;
						break;
					}
				}
				if(Flag==0) break;
				if(i>100) return false;
			}
			temp.owner=solid;
			temp.fileName=fileName;
			temp.isCheck=1;
			temp.filePath=csPath;
			temp.Note=p_UsrPicture[itemdata].Note;
			temp.isMain=0;							//用来申明是否为主要简图，即可以看到整个模型
			fileName=csPath+fileName;
			if (CopyFile(p_UsrPicture[itemdata].filePath+p_UsrPicture[itemdata].fileName,fileName,FALSE))
				p_UsrPicture.Add(temp);
		}
	}
	FreshList();
	return S_OK;
}
/************************************************************************/
/* 生成图片                                                             */
/************************************************************************/
bool CPicMakgeDlg::MakePic(CPoint * point, int iWidth, int iHeight)
{
	CDC dc;
	dc.CreateDC("DISPLAY",NULL,NULL,NULL);    //DISPLAY   代表的是屏幕
	//创建位图，
	CBitmap bm;
	//int Width=abs(m_nStartX-m_nEndX);      /*GetSystemMetrics(SM_CXSCREEN);*/    //截取的高度和宽度
	//int Height=abs(m_nStartY-m_nEndY);             /*GetSystemMetrics(SM_CYSCREEN);*/
	//使创建的DDB的格式与输出设备的结构紧密相关，设备兼容
	bm.CreateCompatibleBitmap(&dc,iWidth-5,iHeight-5);
	CDC tdc;
	tdc.CreateCompatibleDC(&dc);
	CBitmap*pOld=tdc.SelectObject(&bm);
	tdc.BitBlt(0,0,iWidth,iHeight,&dc,point->x,point->y,SRCCOPY);         //的6，7个参数是屏幕的起始坐标
	//tdc.StretchBlt(0,0,Width*0.8,Height*0.8,&dc,m_nStartX,m_nStartY,Width,Height,SRCCOPY);

	tdc.SelectObject(pOld);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int i=1;;i++)
	{
		fileName.Format("pict%04d.jpg",i);
		int  Flag=0;
		for (int j=0;j<p_UsrPicture.GetCount();j++)
		{
			if (fileName==p_UsrPicture[j].fileName)
			{
				Flag=1;
				break;
			}
		}
		if(Flag==0) break;
		if(i>100) return false;
	}
	UsrPicture	temp;                      //临时结构
	ProSolid	solid;
	ProMdlCurrentGet((ProMdl*)&solid);
	temp.owner=solid;
	temp.fileName=fileName;
	temp.isCheck=1;
	temp.filePath=csPath;
	temp.Note="";
	temp.isMain=0;                  //用来申明是否为主要简图，即可以看到整个模型
	fileName=csPath+fileName;
	if (!SaveBitmapAsJpeg(&tdc,bm,m_slidercrtrl.GetPos(),_T(fileName)))
	{
		return false;
	}
	int indexNo=0;
	indexNo=m_List.InsertItem(m_List.GetItemCount(),temp.fileName);
	m_List.SetItemText(indexNo,1,"");	
	if (GetMdlInfo(solid).pic_path.IsEmpty())
	{
		SetMdlInfo(solid)->pic_path=fileName;
		temp.isMain=1;
	}
	m_List.SetItemData(indexNo,p_UsrPicture.Add(temp));	
	//HINSTANCE hInstance;
	//typedef UINT (STR_FUNC)(CDC *,CBitmap &,int,CString );
	////CDC * dc/*tdc*/,CBitmap & bitmap,int nQuality,CString filename
	//STR_FUNC * pFunction;
	////需要修改
	//VERIFY(hInstance=::LoadLibrary("E:\\网络编程\\Main\\SaveAsJpeg.dll"));
	//VERIFY(pFunction=(STR_FUNC*)::GetProcAddress(hInstance,"SaveBitmapAsJpeg"));
	//(*pFunction)(&tdc,bm,100,_T(fileName));
	//   VERIFY(FreeLibrary(hInstance));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//BITMAP btm;
	//bm.GetBitmap(&btm);
	//DWORD size=btm.bmWidthBytes*btm.bmHeight;
	//LPSTR lpData=(LPSTR)GlobalAlloc(GPTR,size);
	///////////////////////////////////////////////
	//BITMAPINFOHEADER bih;
	//bih.biBitCount=btm.bmBitsPixel;
	//bih.biClrImportant=0;
	//bih.biClrUsed=0;
	//bih.biCompression=0;
	//bih.biHeight=btm.bmHeight;
	//bih.biPlanes=1;
	//bih.biSize=sizeof(BITMAPINFOHEADER);
	//bih.biSizeImage=size;
	//bih.biWidth=btm.bmWidth;
	//bih.biXPelsPerMeter=0;
	//bih.biYPelsPerMeter=0;

	//	///////////////////////////////////
	//GetDIBits(dc,bm,0,bih.biHeight,lpData,(BITMAPINFO*)&bih,DIB_RGB_COLORS);

	////	bm.GetBitmapBits(size,lpData);	//此函数在处理5-5-5模式的16位色下会出现颜色混乱
	////////////////////////////////
	////static int filecount=0;
	//int num=(int)p_UsrPicture.GetCount();
	//UsrPicture temp;                      //临时结构
	//ProSolid solid;
	//ProMdlCurrentGet((ProMdl*)&solid);
	//temp.owner=solid;
	//
	//fileName.Format("pict%04d.bmp",num+1);           //定义文件名
	//    ////
	////CString fileJPG;
	////fileJPG.Format("pict%04d.jpg",num+1);
	//   /////
	//temp.fileName=fileName;
	//temp.isCheck=1;
	//   temp.filePath=csPath;
	//temp.Note="";
	//temp.isMain=0;                  //用来申明是否为主要简图，即可以看到整个模型
	//p_UsrPicture.Add(temp);
	//int indexNo=0;
	//indexNo=m_List.InsertItem(m_List.GetItemCount(),fileName);
	//m_List.SetItemText(indexNo,1,"");

	//fileName=csPath+fileName;
	//////
	///*   fileJPG=csPath+fileJPG;*/

	//BITMAPFILEHEADER bfh;
	//bfh.bfReserved1=bfh.bfReserved2=0;
	//bfh.bfType=((WORD)('M'<< 8)|'B');
	//bfh.bfSize=54+size;
	//bfh.bfOffBits=54;
	//CFile bf;
	//if(bf.Open(fileName,CFile::modeCreate|CFile::modeWrite)){

	//	bf.Write(&bfh,sizeof(BITMAPFILEHEADER));
	//	bf.Write(&bih,sizeof(BITMAPINFOHEADER));
	//	bf.Write(lpData,size);
	//	bf.Close();		
	//}
	//GlobalFree(lpData);
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//   //把BMP转换成JPG
	//HINSTANCE hInst;
	//CHANGE *pFunc;
	////具体优待修改
	//VERIFY(hInst = ::LoadLibrary("G:\\PTC启动目录\\jpgdll.dll"));

	//VERIFY(pFunc = (CHANGE*)::GetProcAddress(hInst,"BmpToJpg"));
	//pFunc(fileName.GetBuffer(50),fileJPG.GetBuffer(50));
	//fileName.ReleaseBuffer();
	//fileJPG.ReleaseBuffer();
	//::FreeLibrary(hInst);
	//   DeleteFile(fileName);
	////////////////////////////////////////////////////////////////////////////////////////////////

	//清楚选择框
	//CWindowDC pDC(this->GetOwner());
	//CPen NewPen(PS_SOLID,1,RGB(0,0,255));
	//CPen *OldPen=pDC.SelectObject(&NewPen);
	//pDC.SetROP2(R2_NOT);
	//pDC.MoveTo(m_nStartX,m_nStartY);	
	//pDC.LineTo(m_nEndX,m_nStartY);
	//pDC.LineTo(m_nEndX,m_nEndY);
	//pDC.LineTo(m_nStartX,m_nEndY);
	//pDC.LineTo(m_nStartX,m_nStartY);
	//pDC.SelectObject(OldPen);
	////所有值置为零
	//m_nStartX=0;
	//m_nStartY=0;
	//m_nEndX=0;
	//m_nEndY=0;

	//m_CreatButton.EnableWindow(FALSE);
	return true;
}

