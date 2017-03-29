// ProjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "variantdesign.h"
#include "ProjectDlg.h"
#include "MyDatabase.h"
#include "PrtClass.h"
#include "DetailInfoClass.h"
#include "ProductClass.h"
#include "ProWindows.h"




extern CMyDatabase m_db;
extern CPtrArray p_classarray;
extern FtpInfo Ftp_struc;

// CProjectDlg 对话框

IMPLEMENT_DYNAMIC(CProjectDlg, CDialog)
CProjectDlg::CProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectDlg::IDD, pParent)
	//, m_note(_T(""))
	, m_all(FALSE)
	, m_note(_T(""))
{
}

CProjectDlg::~CProjectDlg()
{
}

void CProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODETAIL, m_projdetail);
	DDX_Check(pDX, IDC_CHECK_ALL, m_all);
	DDX_Control(pDX, IDC_LIST_TASK, m_task);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
}


BEGIN_MESSAGE_MAP(CProjectDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODETAIL, OnNMClickListProdetail)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TASK, OnNMClickListTask)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_FINISH, OnBnClickedButtonFinish)
	ON_BN_CLICKED(IDC_BUTTON_REJECT, OnBnClickedButtonReject)
	ON_BN_CLICKED(IDC_BUTTON_ACCEPT, OnBnClickedButtonAccept)
END_MESSAGE_MAP()


// CProjectDlg 消息处理程序

BOOL CProjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_db.Close();
	if(m_db.ReOpen()==0)
	{
		this->SendMessage(WM_CLOSE,0,0);
		return FALSE;
	}
	m_projdetail.DeleteAllItems();
	m_task.DeleteAllItems();
	ListView_SetExtendedListViewStyle(m_projdetail.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_projdetail.InsertColumn(0,"序号",LVCFMT_LEFT,20,-1);
	m_projdetail.InsertColumn(1,"项目名称",LVCFMT_LEFT,135,-1);
	m_projdetail.InsertColumn(2,"项目号",LVCFMT_LEFT,135,-1);
	m_projdetail.InsertColumn(3,"客户",LVCFMT_LEFT,135,-1);
	m_projdetail.InsertColumn(4,"状态",LVCFMT_LEFT,135,-1);
	ListView_SetExtendedListViewStyle(m_task.m_hWnd, LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_task.InsertColumn(0,"序号",LVCFMT_LEFT,40,-1);
	m_task.InsertColumn(1,"任务名",LVCFMT_LEFT,70,-1);
	m_task.InsertColumn(2,"任务人",LVCFMT_LEFT,70,-1);
	m_task.InsertColumn(3,"开始日期",LVCFMT_LEFT,70,-1);
	m_task.InsertColumn(4,"截止日期",LVCFMT_LEFT,70,-1);
	m_task.InsertColumn(5,"完成状态",LVCFMT_LEFT,75,-1);

    CString sql,str;
	sql.Format("SELECT * FROM Main where ProjectState='%s'","方案设计完成");
	int Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		this->SendMessage(WM_CLOSE,0,0);
        return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			str.Format("%d",i+1);
			m_projdetail.InsertItem(i,str);
			str=m_db.getString("ProjectName");
			m_projdetail.SetItemText(i,1,str);
			//str=m_db.getString("ProjectNo");
			str=m_db.getString("ReqID");
			m_projdetail.SetItemText(i,2,str);
			str=m_db.getString("CustomerName");
			m_projdetail.SetItemText(i,3,str);
			str=m_db.getString("ProjectState");
			m_projdetail.SetItemText(i,4,str);
			//str.Format("%d",m_db.getInt("No"));
			//m_projdetail.SetItemData(i,atoi(str));
			i++;
		} while(m_db.MoveNext());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 单击项目列表框显示这个项目的任务                                     */
/************************************************************************/
void CProjectDlg::OnNMClickListProdetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
	BOOL IsSelect=FALSE;
	m_task.DeleteAllItems();
	for (int i=0;i<m_projdetail.GetItemCount();i++)
	{
		if (m_projdetail.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			ProjectID=m_projdetail.GetItemText(i,2);           //得到项目编号
			break;
		}
	}
	if (!IsSelect)
	{
		return;
	}
	if (!GetTask(ProjectID))
	{
		AfxMessageBox("查找任务失败");
		return;
	}
	*pResult = 0;
}

