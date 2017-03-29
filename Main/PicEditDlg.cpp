// PicEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "PicEditDlg.h"
#include "Math.h"
extern "C" __declspec(dllimport) BOOL SaveBitmapAsJpeg(CDC * dc/*tdc*/,CBitmap & bitmap,int nQuality,CString filename);
// CPicEditDlg 对话框

IMPLEMENT_DYNAMIC(CPicEditDlg, CDialog)
CPicEditDlg::CPicEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicEditDlg::IDD, pParent)
	, m_FullFileName(_T("")),pIpicture(NULL),m_ClickNum(0),mode(PTOL_UNUSE),m_lineColor(RGB(255,255,128)),m_lineWidth(1),m_textHeight(16),m_textWidth(7)
{
}

CPicEditDlg::~CPicEditDlg()
{
}

void CPicEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC_CONTROL, m_pic);
	DDX_Control(pDX, IDC_COMBO1, m_comb1);
	DDX_Control(pDX, IDC_COMBO2, m_comb2);
	DDX_Control(pDX, IDC_EDIT2, m_edit);
	DDX_Control(pDX, IDC_COMBO3, m_comLineWidth);
}


BEGIN_MESSAGE_MAP(CPicEditDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
ON_CBN_SELCHANGE(IDC_COMBO3, OnCbnSelchangeCombo3)
ON_EN_SETFOCUS(IDC_EDIT2, OnEnSetfocusEdit2)
ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
ON_WM_MOVE()
END_MESSAGE_MAP()


// CPicEditDlg 消息处理程序

bool CPicEditDlg::SetFile(CString strFullFileName)
{
	//下面的方法是图片失真太严重
	//m_FullFileName=strFullFileName;
	//if (!PathFileExists(m_FullFileName.GetBuffer()))
	//{
	//	AfxMessageBox("图片文件不存在");
	//	CDialog::OnOK();
	//	return false;
	//}
	//IStream *pStm=NULL;
	//CFile   f;
	//CFileStatus fileStatus;
	//CFileException e;	
	//if (!f.Open(m_FullFileName,CFile::modeRead/*|CFile::typeBinary*/,&e))
	//{
	//	AfxMessageBox("图片文件不存在");
	//	CDialog::OnOK();
	//	return false;
	//}
	//
	//f.GetStatus(fileStatus);
	//HGLOBAL		hGlobal=GlobalAlloc(GMEM_MOVEABLE,(SIZE_T)fileStatus.m_size);
	//LPSTR		pBuf=(LPSTR)GlobalLock(hGlobal);//(LPSTR)GlobalAlloc(GPTR,fileStatus.m_size);
	//f.Read(pBuf,(UINT)fileStatus.m_size);
	//CreateStreamOnHGlobal(pBuf,TRUE,&pStm);
	//if (!SUCCEEDED(OleLoadPicture(pStm,(LONG)fileStatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pIpicture)))
	//{
	//	pIpicture=NULL;
	//}
	//GlobalUnlock(hGlobal);
	//f.Close();

	m_FullFileName=strFullFileName;
	if (!PathFileExists(m_FullFileName.GetBuffer()))
	{
		AfxMessageBox("图片文件不存在");
		CDialog::OnOK();
		return false;
	}

	CFile   file;
	if(!file.Open(m_FullFileName, CFile::modeRead))
		return false;

	DWORD dwSize = (DWORD)file.GetLength();

	HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
	if(hMem == NULL) return  false;

	LPVOID pDes = ::GlobalLock(hMem);
	if(pDes == NULL){
		::GlobalFree(hMem);
		return false;
	}

	file.Read(pDes, dwSize);

	file.Close();

	GlobalUnlock(hMem);

	IStream* pStm = NULL;
	CreateStreamOnHGlobal(hMem, TRUE, &pStm);

	if(!SUCCEEDED(OleLoadPicture(pStm,dwSize,TRUE,IID_IPicture,(LPVOID*)&pIpicture)))
	{
		pStm -> Release();
		::GlobalFree(hMem);
		pStm = NULL;
		return false;
	}

	pStm->Release();
	::GlobalFree(hMem);
	return false;
}

void CPicEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	if (pIpicture!=NULL)
	{
		CSize m_size;
		CRect rect;
		m_pic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		pIpicture->get_Width(&m_size.cx);
		pIpicture->get_Height(&m_size.cy);
		dc.HIMETRICtoDP(&m_size);
		if (rect.Width()>m_size.cx)
		{
			rect.left=rect.left-(m_size.cx-rect.Width())/2;
			rect.right=rect.left+m_size.cx;
		}
		if (rect.Height()>m_size.cy)
		{
			rect.top=rect.top-(m_size.cy-rect.Height())/2;
			rect.bottom=rect.top+m_size.cy;
		}
		dc.DPtoHIMETRIC(&m_size);
		m_pic.MoveWindow(&rect,TRUE);
		pIpicture->Render(*GetDC(),rect.left,rect.bottom,rect.Width(),-rect.Height(),0,0,m_size.cx,m_size.cy,NULL);
	}
	for (int i=0;i<p_LineItemList.GetCount();i++)
	{
		switch(p_LineItemList[i].type) {
		case PTOL_DBL:
			ShowDBL_TEMP(&p_LineItemList[i]);
			break;
		case PTOL_PTL:
			ShowPTL_TEMP(&p_LineItemList[i]);
			break;
		case PTOL_TEXT:
			ShowText_TEMP(&p_LineItemList[i]);
			break;
		default:
			break;
		}
	}
	//确保属于文本编辑状态下时，文本框显示
	if (mode==PTOL_TEXT)
	{
		CRect rect;
		//m_pic.GetClientRect(&rect);
		//m_pic.ClientToScreen(&rect);
		//ScreenToClient(&rect);
		//RedrawWindow(&rect,NULL,RDW_ERASE|RDW_INVALIDATE);
        m_edit.GetWindowRect(&rect);
		ScreenToClient(&rect);
		RedrawWindow(&rect,NULL,RDW_ERASE);
		//m_edit.ShowWindow(SW_SHOW);
		m_edit.SetFocus();
	}
}

