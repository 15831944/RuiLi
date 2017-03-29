// NewFromMdlDlg.cpp : 实现文件
//从原有的模型中继承生成新的实例模型

#include "stdafx.h"
#include "Main.h"
#include "NewFromMdlDlg.h"
#include "partPerfparaTableSet.h"
#include "partVariantTableSet.h"
#include "partRelationSet.h"
#include "partPicSet.h"
#include "FtpFile.h"
#include "UsrSWTXTable.h"
//extern int DlgIcon;
//extern FtpConnectInfo myFtp;
//extern CArray<UsrSWTXTable1,UsrSWTXTable1>  p_UsrSWTXTableList;  //定义全局的事物特性表列项，
////在程序运行时，要给他一个初始值(无效)
//extern CArray<UsrRelTable,UsrRelTable>    p_UsrRelTableList;   //定义全局的关系列表项数组
//extern CArray<UsrPicture,UsrPicture>   p_UsrPicture;           //图片的全局变量                                                      
//extern CArray<UsrPerfParaTable,UsrPerfParaTable> p_UsrPerfParaTableList;//定义的性能参数表的全局变量
//extern CArray<UsrPartPorp,UsrPartPorp> p_UsrPartPorp;          //定义模型基本信息的全局变量
//
//// CWorkPlaceManDlg 对话框
//extern bool RemoveDireAndFile(CString localdir);
//BOOL ShowPartTree(CTreeCtrl * m_Tree,CDatabase * p_db);
//BOOL ShowAsmTree(CTreeCtrl * m_Tree,CDatabase * p_db);
// CNewFromMdlDlg 对话框

IMPLEMENT_DYNAMIC(CNewFromMdlDlg, CDialog)
CNewFromMdlDlg::CNewFromMdlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewFromMdlDlg::IDD, pParent)
	, m_selTab(0)
	, mdlSelectNumber(-1)
	, m_Name(_T(""))
{
}

CNewFromMdlDlg::~CNewFromMdlDlg()
{
}

void CNewFromMdlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_EDIT1, m_Path);
	//DDX_Control(pDX, IDC_EDIT2, m_Name);
	DDX_Control(pDX, IDC_TREE5, m_Tree1);
	DDX_Control(pDX, IDC_TREE6, m_Tree2);
	DDX_Text(pDX, IDC_EDIT2, m_Name);
}


BEGIN_MESSAGE_MAP(CNewFromMdlDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE5, OnTvnSelchangedTree5)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CNewFromMdlDlg 消息处理程序

BOOL CNewFromMdlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("零件模型");
	m_Tab.InsertItem(0, &tcItem);
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("产品/组件模型");
    m_Tab.InsertItem(1, &tcItem);
	m_Tree2.ShowWindow(SW_HIDE);
	m_selTab=0;
	//设置图片列表
	m_ImageList.Create(16, 16, ILC_COLOR, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON2));
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON3));
	m_Tree1.SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_Tree1.SetTextColor (RGB(7,145,13));
	ShowPartTree(&m_Tree1,m_pSet.m_pDatabase);
	ShowAsmTree(&m_Tree2,m_pSet.m_pDatabase);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"名称",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(1,"编码",LVCFMT_LEFT,100,-1);
	m_List.InsertColumn(2,"备注",LVCFMT_LEFT,200,-1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 切换                                                                 */
/************************************************************************/
void CNewFromMdlDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	int sel = m_Tab.GetCurSel();
	switch(sel)
	{
	case 0:
        if (m_selTab==0) return;
		m_Tree1.ShowWindow(SW_SHOW);
		m_Tree2.ShowWindow(SW_HIDE);
		m_selTab=0;
		break;
	case 1:
		if (m_selTab==1) return;
		m_Tree2.ShowWindow(SW_SHOW);
		m_Tree1.ShowWindow(SW_HIDE);
		m_selTab=1;
		break;
	}
	*pResult = 0;
}
void CNewFromMdlDlg::OnTvnSelchangedTree5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	long num=(long)pNMTreeView->itemNew.lParam;
	int  indexNo=0;
	CString csText;
	m_List.DeleteAllItems();
	try
	{
		if(m_pSet.IsOpen()) m_pSet.Close();
		m_pSet.m_strFilter.Format("[Number]=%d",num);
		m_pSet.Open();
		if (m_pSet.IsBOF()) {
			m_pSet.Close();
			return;
		}
		m_pSet.MoveFirst();
		csText=m_pSet.m_FileDirectory;
		//csText.Replace("零件目录","零件模型");
		csText=csText+"/";
		m_Path.SetWindowText(csText);
		mdlSelectNumber=num;
        m_pSet.Close();

		m_pSet.m_strFilter.Format("[FatherNumber]=%d",num);
		m_pSet.Open();
		if (m_pSet.IsBOF()) {
			m_pSet.Close();
			return;
		}
		m_pSet.MoveFirst();
		while (!m_pSet.IsEOF()) {
             m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,indexNo,m_pSet.m_Name,0,0,0,m_pSet.m_Number);
			 m_List.SetItemText(indexNo,1,m_pSet.m_MdlCode);
			 m_List.SetItemText(indexNo,2,m_pSet.m_Note);
			 indexNo++;
			 m_pSet.MoveNext();
		}
	}
	catch (...) {
		AfxMessageBox("出错");
	}
	if(m_pSet.IsOpen()) m_pSet.Close();    
	*pResult = 0;
}

void CNewFromMdlDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
			int num=0;
	switch(m_selTab) {
	case 0:
		num=(int)m_List.GetItemCount();
		for (int i=0;i<num;i++) {
			if(m_List.GetItemState(i,LVIS_SELECTED)!=LVIS_SELECTED) continue;
			m_pSet.m_strFilter.Format("[Number]=%d",(long)m_List.GetItemData(i));
			if (m_pSet.IsOpen()) m_pSet.Close();
			m_pSet.Open();
			if (!m_pSet.IsBOF()) {
				m_pSet.MoveFirst();
				CString csText=m_pSet.m_FileDirectory;
				//csText.Replace("零件目录","零件模型");
				csText=csText+"/";
				m_Path.SetWindowText(csText);
				mdlSelectNumber=(long)m_List.GetItemData(i);
			}
			m_pSet.Close();		
		}
		break;
	default:
		break;
	}

	*pResult = 0;
}

void CNewFromMdlDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}

void CNewFromMdlDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_Name==_T("")) {
		AfxMessageBox("名称不能为空");
		return;
	}
	if (mdlSelectNumber==-1)
	{
		AfxMessageBox("没有选择基模型或类别");
		return;
	}
	switch(m_selTab) {
	case 0:
        OkPart(m_Name);
		break;
	default:
		break;
	}
	//OnOK();
}