void CProjectDlg::OnNMClickListTask(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	UpdateData(FALSE);
	BOOL IsSelect=FALSE;
	CString sql,str;
	for (int i=0;i<m_task.GetItemCount();i++)
	{
		if (m_task.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			IsSelect=TRUE;
			TaskID=(int)m_task.GetItemData(i);           //得到产品编号
			break;
		}
	}
	if (!IsSelect)
	{
		return;
	}
	else
	{
		sql.Format("SELECT * FROM rl_Task where ProjectID='%s' AND ID=%d",ProjectID,TaskID);
		int Z=m_db.Query(sql);
		if (Z==-1)
		{
			return;
		}
		else
		{
			m_db.MoveBegin();
			do {
				m_note=m_db.getString("Description");         //得到任务的描述
				ElementID=m_db.getInt("ElementID");           //元件ID
			} while(m_db.MoveNext());
		}
		UpdateData(FALSE);
	}
	*pResult = 0;
}
/************************************************************************/
/* 点击OK按钮开始设计零件                                               */
/************************************************************************/
void CProjectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sql,str;
	info=new DetailInfoClass;
	CFileFind ff;
	strLocalDirectoryPath="C:\\proe_temp";
	CString Saveplace;
	BOOL a=ff.FindFile(strLocalDirectoryPath);
	if(!a)
	{
		if (strLocalDirectoryPath.Right(1)!=":")
		{
			if(::CreateDirectory(strLocalDirectoryPath,NULL)==0)
			{
				delete info;
				return;
			}
		}

	}
	//查询项目组成表
	sql.Format("SELECT * FROM rl_ProjectCompose where ProjectID='%s' AND ElementID=%d",ProjectID,ElementID);
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		delete info;
		return;
	}
	else
	{
		m_db.MoveBegin();
		do {
			info->SolidID=-1;
			info->TaskType=m_db.getString("TaskType");          //工作类型（A、B、C、D）
			info->OldInsCode=m_db.getString("InsCode");         //老图号
			info->InsCode=m_db.getString("Tuhao");              //新图号
			str=m_db.getString("PartType");                     //零件或者装配件
            if(str=="零件")
				info->Type="prt";
			else
				info->Type="asm";
			info->InsName=m_db.getString("Name");               //产品零件名称
			info->MdlNum.Format("%d",m_db.getInt("ModelID"));   //模型号
			//info->SolidFilePath=m_db.getString("PlaceFolder");
			//info->DrwFilePath=info->SolidFilePath;
			//info->SolidFileName=m_db.getString("FileName");
		} while(m_db.MoveNext());
	}
	OnOK();
	if (info->TaskType=="A")
	{
		AfxMessageBox("这个任务无须做");
	}
	else
		if (info->TaskType=="B")               //根据原来模型设计
		{
			//打开已有模型
			if (!OpenMdl(info))
			{
				AfxMessageBox("打开模型失败");
				delete info;
				return;
			}
		}
		else
			if (info->TaskType=="C")           //根据已有零件
			{
				//打开已有零件或者部件
				if (!OpenExistPart(info))
				{
					AfxMessageBox("打开文件失败");
					delete info;
					return;
				}
			}
			else
				if (info->TaskType=="D")        //新建零件
				{
					//新建零件
					if (!CreateExistPart(info))
					{
						AfxMessageBox("新建文件错误");
						delete info;
						return;
					}
				}
				else
				{
					AfxMessageBox("这个任务是错误的");
					delete info;
					return;
				}
				//info->SolidFilePath=m_db.getString("PlaceFolder");                  //更新保存位置
				//info->DrwFilePath=info->SolidFilePath;
	OnOK();
}
/************************************************************************/
/*  得到模型的详细信息,打开模型                                         */
/************************************************************************/
BOOL CProjectDlg::OpenMdl(DetailInfoClass *info)
{
	int s=-1;
	BOOL IsLocal;
	CPrtClass prtclass;
	CProductClass pdtclass;
    if (info->Type=="prt")
    {
		if(!GetPartModelInfo(info->MdlNum))
		{
			AfxMessageBox("得到模型信息错误");
			return FALSE;
		}
		int k=info->OldSolidFilePath.Find(":");
		if (k==-1)
		{
			IsLocal=FALSE;
		}
		else
		{
			IsLocal=TRUE;
		}
		//下载模型,打开模型
		if(!prtclass.OpenPartFile(info,&s,IsLocal))
		{
			return FALSE;
		}
		else
		{
			info->IsInsert="T";
			prtclass.ActivatePart(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
		}
    }
	else
		if (info->Type=="asm")
		{
            if (!GetAsmModelInfo(info->MdlNum))
            {
				AfxMessageBox("得到模型信息错误");
				return FALSE;
            }
			info->DrwFilePath=strLocalDirectoryPath;           //需要下载的本地目录
			info->SolidFilePath=strLocalDirectoryPath;
			if (!pdtclass.NewProductFile(info,&s,TRUE))
			{
				return FALSE;
			}
			else
			{
				info->IsInsert="T";
				if (!pdtclass.ActivateAsm(info->solid))
				{
					AfxMessageBox("打开窗口失败");
					return FALSE;
				}
			}
		}
		return TRUE;
}
/************************************************************************/
/* 打开已有零件和部件                                                   */
/************************************************************************/
BOOL CProjectDlg::OpenExistPart(DetailInfoClass *info)
{
	int s=-1;
	BOOL IsLocal;
	CString sql,str;
	CPrtClass prtclass;
	CProductClass pdtclass;
	if (info->Type=="prt")
	{   
		sql.Format("SELECT * FROM NPartInsInfoTable WHERE InsCode='%s'",info->OldInsCode);
	}
	else
        sql.Format("SELECT * FROM NProductInsInfoTable WHERE InsCode='%s'",info->OldInsCode);
	int Z=m_db.Query(sql);
	if (Z==0 || Z==-1)
	{
		return FALSE;
	}
    else
	{
		m_db.MoveBegin();
		do {
			if (info->Type=="prt")
			{
				info->OldSolidID=m_db.getInt("PrtNum");
				info->GroupNum=m_db.getInt("TypeNum");
			}
			else
			{
				info->OldSolidID=m_db.getInt("PdtNum");
				info->GroupNum=m_db.getInt("TypeNum");
			}
			break;
		} while(m_db.MoveNext());
	}
	if (info->Type=="prt")
	{
		str.Format("%d",info->OldSolidID);
		if(!GetPartInfo(str))
		{
			AfxMessageBox("得到零件信息错误");
			return FALSE;
		}
		int k=info->OldSolidFilePath.Find(":");
		if (k==-1)
		{
			IsLocal=FALSE;
		}
		else
		{
			IsLocal=TRUE;
		}
		//下载模型,打开模型
		if(!prtclass.OpenPartFile(info,&s,IsLocal))
		{
			return FALSE;
		}
		else
		{
			info->IsInsert="T";
			prtclass.ActivatePart(((DetailInfoClass *)p_classarray.GetAt(s))->solid);
		}
	}
	else
		if (info->Type=="asm")
		{
			info->DrwFilePath=strLocalDirectoryPath;                    //需要下载的本地目录
			info->SolidFilePath=strLocalDirectoryPath;                  //需要下载的本地目录
			info->SolidFileName=info->InsCode+".asm";
            info->DrwName=info->InsCode+".drw";
            //得到产品类型
			sql.Format("SELECT * FROM NAsmType where Number=%d",info->GroupNum);
			Z=m_db.Query(sql);
			if (Z==-1 || Z==0)
			{
				return FALSE;
			}
			else
			{
				m_db.MoveBegin();
				do {
					info->GrpFrm=m_db.getString("Name");
				} while(m_db.MoveNext());
			}
			if (!pdtclass.NewProductFile(info,&s,FALSE))
			{
				return FALSE;
			}
			else
				if (!pdtclass.ActivateAsm(info->solid))
				{
					AfxMessageBox("打开窗口失败");
					return FALSE;
				}
		}
    return TRUE;
}
/************************************************************************/
/* 打开新建零件                                                         */
/************************************************************************/
BOOL CProjectDlg::CreateExistPart(DetailInfoClass *info)
{
    ProError status;
	ProName  w_name;
	ProSolid solid;
	int w_id;
	ProStringToWstring(w_name,info->InsCode.GetBuffer());
	if (info->Type=="prt")
	{
        status=ProSolidCreate(w_name,PRO_PART,&solid);
	}
	else
		status=ProSolidCreate(w_name,PRO_ASSEMBLY,&solid);
	if (status!=PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	status=ProMdlDisplay((ProMdl)solid);
	status=ProWindowCurrentGet(&w_id);
	status = ProWindowActivate(w_id);
	p_classarray.Add(info);
    return TRUE;
}
/************************************************************************/
/* 得到零件模型的详细信息                                               */
/************************************************************************/
BOOL CProjectDlg::GetPartModelInfo(CString MdlNum)
{
	CString sql,str;
	sql.Format("SELECT * FROM NPartManageTable WHERE Number=%s",MdlNum);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			info->MdlName=m_db.getString("Name");
			info->GroupNum=m_db.getInt("TypeNum");
			info->OldSolidFileName=m_db.getString("SolidFileName");
			info->OldSolidFilePath=m_db.getString("SolidFilePlace");
			info->SolidFileName=info->InsCode+".prt";
			info->DrwName=info->InsCode+".drw";
			info->OldDrwName=m_db.getString("DrawingFileName");
			info->OldDrwFilePath=m_db.getString("DrawingFilePlace");
			info->Material==m_db.getString("Material");
			info->DrwFilePath=strLocalDirectoryPath;
			info->SolidFilePath=strLocalDirectoryPath;
			break;
		} while(m_db.MoveNext());
	}		
	sql.Format("SELECT * FROM NPartType where Number=%d",info->GroupNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			info->GrpFrm=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	info->DesignState="未完成";
	info->AuditState="审核未通过";
	info->IsLocalOrServer="本地";
	info->PDMState="未提交";
	//得到模型的性能参数
	//得到性能参数的值
	sql.Format("SELECT * FROM NPartPerfparaTable WHERE PrtNum=%s",MdlNum);
	Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		info->PerformanceValue.RemoveAll();
	}
	else
	{
		m_db.MoveBegin();
		do {
			str=m_db.getString("Value");
			info->PerformanceValue.Add(str);
		} while(m_db.MoveNext());
	}
	UpdateData(FALSE);
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CProjectDlg::GetAsmModelInfo(CString MdlNum)
{
	CString sql,str;
	sql.Format("SELECT * FROM NAsmManageTable WHERE Number=%s",MdlNum);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			info->MdlName=m_db.getString("Name");
			info->GroupNum=m_db.getInt("TypeNum");
			info->OldSolidFileName=m_db.getString("SolidFileName");
			info->OldSolidFilePath=m_db.getString("SolidFilePlace");
			info->SolidFileName=info->InsCode+".asm";
			info->DrwName=info->InsCode+".drw";
			info->OldDrwName=m_db.getString("DrawingFileName");
			info->OldDrwFilePath=m_db.getString("DrawingFilePlace");
			info->DrwFilePath=strLocalDirectoryPath;
			info->SolidFilePath=strLocalDirectoryPath;
			break;
		} while(m_db.MoveNext());
	}		
	sql.Format("SELECT * FROM NAsmType where Number=%d",info->GroupNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			info->GrpFrm=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	info->DesignState="未完成";
	info->AuditState="审核未通过";
	info->IsLocalOrServer="本地";
	info->PDMState="未提交";
	//得到模型的性能参数
	//得到性能参数的值
	sql.Format("SELECT * FROM NAsmPerformanceTable WHERE AsmNum=%s",MdlNum);
	Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		info->PerformanceValue.RemoveAll();
	}
	else
	{
		m_db.MoveBegin();
		do {
			str=m_db.getString("Value");
			info->PerformanceValue.Add(str);
		} while(m_db.MoveNext());
	}
	UpdateData(FALSE);
	return TRUE;
}
/************************************************************************/
/* 得到零件的详细信息                                                   */
/************************************************************************/
BOOL CProjectDlg::GetPartInfo(CString InsCode)            //是零件号
{
	CString sql,str;
	CPrtClass prtclass;
	BOOL Find;
	sql.Format("select * from NPartInsInfoTable WHERE PrtNum=%s",InsCode);
	int Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			info->OldInsCode=m_db.getString("InsCode");
			info->PDMCode=m_db.getString("PDMNum");
			info->PDMState=m_db.getString("PDMState");
			info->OldSolidID=m_db.getInt("PrtNum");
			info->GroupNum=m_db.getInt("TypeNum");
		    info->MdlNum.Format("%d",m_db.getInt("MdlNum"));
			info->OldSolidFileName=m_db.getString("SolidFileName");
			info->OldSolidFilePath=m_db.getString("SolidPlace");
			info->OldDrwName=m_db.getString("DrawingName");;
			info->OldDrwFilePath=m_db.getString("DrawingPlace");
			info->DrwFilePath=strLocalDirectoryPath;
			info->SolidFilePath=strLocalDirectoryPath;
			info->SolidFileName=info->InsCode+".prt";
			info->DrwName=info->InsCode+".drw";
			break;
		} while(m_db.MoveNext());
	}
	//得到类别
	sql.Format("SELECT * FROM NPartType where Number=%d",info->GroupNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		m_db.MoveBegin();
		do {
			info->GrpFrm=m_db.getString("Name");
		} while(m_db.MoveNext());
	}
	//得到模型名
	sql.Format("SELECT * FROM NPartManageTable WHERE Number=%s",info->MdlNum);
	Z=m_db.Query(sql);
	if (Z==-1 || Z==0)
	{
		return FALSE;
	}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
			info->MdlName=m_db.getString("Name");
			break;
		} while(m_db.MoveNext());
	}
	//	得到PDM的保存位置和文件名
	if (info->PDMState=="提交")
	{
		if (!prtclass.GetDocumentsFromPDM(Ftp_struc.Database,info,&Find))    //这次调用，最后这个参数没有用
		{
			return FALSE;
		}
	}
	info->DesignState="未完成";
	info->AuditState="审核未通过";
	info->IsLocalOrServer="本地";
	info->PDMState="未提交";
	//查找性能参数
	sql.Format("SELECT * FROM NPartInsPerfTable WHERE PrtNum=%d",info->OldSolidID);
	Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	if (Z==0)
	{
		info->PerformanceValue.RemoveAll();
	}
	else
	{
		m_db.MoveBegin();
		do {
			str=m_db.getString("Value");
			info->PerformanceValue.Add(str);
		} while(m_db.MoveNext());
	}
	return TRUE;
}
void CProjectDlg::OnBnClickedButtonFinish()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sql;
	for (int i=0;i<m_task.GetItemCount();i++)
	{
		if (m_task.GetCheck(i)==TRUE)
		{
			TaskID=(int)m_task.GetItemData(i);
			sql.Format("SELECT * FROM rl_Task where ProjectID='%s' AND ID=%d",ProjectID,TaskID);
			int Z=m_db.Query(sql);
			if (Z==-1)
			{
				return;
			}
			else
			{
				m_db.MoveBegin();
				do {
					ElementID=m_db.getInt("ElementID");
				} while(m_db.MoveNext());
			}
			sql.Format("Update rl_Task Set State='完成' where ProjectID='%s' and ID=%d",ProjectID,TaskID);
			try
			{
				m_db.m_pConnection->Close();
				m_db.ReOpen();
				m_db.m_pConnection->BeginTrans();
				if(m_db.Execute(sql)==0)
				{
					m_db.m_pConnection->RollbackTrans();
					return;
				}
				else
				{
					sql.Format("Update rl_ProjectCompose Set State='完成' where ProjectID='%s' and ElementID=%d",ProjectID,ElementID);
					if(m_db.Execute(sql)==0)
					{
						m_db.m_pConnection->RollbackTrans();
						return;
					}
					m_db.m_pConnection->CommitTrans();
				}
			}
            catch (...)
			{
				m_db.m_pConnection->RollbackTrans();
            }
		}
	}
	m_task.DeleteAllItems();
	if (!GetTask(ProjectID))
	{
		AfxMessageBox("更新任务失败");
		return;
	}
}
/************************************************************************/
/* 根据要求得到所选项目的任务                                               */
/************************************************************************/
BOOL CProjectDlg::GetTask(CString ProjectID)        // 项目编号，是数字
{
	UpdateData(TRUE);
	UpdateData(FALSE);
	BOOL IsSelect=FALSE;
	CString sql,str;
	if (m_all)       //显示当前选中项目的所有任务
		sql.Format("SELECT * FROM rl_Task where ProjectID='%s'",ProjectID);
	else
		sql.Format("SELECT * FROM rl_Task where ProjectID='%s' and TaskUser='%s'",ProjectID,UserName);
	int Z=m_db.Query(sql);
	if (Z==-1)
	{
		return FALSE;
	}
	else
		if (Z==0)
		{
			return TRUE;
		}
	else
	{
		int i=0;
		m_db.MoveBegin();
		do {
				str.Format("%d",i+1);
				m_task.InsertItem(i,str);
				str=m_db.getString("Name");
				m_task.SetItemText(i,1,str);
				str=m_db.getString("TaskUser");
				m_task.SetItemText(i,2,str);
				str=m_db.getString("StartDate");
                m_task.SetItemText(i,3,str);
				str=m_db.getString("EndDate");
				m_task.SetItemText(i,4,str);
				str=m_db.getString("State");
				m_task.SetItemText(i,5,str);
				str.Format("%d",m_db.getInt("ID"));
				m_task.SetItemData(i,atoi(str));
			i++;
		} while(m_db.MoveNext());
	}
	return TRUE;
}
//拒绝按钮
void CProjectDlg::OnBnClickedButtonReject()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sql;
	for (int i=0;i<m_task.GetItemCount();i++)
	{
		if (m_task.GetCheck(i)==TRUE)
		{
			TaskID=(int)m_task.GetItemData(i);
			sql.Format("SELECT * FROM rl_Task where ProjectID='%s' AND ID=%d",ProjectID,TaskID);
			int Z=m_db.Query(sql);
			if (Z==-1)
			{
				return;
			}
			else
			{
				m_db.MoveBegin();
				do {
					ElementID=m_db.getInt("ElementID");
				} while(m_db.MoveNext());
			}
			sql.Format("Update rl_Task Set State='拒绝' where ProjectID='%s' and ID=%d",ProjectID,TaskID);
			try
			{
				m_db.m_pConnection->Close();
				m_db.ReOpen();
				m_db.m_pConnection->BeginTrans();
				if(m_db.Execute(sql)==0)
				{
					m_db.m_pConnection->RollbackTrans();
					return;
				}
				else
				{
					sql.Format("Update rl_ProjectCompose Set State='拒绝' where ProjectID='%s' and ElementID=%d",ProjectID,ElementID);
					if(m_db.Execute(sql)==0)
					{
						m_db.m_pConnection->RollbackTrans();
						return;
					}
					m_db.m_pConnection->CommitTrans();
				}
			}
			catch (...)
			{
				m_db.m_pConnection->RollbackTrans();
			}
		}
	}
	m_task.DeleteAllItems();
	if (!GetTask(ProjectID))
	{
		AfxMessageBox("更新任务失败");
		return;
	}
}
//接受
void CProjectDlg::OnBnClickedButtonAccept()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sql;
	for (int i=0;i<m_task.GetItemCount();i++)
	{
		if (m_task.GetCheck(i)==TRUE)
		{
			TaskID=(int)m_task.GetItemData(i);
			sql.Format("SELECT * FROM rl_Task where ProjectID='%s' AND ID=%d",ProjectID,TaskID);
			int Z=m_db.Query(sql);
			if (Z==-1)
			{
				return;
			}
			else
			{
				m_db.MoveBegin();
				do {
					ElementID=m_db.getInt("ElementID");
				} while(m_db.MoveNext());
			}
			sql.Format("Update rl_Task Set State='接受' where ProjectID='%s' and ID=%d",ProjectID,TaskID);
			try
			{
				m_db.m_pConnection->Close();
				m_db.ReOpen();
				m_db.m_pConnection->BeginTrans();
				if(m_db.Execute(sql)==0)
				{
					m_db.m_pConnection->RollbackTrans();
					return;
				}
				else
				{
					sql.Format("Update rl_ProjectCompose Set State='接受' where ProjectID='%s' and ElementID=%d",ProjectID,ElementID);
					if(m_db.Execute(sql)==0)
					{
						m_db.m_pConnection->RollbackTrans();
						return;
					}
					m_db.m_pConnection->CommitTrans();
				}
			}
			catch (...)
			{
				m_db.m_pConnection->RollbackTrans();
			}
		}
	}
	m_task.DeleteAllItems();
	if (!GetTask(ProjectID))
	{
		AfxMessageBox("更新任务失败");
		return;
	}
}