BOOL CPicEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	InitParas();
	if(m_FullFileName.IsEmpty())
	{
		AfxMessageBox("请先选择要编辑的图片");
		OnOK();
		return FALSE;
	}
	return TRUE;								// return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CPicEditDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	//未改变前的大小
	CRect  rect;
	GetWindowRect(&rect);
	CDialog::OnSizing(fwSide, pRect);
	CRect rect1;
	m_pic.GetWindowRect(&rect1);
	ScreenToClient(rect1);
	//保持控件与对话框的边距不变
	rect1.top=rect1.top+(pRect->top-rect.top);
	rect1.left=rect1.left+(pRect->left-rect.left);
	rect1.bottom=rect1.bottom+(pRect->bottom-rect.bottom);
	rect1.right=rect1.right+(pRect->right-rect.right);
    m_pic.MoveWindow(&rect1,TRUE);
	// TODO: 在此添加消息处理程序代码
}
/************************************************************************/
/* 添加标注                                                             */
/************************************************************************/
void CPicEditDlg::OnBnClickedAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ClickNum=0;
	pt_temp1=CPoint(0,0);
	pt_temp2=CPoint(0,0);
	switch(m_comb1.GetCurSel()) {
	case 0:
		mode=PTOL_DBL;
		temp_item.type=PTOL_DBL;
		break;
	case 1:
		mode=PTOL_TEXT;
		temp_item.type=PTOL_TEXT;
		break;
	case 2:
		mode=PTOL_PTL;
		temp_item.type=PTOL_PTL;
		break;
	default:
		MessageBox("请先选择所要添加的类型！","提示",MB_OK|MB_ICONINFORMATION);
		break;
	}
}

