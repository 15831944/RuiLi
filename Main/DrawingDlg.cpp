// DrawingDlg.cpp : 实现文件
//设置模型的工程图

#include "stdafx.h"
#include "Main.h"
#include "DrawingDlg.h"
#include "NewDlg.h"
#include "2DrawingDlg.h"
//获得与制定模型有关的工程图
//void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);
//extern CArray<UsrDrawingItem,UsrDrawingItem> P_UsrDrawingList;
// CDrawingDlg 对话框

IMPLEMENT_DYNAMIC(CDrawingDlg, CDialog)
CDrawingDlg::CDrawingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawingDlg::IDD, pParent)
	,dlg(NULL)
	,solid(NULL)
	,select_draw(NULL)
{
}

CDrawingDlg::~CDrawingDlg()
{
	if (dlg!=NULL)
	{
		delete dlg;
		dlg=NULL;
	}
}

void CDrawingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_SessionButton);
	DDX_Control(pDX, IDC_EDIT2, m_DrawFileName);
	DDX_Control(pDX, IDC_EDIT1, m_DrawPath);
	DDX_Control(pDX, IDC_BUTTON4, m_ShowButton);
	DDX_Control(pDX, IDC_BUTTON2, m_DelButton);
}


BEGIN_MESSAGE_MAP(CDrawingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_MESSAGE(WM_LIST_OK,OnListDlgOk)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CDrawingDlg 消息处理程序

BOOL CDrawingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化	
	if(ProMdlCurrentGet((ProMdl*)&solid)!=PRO_TK_NO_ERROR) 
	{
		m_SessionButton.EnableWindow(FALSE);
		return FALSE;
	}

	if (GetMdlInfo(solid).bHaveDraw)
	{
		select_draw=(ProDrawing)GetMdlInfo(solid).hDraw;
	}
	SetButtonAndText();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CDrawingDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(dlg==NULL) 
	{
		dlg=new CListDlg();
	    dlg->m_strText="访问进程中的工程图";
	    dlg->Create(IDD_LIST_DLG,this);
	}	
	dlg->m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	dlg->m_List.DeleteAllItems();
	do 	{}  while(dlg->m_List.DeleteColumn(0)!=0);
	dlg->m_List.InsertColumn(0,"名称",LVCFMT_LEFT,100,-1);
	dlg->m_List.InsertColumn(1,"模型",LVCFMT_LEFT,100,-1);
	dlg->m_List.InsertColumn(2,"位置",LVCFMT_LEFT,200,-1);	
	p_DrawingList.RemoveAll();
	p_DrawingList.FreeExtra();
	GetSolidDrawing(solid,&p_DrawingList);
	for (int i=0;i<(int)p_DrawingList.GetCount();i++)
	{
		ProMdldata date;
		int        indexNo;
		CString    str;
		if(ProMdlDataGet(p_DrawingList[i],&date)!=PRO_TK_NO_ERROR)
		{
			p_DrawingList.RemoveAt(i);
			i=i-1;
			continue;
		}
		GetDrawingSolidName(p_DrawingList[i],&str);
		indexNo=dlg->m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,dlg->m_List.GetItemCount(),CString(date.name)+".drw",0,0,0,i);
		dlg->m_List.SetItemText(indexNo,1,str);		
		dlg->m_List.SetItemText(indexNo,2,_T(CString(date.device) +":"+ CString(date.path)));
		
	} 
	dlg->ShowWindow(SW_SHOW);
}
LRESULT CDrawingDlg::OnListDlgOk(WPARAM wParam,LPARAM lParam)
{
	POSITION   pos=dlg->m_List.GetFirstSelectedItemPosition();
	int        nItem=dlg->m_List.GetNextSelectedItem(pos);
	int        i=0;
	UsrPartPorp * tempProp=GetMdlPInfo(solid);
	if (nItem>=0)
	{
		tempProp->bHaveDraw=true;
		i=(int)dlg->m_List.GetItemData(nItem);
		tempProp->hDraw=p_DrawingList[i];
		select_draw=p_DrawingList[i];		
	}
	else
	{
		tempProp->bHaveDraw=false;
		tempProp->hDraw=NULL;
		select_draw=NULL;
	}/*
	delete dlg;
	dlg=NULL;*/
	SetButtonAndText();
	return 1;
}
/************************************************************************/
/* 获得制定工程句柄中模型的名称                                         */
/************************************************************************/
void CDrawingDlg::GetDrawingSolidName(ProDrawing hDraw, CString * str)
{
	ProSolid     *solids;
	int          solidnum=0;
	ProName      name;
	ProMdlType   type;
	ProDrawingSolidsCollect(hDraw,&solids);
	ProArraySizeGet((ProArray)solids,&solidnum);
	for (int j=0;j<solidnum;j++)
	{
		CString temp;
		ProMdlNameGet(solids[j],name);
		ProMdlTypeGet(solids[j],&type);
		temp.Format("%s.%s;",CString(name),type==PRO_PART?"prt":"asm");
		str->Append(temp);
	}
}
/************************************************************************/
/* 浏览硬盘中的工程图文件                                               */
/************************************************************************/
void CDrawingDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString  ext1;
	ProPath  name;
	ProMdl   mdl;
	ProName  w_name;
	ProType  type;
	ProError status;
	int      dot;
	int      w_id;
	CString  path;
	CFileDialog dlg(TRUE,"drw","*.drw*"
		,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST
		,"PRO/ENGINEET 文件(*.drw)|*.drw*||");
	if (dlg.DoModal()==IDOK) {
		dot=dlg.GetPathName().Find('.',dlg.GetPathName().GetLength()-2);
		if (dot!=-1) {
			path=dlg.GetPathName().Left(dlg.GetPathName().GetLength()-2);
		}
		else
		{
			dot=dlg.GetPathName().Find('.',dlg.GetPathName().GetLength()-3);
			if (dot!=-1) {
				path=dlg.GetPathName().Left(dlg.GetPathName().GetLength()-3);
			}
			else path=dlg.GetPathName();

		}
		ProStringToWstring(name,_T(path.GetBuffer()+'\0'));
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,&mdl); 
		if (status!=PRO_TK_NO_ERROR)
		{
			AfxMessageBox("打开文件失败");
			return;
		}
		status = ProMdlTypeGet(mdl, (ProMdlType*)&type);
		status= ProMdlNameGet(mdl, w_name);
		status= ProObjectwindowCreate(w_name, type, &w_id);
		if(!status== PRO_TK_NO_ERROR)
		{
			return;
		}
		status = ProMdlDisplay(mdl);
		status = ProWindowActivate(w_id);
		OnOK();
	}
	else
	{
		if(ProWindowCurrentGet(&w_id)==PRO_TK_NO_ERROR){

			ProWindowActivate(w_id);
		}
	}	
}
/************************************************************************/
/* 打开选中的工程图                                                     */
/************************************************************************/
void CDrawingDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	ProError status;
	int      window_id;
	if (select_draw==NULL)
	    return;
	if(ProMdlWindowGet(select_draw,&window_id)!=PRO_TK_NO_ERROR)
	{
		ProMdldata data;
		status = ProMdlDataGet(select_draw,&data);
		status = ProObjectwindowCreate(data.name, PRO_DRAWING ,&window_id);
	}
	ProWindowActivate(window_id);
	status = ProMdlDisplay(select_draw);
	OnCancel();
}
/************************************************************************/
/* 删除选中的工程图                                                     */
/************************************************************************/
void CDrawingDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
    select_draw=NULL;
	SetMdlInfo(solid)->bHaveDraw=false;
	SetMdlInfo(solid)->hDraw=NULL;
	SetButtonAndText();
}

void CDrawingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
/************************************************************************/
/* 建立新的工程图                                                       */
/************************************************************************/
void CDrawingDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//CNewDlg dlg;
	//if(dlg.DoModal()==IDOK) OnOK();
	C2DrawingDlg dlg;
	if(dlg.DoModal()==IDCANCEL) OnCancel();
}

void CDrawingDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    OnOK();    
}
void CDrawingDlg::SetButtonAndText()
{
	if (select_draw==NULL)
	{
		m_DrawPath.SetWindowText("");
		m_DrawFileName.SetWindowText("");
		m_ShowButton.EnableWindow(FALSE);
		m_DelButton.EnableWindow(FALSE);
	}
	else
	{
		m_DrawFileName.SetWindowText(GetMdlFileName(select_draw));
		m_DrawPath.SetWindowText(GetMdlPath(select_draw));
		m_ShowButton.EnableWindow(TRUE);
		m_DelButton.EnableWindow(TRUE);
	}
}
void CDrawingDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}

void CDrawingDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此添加消息处理程序代码
	UsrRepaintWindow();
}
