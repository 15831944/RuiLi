// PdtAlterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "PdtAlterDlg.h"
#include "Compute.h"
#include "MyDatabase.h"
#include "DetailInfoClass.h"
#include "FtpFile.h"
#include "CommonFuncClass.h"
#include "ProWindows.h"
#include "ProductClass.h"
#include "ProductMenberBasicInfoDlg.h"
#include "afxtempl.h"
#include "PrtClass.h"


void GetSolidDrawing(ProSolid solid,CArray<ProDrawing,ProDrawing> * p_DrawingList);

extern CPtrArray p_classarray;
extern CMyDatabase m_db;
extern CStringArray RelationSet;
extern CStringArray RelationSetTemp;
extern FtpInfo Ftp_struc;
extern CArray <JuBing,JuBing> JuBing_struc;
extern CArray <JuBing,JuBing> JuBing_struc_temp;
extern CArray <FilePath,FilePath> FilePath_struc;
extern CArray<Database,Database> Database_struc;
extern CArray<Database,Database> Database_struc_temp;
extern CArray<ProDrawing,ProDrawing>  DrawingList;
// CPdtAlterDlg 对话框

IMPLEMENT_DYNAMIC(CPdtAlterDlg, CDialog)
CPdtAlterDlg::CPdtAlterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPdtAlterDlg::IDD, pParent)
{
	p_Picture=NULL;
	picNum=0;
	MdlNum="0";
	Pos=-1;
}

CPdtAlterDlg::~CPdtAlterDlg()
{
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
	}
}

void CPdtAlterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
	DDX_Control(pDX, IDC_LIST_DIM, m_listdim);
}


BEGIN_MESSAGE_MAP(CPdtAlterDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PRE, OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedButtonNext)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_PAINT()
	//ON_WM_CLOSE()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CPdtAlterDlg 消息处理程序

void CPdtAlterDlg::OnBnClickedButtonPre()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowPic(atol(MdlNum),picNum-1);
}