/************************************************************************/
/* 点击左键                                                             */
/************************************************************************/
void CPicEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值]
	switch(mode) {
	case PTOL_DBL:
		ShowDBL(&point);
		break;
	case PTOL_TEXT:
		ShowText(&point);
		break;
	case PTOL_PTL:
		ShowPTL(&point);
		break;
	default:
		break;
	}
	CDialog::OnLButtonDown(nFlags, point);
}
void CPicEditDlg::ShowDBL(CPoint * point)
{
	switch(m_ClickNum) {
	case 0:                  //点击第一点
		temp_item.pt_first=*point;
		m_ClickNum++;
		break;
	case 1:                  //点击第二点
		m_ClickNum++;
		temp_item.pt_second=*point;
		ShowDBL_TEMP(&temp_item);
	    temp_item.type=PTOL_DBL;
		p_LineItemList.Add(temp_item);
		InitParas();
		break;
	default:
		break;
	}
}
void CPicEditDlg::ShowPTL(CPoint * point)
{
	switch(m_ClickNum) {
	case 0:
		temp_item.pt_first=*point;
		m_ClickNum++;
		break;
	case 1:
		m_ClickNum++;
		temp_item.pt_second=*point;
		ShowPTL_TEMP(&temp_item);
		temp_item.type=PTOL_PTL;
		p_LineItemList.Add(temp_item);
		InitParas();
		break;
	default:
		break;
	}
}
void CPicEditDlg::ShowText(CPoint * point)
{
	CClientDC dc(this);
	switch(m_ClickNum) {
	case 0:
		temp_item.rect.top=point->y;
		temp_item.rect.left=point->x;
		temp_item.rect.bottom=point->y+m_textHeight+2;
		m_ClickNum++;
		break;
	case 1:
		m_ClickNum++;
		temp_item.rect.right=point->x;
		break;
	case 2:
		m_edit.GetWindowText(temp_item.text);
		//空的文本不添加
		if(temp_item.text.IsEmpty()) 
		{
			InitParas();
			return;
		}
		//如果只有一行的话，则宽度就是文本的长度
		temp_item.text.Trim();
		temp_item.text.MakeUpper();
		temp_item.rect.right=(m_edit.GetLineCount()==1?(temp_item.rect.left+temp_item.text.GetLength()*m_textWidth):(temp_item.rect.right));
		//高度为文本的高度与函数的乘积
		temp_item.rect.bottom=temp_item.rect.top+m_edit.GetLineCount()*m_textHeight;
		temp_item.type=PTOL_TEXT;
		p_LineItemList.Add(temp_item);
		InitParas();
	default:
		break;
	}
}
/************************************************************************/
/* 在鼠标移动过程中的动作                                               */
/************************************************************************/
void CPicEditDlg::ShowDBL_OnMouseMove(CPoint * point)
{
	//static CPoint pt_former=CPoint(0,0);
	//如果是第一次移动
	if (pt_temp1==CPoint(0,0))
	{
		pt_temp1=*point;
		temp_item.pt_second=*point;
		ShowDBL_TEMP(&temp_item);
	}
	else
	{
		temp_item.pt_second=pt_temp1;
		ShowDBL_TEMP(&temp_item);
		pt_temp1=*point;
		temp_item.pt_second=*point;
		ShowDBL_TEMP(&temp_item);
	}
}
void CPicEditDlg::ShowPTL_OnMouseMove(CPoint * point)
{
	if (pt_temp1==CPoint(0,0))
	{
		pt_temp1=*point;
		temp_item.pt_second=*point;
		ShowPTL_TEMP(&temp_item);
	}
	else
	{
		temp_item.pt_second=pt_temp1;
		ShowPTL_TEMP(&temp_item);
		pt_temp1=*point;
		temp_item.pt_second=*point;
		ShowPTL_TEMP(&temp_item);
	}
}
void CPicEditDlg::ShowText_OnMouseMove(CPoint * point)
{
	if (point->x < temp_item.rect.left)
	{
		return;
	}
	pt_temp1=*point;
	temp_item.rect.right=pt_temp1.x;
	m_edit.ShowWindow(SW_SHOW);
	m_edit.MoveWindow(&temp_item.rect,TRUE);
}
/************************************************************************/
/* 移动鼠标                                                             */
/************************************************************************/
void CPicEditDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//等待第二点的时候
	if (m_ClickNum==1)
	{
		switch(mode) {
	case PTOL_DBL:
		ShowDBL_OnMouseMove(&point);
		break;
	case PTOL_TEXT:
		ShowText_OnMouseMove(&point);
		break;
	case PTOL_PTL:
		ShowPTL_OnMouseMove(&point);
		break;
	default:
		break;
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}
/************************************************************************/
/* 在鼠标移动过程中进行绘制(橡皮擦)                                     */
/************************************************************************/
int CPicEditDlg::TempDraw(line_item * p_item)
{
	//CClientDC dc(this);
	//
	switch(mode) {
	case PTOL_DBL:
		break;
	case PTOL_PTL:
		break;
	case PTOL_TEXT:
		break;
	default:
		break;
	}
	return 0;
}
/************************************************************************/
/* 鼠标移动过程中画线                                                   */
/************************************************************************/
void CPicEditDlg::ShowDBL_TEMP(line_item *	p_LineItem)
{
	CClientDC dc(this);
	if(m_ClickNum==1) dc.SetROP2(R2_NOT);
	double	sin_x=0.0;
	double	cos_x=0.0;
	double	a=15;							//两头的长度
	int     b=m_lineWidth;                  //两头线的宽度
	int     b1=m_lineWidth;                 //中间线的宽度
	CPoint	pt1=p_LineItem->pt_first;
	CPoint	pt2=p_LineItem->pt_second;      //如果是等待第二点的话
	//CPoint	pt3=(m_ClickNum==0?p_LineItem->pt_third:pt_temp);      //如果是等待第三点的话
	CPoint	pt1_1;
	CPoint	pt1_2;
	CPoint	pt2_1;
	CPoint	pt2_2;
	double	c=sqrt(double((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y)));
	sin_x=(double(pt2.y-pt1.y))/c;
	cos_x=(double(pt2.x-pt1.x))/c;
	pt1_1.y=pt1.y-(LONG)(a*cos_x/2);
	pt1_2.y=pt1.y+(LONG)(a*cos_x/2);
	pt1_1.x=pt1.x+(LONG)(a*sin_x/2);
	pt1_2.x=pt1.x-(LONG)(a*sin_x/2);

	pt2_1.y=(pt2.y-pt1.y)+pt1_1.y;
	pt2_2.y=(pt2.y-pt1.y)+pt1_2.y;
	pt2_1.x=(pt2.x-pt1.x)+pt1_1.x;
	pt2_2.x=(pt2.x-pt1.x)+pt1_2.x;

	//画端线
	CPen NewPen1(PS_SOLID,b,m_lineColor);
	CPen * OldPen=dc.SelectObject(&NewPen1);
	dc.MoveTo(pt1_1);
	dc.LineTo(pt1_2);
	dc.MoveTo(pt2_1);
	dc.LineTo(pt2_2);

	//画中线
	CPen NewPen2(PS_SOLID,b1,m_lineColor);
	dc.SelectObject(&NewPen2);
	dc.MoveTo(pt1);
	dc.LineTo(pt2);

 	dc.SelectObject(OldPen);
	//显示文本框
}
/************************************************************************/
/* 画指引线                                                             */
/************************************************************************/
void CPicEditDlg::ShowPTL_TEMP(line_item *	p_LineItem)
{
	CClientDC dc(this);
	if(m_ClickNum == 1) dc.SetROP2(R2_NOT);
	int     a=4*m_lineWidth;                   //黑点的直径
	int     b=m_lineWidth;                   //线的宽度
	//画黑点
	CPen NewPen1(PS_SOLID,b,m_lineColor);
	CBrush brush(m_lineColor);
	CBrush * pOldBursh=dc.SelectObject(&brush);
	CPen * OldPen=dc.SelectObject(&NewPen1);
	dc.Ellipse(p_LineItem->pt_first.x-a/2,p_LineItem->pt_first.y-a/2
		,p_LineItem->pt_first.x+a/2,p_LineItem->pt_first.y+a/2);
	dc.SelectObject(pOldBursh);

	CPen NewPen2(PS_SOLID,b,m_lineColor);
	dc.SelectObject(&NewPen2);
	dc.MoveTo(p_LineItem->pt_first);
	dc.LineTo(p_LineItem->pt_second);
	dc.SelectObject(OldPen);
}
/************************************************************************/
/* 显示文本                                                             */
/************************************************************************/
void CPicEditDlg::ShowText_TEMP(line_item *	p_LineItem)
{
	CClientDC dc(this);
	//更新
	if(p_LineItem->type!=PTOL_TEXT) return;
	//InvalidateRect(p_LineItem->rect);
	if(!p_LineItem->text.IsEmpty())
	{
		CFont   NewFont;
		CBrush  brush(m_lineColor);
		CBrush * pOldBursh=dc.SelectObject(&brush);
		NewFont.CreateFont(m_textHeight,m_textWidth,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH&FF_SWISS,"Arial");
		CFont * pOldFont=dc.SelectObject(&NewFont);
		//dc.Rectangle(p_LineItem->rect);
		dc.TextOut(p_LineItem->rect.left,p_LineItem->rect.top,p_LineItem->text);
		dc.SelectObject(pOldBursh);
		dc.SelectObject(pOldFont);
	}		
}
/************************************************************************/
/* 显示文本框                                                           */
/************************************************************************/
int CPicEditDlg::EditTextBox(line_item * p_LineItem)
{
	CClientDC    dc(this);
	if(p_LineItem->type!=PTOL_TEXT) return  0;
	if (!p_LineItem->text.IsEmpty())
	{
		dc.Rectangle(p_LineItem->rect);	
		dc.TextOut(p_LineItem->rect.left+2,p_LineItem->rect.top+1,p_LineItem->text);
	}
	return 0;
}

void CPicEditDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
/************************************************************************/
/* 初始话所有辅助的参数                                                 */
/************************************************************************/
int CPicEditDlg::InitParas(void)
{
	mode=PTOL_UNUSE;
	m_ClickNum=0;
	temp_item.type=PTOL_UNUSE;
	temp_item.pt_first=CPoint(0,0);
	temp_item.pt_second=CPoint(0,0);
	temp_item.pt_third=CPoint(0,0);
	temp_item.rect=CRect(0,0,0,0);
	temp_item.text.Empty();
	pt_temp1=CPoint(0,0);
	m_comb1.SetCurSel(0);
	m_edit.ShowWindow(SW_HIDE);
	m_edit.SetWindowText("");
	ShowComb2();
	return 0;
}
/************************************************************************/
/* 设置颜色                                                             */
/************************************************************************/
void CPicEditDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog dlg(m_lineColor,0,NULL);
	if (dlg.DoModal()==IDOK )
	{
		m_lineColor=dlg.GetColor();
	}
}
/************************************************************************/
/* 刷新界面                                                             */
/************************************************************************/
void CPicEditDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessage(WM_PAINT,0,0);
}
/************************************************************************/
/* 保存                                                                 */
/************************************************************************/
void CPicEditDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CDC dc;
	dc.CreateDC("DISPLAY",NULL,NULL,NULL);    //DISPLAY   代表的是屏幕
	//创建位图，
	CBitmap bm;
	CRect rect;
	m_pic.GetWindowRect(&rect);
	int Width=rect.Width();                     /*GetSystemMetrics(SM_CXSCREEN);*/    //截取的高度和宽度
	int Height=rect.Height();                   /*GetSystemMetrics(SM_CYSCREEN);*/
	//使创建的DDB的格式与输出设备的结构紧密相关，设备兼容
	bm.CreateCompatibleBitmap(&dc,Width,Height);
	CDC tdc;
	tdc.CreateCompatibleDC(&dc);
	CBitmap*pOld=tdc.SelectObject(&bm);
	tdc.BitBlt(0,0,Width,Height,&dc,rect.left,rect.top,SRCCOPY);         //的6，7个参数是屏幕的起始坐标
	tdc.SelectObject(pOld);
	SaveBitmapAsJpeg(&tdc,bm,100,_T(m_FullFileName));
}