void CNewFromMdlDlg::OkPart(CString new_name)
{
	
	ProError                status;
	ProPath                 wcurrentDir;
	CString                 currentDit;
	CFtpFile                m_ftp;
	CString                 fullFileName;//包括路径的文件名
	ProPath                 name;
	ProType                 type;
	ProName                 w_name;
	int                     w_id;
	ProMdl                  mdl;

	status=ProDirectoryCurrentGet(wcurrentDir);
	currentDit=CString(wcurrentDir);
	//下载并打开模型
	if (mdlSelectNumber!=-1) {
		try
		{
			if (m_pSet.IsOpen()) m_pSet.Close();
			m_pSet.m_strFilter.Format("[Number]=%d",mdlSelectNumber);
			m_pSet.Open();
			if (m_pSet.IsBOF()) 
			{AfxMessageBox("模型已经不存在，不能打开！");m_pSet.Close();return;}
			m_pSet.MoveFirst();
			//检查当前是否有已经存在相同文件名的模型打开
			for (int i=0;;i++)
			{
				CString version;				
				version.Format("%s(%d)",new_name,i);
				version=currentDit+version;
				if (CreateDirectoryA(version,NULL))
				{
					currentDit=version;
					break;
				}
			}
			//从服务器得到基模型的文件
			if (!m_ftp.DownloadDirectorytoDirectory(m_pSet.m_FileDirectory,currentDit,myFtp))
			{
				AfxMessageBox("从服务器读取文件过程中不成功，请检查网络");
				m_pSet.Close();
				return;
			}
			fullFileName=currentDit+"\\"+m_pSet.m_FileName;
			ProStringToWstring(name,fullFileName.GetBuffer());
			status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,&mdl); 
			if (status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("没能打开下载到本地的文件！请检查版本！");
				m_pSet.Close();
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


		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			if(m_pSet.IsOpen()) m_pSet.Close();
			return;
		}
		m_pSet.Close();

	}
	CDialog::OnOK();
	status = ProWindowActivate(w_id);
	//添加模型的信息，图片信息没有包括
	CpartVariantTableSet    m_partParaInfo(m_pSet.m_pDatabase);
	CpartPerfparaTableSet   m_partPerf(m_pSet.m_pDatabase);
	//CpartPicSet             m_partPic(m_pSet.m_pDatabase);
	CpartRelationSet        m_partRel(m_pSet.m_pDatabase);
	int                     propNum=0;
	int                     num=0;
	if (mdlSelectNumber==-1)
	{
		return;
	}
	num=(int)p_UsrPartPorp.GetCount();
	for (int i=0;i<num;i++)
	{
		if (p_UsrPartPorp[i].isDef!=1) continue;
		if (p_UsrPartPorp[i].owner==mdl) return;
	}
	try
	{
		if (m_pSet.IsOpen()) m_pSet.Close();
		m_pSet.m_strFilter.Format("[Number]=%d",mdlSelectNumber);
		m_pSet.Open();
		if (m_pSet.IsBOF()) 
		{
			m_pSet.Close();
			return;
		}
		//添加属性信息
		UsrPartPorp tempProp;
		tempProp.owner=mdl;
		tempProp.Name=new_name;
		tempProp.Person=_T("");
		tempProp.Dept=_T("");
		tempProp.isDef=1;
		tempProp.FilePath=_T(currentDit+"\\");
		tempProp.FileName=m_pSet.m_FileName;
		tempProp.FatherNumber=mdlSelectNumber;
		tempProp.Code=_T(m_pSet.m_MdlCode+".(请输入)");
		tempProp.Type="prt";
		tempProp.Note=_T("");


		
		IsMdlInfoDefAndDef((ProSolid)mdl);  //没有定义就先定义
		GetMdlProp((ProSolid)mdl,&propNum); //得到定义的序号
		p_UsrPartPorp[propNum]=tempProp;//进行修改
		
		//propNum=(int)p_UsrPartPorp.Add(tempProp);

		//添加参数信息
		m_partParaInfo.m_strFilter.Format("[PrtNum]=%d",mdlSelectNumber);
		m_partParaInfo.Open();
		if (!m_partParaInfo.IsBOF())
		{
			m_partParaInfo.MoveFirst();
			while (!m_partParaInfo.IsEOF())
			{
				UsrSWTXTable1 temp;
				temp.id=m_partParaInfo.m_IdNum;
				temp.Name=m_partParaInfo.m_Name;
				temp.isCheck=1;
				temp.Note=m_partParaInfo.m_Note;
				temp.featName=m_partParaInfo.m_ParaFeatName;
				temp.dimSymbol=m_partParaInfo.m_DimSymbol;
				temp.value=m_partParaInfo.m_Value;
				temp.owner=mdl;
				temp.type=m_partParaInfo.m_Type;
				temp.subType=m_partParaInfo.m_SubType;
				temp.DwLimit=m_partParaInfo.m_DwLimit;
				temp.UpLimit=m_partParaInfo.m_UpLimit;
				p_UsrSWTXTableList.Add(temp);
				m_partParaInfo.MoveNext();
			}
		}
		m_partParaInfo.Close();
		//添加性能参数信息
		m_partPerf.m_strFilter.Format("[PrtNum]=%d",mdlSelectNumber);
		m_partPerf.Open();
		if (!m_partPerf.IsBOF())
		{
			m_partPerf.MoveFirst();
			while (!m_partPerf.IsEOF())
			{
				UsrPerfParaTable temp_perf;
				temp_perf.isCheck=1;temp_perf.Name=m_partPerf.m_Name;
				temp_perf.owner=mdl;temp_perf.value=m_partPerf.m_Value;
				temp_perf.Note=m_partPerf.m_Note;
				p_UsrPerfParaTableList.Add(temp_perf);
				m_partPerf.MoveNext();
			}
		}
		m_partPerf.Close();
		//添加关系式信息
		m_partRel.m_strFilter.Format("[PrtNum]=%d",mdlSelectNumber);
		m_partRel.Open();
		if (!m_partRel.IsBOF())
		{
			m_partRel.MoveFirst();
			while (!m_partRel.IsEOF())
			{
				//UsrRelTable temp_rel;
				//temp_rel.isCheck=1;
				//temp_rel.owner=mdl;
				//temp_rel.rel=m_partRel.m_Relation;
				//temp_rel.note=m_partRel.m_Note;
				//p_UsrRelTableList.Add(temp_rel);
				g_RelCollection.AddRel((ProSolid)mdl,m_partRel.m_Relation,m_partRel.m_Note);
				m_partRel.MoveNext();
			}
		}
		m_partRel.Close();
		////添加图片信息
		//m_partPic.m_strFilter.Format("[PrtNum]=%d",selectNumber);
		//m_partPic.Open();
		//if (!m_partPic.IsBOF())
		//{
		//	m_partPic.MoveFirst();
		//	while (!m_partPic.IsEOF())
		//	{
		//		UsrPicture temp_pic;
		//		temp_pic.isCheck=1;
		//		temp_pic.isMain=(int)m_partPic.m_IsMain;
		//		if (temp_pic.isMain==1)
		//		{
		//			p_UsrPartPorp[propNum].pic_path=localDir+"picture\\"+m_partPic.m_Name;
		//		}
		//		temp_pic.fileName=m_partPic.m_Name;
		//		temp_pic.Note=m_partPic.m_Note;
		//		temp_pic.filePath=localDir+"picture\\";
		//		temp_pic.owner=mdl;
		//		p_UsrPicture.Add(temp_pic);
		//		m_partPic.MoveNext();
		//	}
		//}
		//m_partPic.Close();

	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		if (m_pSet.IsOpen()) m_pSet.Close();
	}
	if (m_pSet.IsOpen()) m_pSet.Close();
}