void CPdtAlterDlg::OnBnClickedButtonNext()
{
	ShowPic(atol(MdlNum),picNum+1);
	// TODO: 在此添加控件通知处理程序代码
}
/************************************************************************/
/* 开始变型按钮                                                         */
/************************************************************************/
void CPdtAlterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CCommonFuncClass commonfun;
	CProductClass pdtclass;
	CCompute compute;
	CString str;
	for (int i=0;i<m_listdim.GetItemCount();i++)
	{
		Database_struc_temp[i].value=m_listdim.GetItemText(i,1);
		Database_struc_temp[i].up=m_listdim.GetItemText(i,2);
		Database_struc_temp[i].down=m_listdim.GetItemText(i,3);
	}
	//得到关系式和产品零件的变量
	Database_struc.RemoveAll();
	RelationSet.RemoveAll();
	//完成了读变量和读关系式
	if(!compute.GetAllVariantAndRelativeVariant(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid,MdlNum))
	{
		return ;
	}
	RelationSetTemp.RemoveAll();
	//临时的结构赋给正式的结构
	for (int i=0;i<Database_struc_temp.GetSize();i++)
	{
		for (int j=0;j<Database_struc.GetSize();j++)
		{
			if (Database_struc[j].PARADIMSYMBOL==Database_struc_temp[i].PARADIMSYMBOL)
			{
				Database_struc[j].value=Database_struc_temp[i].value;
				Database_struc[j].up=Database_struc_temp[i].up;
				Database_struc[j].down=Database_struc_temp[i].down;
				Database_struc[j].IsOk=TRUE;
			}
		}
	}
	//得到零件的主变量的值
	if (!compute.GetValueFromMdl())
	{
		return;
	}
	//重生模型
	for (int j=0;j<Database_struc.GetSize();j++)
	{
		if (Database_struc[j].IsOrNot)
		{
			compute.UpdateAllRelation(Database_struc[j].PARADIMSYMBOL,j);
		}
	}
	//判断是否已经全部计算完了
	for (int j=0;j<RelationSet.GetSize();j++)
	{
		if(compute.IsOnlyOne(j)!="结束")
		{
			AfxMessageBox("你建的关系式有误，无法为你计算！");
			return;
		}
	}
	if(!compute.AsmGeneration(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		str.Format("重生失败，是否仍旧新建产品！");
		if(AfxMessageBox(str,MB_YESNO)!=IDYES)
		{
			OnOK();
			return;
		}
	}
	OnOK();
	////判断哪些已经改了
    if(!pdtclass.FindProductMenberForProductWhichIsModify(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid,TRUE))
	{
		return;
	}
	if (!commonfun.SetProductPara(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		AfxMessageBox("设置参数错误");
		return;
	}
	if (!pdtclass.ActivateAsm(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		AfxMessageBox("打开窗口失败");
		return;
	}
	//commonfun.AddMdlInfo(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid);
	OnOK();
}

void CPdtAlterDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//显示模型
	CProductClass pdtclass;
	CCommonFuncClass commonfun;
    //向二维图添加图号等信息，但好象还没有实现功能
	//commonfun.SetParam(Pos);
	if (!commonfun.SetProductPara(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		AfxMessageBox("设置参数错误");
		return;
	}
	if (!pdtclass.ActivateAsm(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		AfxMessageBox("打开Pro/E窗口失败");
		return;
	}
	OnCancel();
}

void CPdtAlterDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此添加消息处理程序代码
	CRect rect;
	if (p_Picture!=NULL)
	{
		CSize m_size;

		m_pic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		p_Picture->get_Width(&m_size.cx);
		p_Picture->get_Height(&m_size.cy);
		m_pic.MoveWindow(&rect,TRUE);
		p_Picture->Render(*GetDC(),rect.left,rect.bottom,rect.Width(),-rect.Height(),0,0,m_size.cx,m_size.cy,NULL);
	}
	// 不为绘图消息调用 CDialog::OnPaint()
}

//void CPdtAlterDlg::OnClose()
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	OnCancel();
//}

BOOL CPdtAlterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CCommonFuncClass commonfun;
	//去掉进程中没有的基本信息
	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}

	// TODO:  在此添加额外的初始化
	ListView_SetExtendedListViewStyle(m_listdim.m_hWnd, LVS_EX_GRIDLINES);
	m_listdim.InsertColumn(0,"尺寸名",LVCFMT_LEFT,60,-1);
	m_listdim.InsertColumn(1,"尺寸值",LVCFMT_LEFT,80,-1);
	m_listdim.InsertColumn(2,"上偏差",LVCFMT_LEFT,80,-1);
	m_listdim.InsertColumn(3,"下偏差",LVCFMT_LEFT,80,-1);
	m_listdim.InsertColumn(4,"备注",LVCFMT_LEFT,80,-1);
	m_listdim.SetReadOnlyColumns(0,false);
	m_listdim.SetReadOnlyColumns(4,false);
	GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	//查询数据库
	CCompute compute;
	CString strSql;
	Database_struc_temp.RemoveAll();
	//得到装配体的变型尺寸信息到临时的结构中
	if(!compute.ObtainAsmFromBaseTemp(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid,MdlNum))
	{
		AfxMessageBox("无法得到该模型的可变尺寸");
		return FALSE;
	}
	//从当前模型文件中得到可变尺寸的数值信息。
	if(!compute.ObtainValueFromMdlTemp(((DetailInfoClass *)p_classarray.GetAt(Pos))->solid))
	{
		return FALSE;
	}
	//显示模型文件图片
	ShowPic(atol(MdlNum),1);
	int t=0;
	//显示变量尺寸在列表框中
	for(int i=0;i<Database_struc_temp.GetSize();i++)	
	{
		int indexNo=m_listdim.InsertItem(LVIF_TEXT|LVIF_PARAM,t,Database_struc_temp[i].name,
			0,0,0,Database_struc_temp[i].number);
		m_listdim.SetItemText(indexNo,1,Database_struc_temp[i].value);
		m_listdim.SetItemText(indexNo,2,Database_struc_temp[i].up);	 //上偏差
		m_listdim.SetItemText(indexNo,3,Database_struc_temp[i].down);	 //下偏差
		m_listdim.SetItemText(indexNo,4,Database_struc_temp[i].note);
		t++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 查找图片数据库                                                       */
/************************************************************************/
BOOL CPdtAlterDlg::ShowPic(int num,int Number)
{
	BYTE *pBuffer=NULL;
	CRect rect;
	m_pic.GetClientRect(&rect);
	CString strSql;
	int nSize=0;
	if (p_Picture!=NULL)
	{
		p_Picture->Release();
		p_Picture=NULL;
	}
	if (!m_db.DatabaseState())
	{
		m_db.ReOpen();
	}
	strSql.Format("SELECT * FROM NAsmPic WHERE AsmNum=%d AND Number=%d",num,Number);
	int Z=m_db.Query(strSql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		m_pic.ClientToScreen(&rect);
		this->ScreenToClient(&rect);
		RedrawWindow(&rect,NULL,RDW_INVALIDATE | RDW_ERASE);
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			nSize=m_db.getBinary("File",&pBuffer);
			picNum=m_db.getInt("Number");
			break;
		} while(m_db.MoveNext());
	}
	DBOpenPic(pBuffer,nSize,&p_Picture);
	OnPaint();
	//根据图片的位置显示对话框中下一个按钮和上一个按钮的的可操作性
	strSql.Format("SELECT * FROM NAsmPic WHERE AsmNum=%d AND Number=%d",num,picNum+1);
	if (m_db.Query(strSql)==0)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE); 
	}
	else
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
	strSql.Format("SELECT * FROM NAsmPic WHERE AsmNum=%d AND Number=%d",num,picNum-1);
	if (m_db.Query(strSql)==0)
	{
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE); 
	}
	else
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(TRUE);
	delete [] pBuffer;
	return TRUE;
}
/************************************************************************/
/* 把二进制的数据转化为图片                                             */
/************************************************************************/
BOOL CPdtAlterDlg::DBOpenPic(BYTE *pBuffer,int nSize,IPicture ** pic)
{			
	HGLOBAL hGlobal=::GlobalAlloc(GMEM_MOVEABLE,nSize);
	LPSTR buffer=(LPSTR)GlobalLock(hGlobal);
	memcpy(buffer,pBuffer,nSize);

	IStream *  pstm =NULL; // 需要一个流（stream）			
	CreateStreamOnHGlobal(buffer, TRUE, &pstm);
	if (!SUCCEEDED(OleLoadPicture(pstm,nSize,TRUE,IID_IPicture,(LPVOID*)pic)))
	{
		AfxMessageBox("无图片");
		return FALSE;
	}
	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal)	;
	pstm->Release();
	return TRUE;
}
void CPdtAlterDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	ProError status;

	int w_id;
	status=ProWindowCurrentGet(&w_id);
	status=ProWindowActivate(w_id);
	status=ProWindowRepaint(w_id);
	// TODO: 在此添加消息处理程序代码
}