void CPicEditDlg::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_lineWidth=m_comLineWidth.GetCurSel()+1;
	SendMessage(WM_PAINT,0,0);
}

void CPicEditDlg::OnEnSetfocusEdit2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_edit.ShowWindow(SW_SHOW);
}

int CPicEditDlg::ShowComb2(void)
{
	do {		
	} while(m_comb2.DeleteString(0)!=-1);
	for (int i=0;i<p_LineItemList.GetCount();i++)
	{
		CString temp;
		switch(p_LineItemList[i].type) {
		case PTOL_TEXT:
			temp.Format("%d.%s",i,p_LineItemList[i].text);
			break;
		case PTOL_DBL:
			temp.Format("%d.端线",i);
			break;
		case PTOL_PTL:
			temp.Format("%d.指引线",i);
			break;
		default:
			break;
		}
		m_comb2.InsertString(i,temp);
		m_comb2.SetItemData(i,i);
		m_comb2.SetCurSel(0);
	}
	return 0;
}
/************************************************************************/
/* 删除选中的                                                           */
/************************************************************************/
void CPicEditDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comb2.GetCurSel()>=0)
	{
		p_LineItemList.RemoveAt(m_comb2.GetItemData(m_comb2.GetCurSel()));
		SendMessage(WM_PAINT,0,0);
		ShowComb2();
	}
}
/************************************************************************/
/* 选定列表框，高亮显示制定                                             */
/************************************************************************/
void CPicEditDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comb2.GetCurSel()>=0)
	{
		SendMessage(WM_PAINT,0,0);
		COLORREF temp=m_lineColor;
		int     temp1= m_lineWidth;
		int      temp2=m_textHeight;
		int      temp3=m_textWidth;
		m_lineWidth+=1;
		m_textHeight+=1;
		m_textWidth+=1;
		m_lineColor=RGB(146,253,130);		
		switch(p_LineItemList[m_comb2.GetItemData(m_comb2.GetCurSel())].type) {
		case PTOL_DBL:
			ShowDBL_TEMP(&p_LineItemList[m_comb2.GetItemData(m_comb2.GetCurSel())]);
			break;
		case PTOL_PTL:
			ShowPTL_TEMP(&p_LineItemList[m_comb2.GetItemData(m_comb2.GetCurSel())]);
			break;
		case PTOL_TEXT:
			ShowText_TEMP(&p_LineItemList[m_comb2.GetItemData(m_comb2.GetCurSel())]);
			break;
		default:
			break;
		}
		m_lineColor=temp;
		m_lineWidth=temp1;
		m_textHeight=temp2;
		m_textWidth=temp3;
	}
}

void CPicEditDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此添加消息处理程序代码
	UsrRepaintWindow();
}
