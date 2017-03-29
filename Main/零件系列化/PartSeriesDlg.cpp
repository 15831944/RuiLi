// PartSeriesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "PartSeriesDlg.h"
#include "AddMdlSeriesDlg.h"
#include "MdlSeriesPicShowDlg.h"
//extern int DlgIcon;
// CPartSeriesDlg 对话框

IMPLEMENT_DYNAMIC(CPartSeriesDlg, CDialog)
CPartSeriesDlg::CPartSeriesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartSeriesDlg::IDD, pParent)
	, mdlNumber(0)
	, p_db(NULL)
	, m_current_item(-1)
	,m_InsTableSet(NULL)
	, combColumn(-1)
{
}

CPartSeriesDlg::~CPartSeriesDlg()
{
	if (m_InsTableSet!=NULL) {
		delete m_InsTableSet;
	}
}

void CPartSeriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_ADD_BUTTON, m_AddButton);
	DDX_Control(pDX, IDC_BUTTON1, m_OkButton);
	DDX_Control(pDX, IDC_BUTTON2, m_CancelAddButton);
	DDX_Control(pDX, IDC_MOD_BUTTON, m_ModifyButton);
}


BEGIN_MESSAGE_MAP(CPartSeriesDlg, CDialog)
	ON_MESSAGE(WM_VALIDATE, OnEndLabelEditVariableCriteria)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)	
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_NOTIFY(LVN_HOTTRACK, IDC_LIST1, OnLvnHotTrackList1)
	ON_BN_CLICKED(IDC_DEL_BUTTON, OnBnClickedDelButton)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_MOD_BUTTON, OnBnClickedModButton)
	ON_BN_CLICKED(IDC_VIEW_BUTTON, OnBnClickedViewButton)
END_MESSAGE_MAP()


// CPartSeriesDlg 消息处理程序

BOOL CPartSeriesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	if(p_db==NULL) return FALSE;
	//设置按钮状态
	m_AddButton.EnableWindow(TRUE);
	m_CancelAddButton.EnableWindow(FALSE);
	m_OkButton.EnableWindow(FALSE);
	//设置提示信息
	EnableToolTips(TRUE);		
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	CWnd* pW=GetDlgItem(IDC_LIST1);
	m_tooltip.AddTool(pW,"模型的系列表");
	//设置列表框
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_TRACKSELECT);
    InitListByMdlInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CPartSeriesDlg::SetMdlNumberAndDB(long num,CDatabase * db)
{
	mdlNumber=num;
	p_db=db;
	m_InsTableSet=new CPartInsTableSet(p_db);
	//确定数据库连接
	if(!p_db->IsOpen()) 
	{
		CString strConnect=m_InsTableSet->GetDefaultConnect();
		p_db->Open(NULL,FALSE,FALSE,strConnect,FALSE);
	}
}
/************************************************************************/
/* 设置模型的性能参数和事物特性表信息                                   */
/************************************************************************/
void CPartSeriesDlg::InitListByMdlInfo(void)
{
	int        tempColNum=0;
	ItemProp   temp;
	CString    sql_lang;
	CpartVariantTableSet      m_mdlVariantSet(p_db);
	CPartMdlRecordSet         m_MdlInfoSet(p_db);
	CpartPerfparaTableSet     m_mdlPerfparaSet(p_db);
	MdlInsInfo mdlInfotemp; //临时的系列信息

	for(int i=0;i<(int)p_ColProp.GetCount();i++)
	{
		m_List.DeleteColumn(0);
	}
	p_ColProp.RemoveAll();
	p_InsInfo.RemoveAll();
	m_List.DeleteAllItems();

	try{
		//列表框的设置 0：编号 1：名称  2：参数   ……  n：性能参数	
		//1.第0行第0列
		m_List.InsertColumn(tempColNum,"编号",LVCFMT_LEFT,30,-1);
		temp.para_num=-1;temp.note=" 模型系列的编号";temp.type=-1;
		p_ColProp.Add(temp);
		m_List.InsertItem(0,"");
		
		//2.第0行第1列   获得模型的名称		
		m_MdlInfoSet.m_strFilter.Format("[Number]=%d",mdlNumber);		
		if(!m_MdlInfoSet.Open())
		{
			AfxMessageBox("数据库连接失败！");
			return;
		}
		if (m_MdlInfoSet.IsBOF()) 		{
			
			return;
		}
		m_MdlInfoSet.MoveFirst();
		//设置名称列
		tempColNum=1;temp.para_num=-1;temp.note="模型及系列的名称";	temp.type=-1;
		p_ColProp.Add(temp);
		//获得模型的基本信息
		mdlInfotemp.Name=m_MdlInfoSet.m_Name;mdlInfotemp.Code=m_MdlInfoSet.m_MdlCode;
		mdlInfotemp.Person=m_MdlInfoSet.m_MdlerName;mdlInfotemp.Dept=m_MdlInfoSet.m_MdlerDepartment;
		mdlInfotemp.m_time=m_MdlInfoSet.m_Datatime;
		p_InsInfo.Add(mdlInfotemp);
		m_List.InsertColumn(tempColNum,"名称",LVCFMT_LEFT,50,-1);
		m_List.SetItemText(0,tempColNum,m_MdlInfoSet.m_Name);
		//m_MdlInfoSet.Close();



		//查找事物特性表参数
		//if(m_mdlVariantSet.IsOpen()) m_mdlVariantSet.Close();
		//sql_lang.Format("SELECT * FROM [dbo].[partVariantTable] WHERE [PrtNum]=%d",mdlNumber);
		m_mdlVariantSet.m_strFilter.Format("[PrtNum]=%d",mdlNumber);
		if(m_mdlVariantSet.Open())
		{
			if (!m_mdlVariantSet.IsBOF()) {
				m_mdlVariantSet.MoveFirst();

				while (!m_mdlVariantSet.IsEOF()) {
					tempColNum++;

					temp.para_num=m_mdlVariantSet.m_Number;
					temp.note=m_mdlVariantSet.m_Note;

					if (m_mdlVariantSet.m_SubType=="PRO_PARAM_DOUBLE" || m_mdlVariantSet.m_Type=="dim") {
						temp.type=1;  //浮点型
					}
					else if (m_mdlVariantSet.m_SubType=="PRO_PARAM_INTEGER") {
						temp.type=2;  //整型
					}
					else if (m_mdlVariantSet.m_SubType=="PRO_PARAM_BOOLEAN") {
						temp.type=3;  //布尔型
					}
					else {
						temp.type=0;  //字符型
					}
	
					p_ColProp.Add(temp);
					m_List.InsertColumn(tempColNum,m_mdlVariantSet.m_Name,LVCFMT_LEFT,50,-1);
					m_List.SetItemText(0,tempColNum,m_mdlVariantSet.m_Value);
					m_mdlVariantSet.MoveNext();
				}

			}	
			//m_mdlVariantSet.Close();
		}
		else
		{
			AfxMessageBox("添加事物特性表参数值时，因为数据库连接失败，没法显示事物特性表信息！");
			return;
		}

		//中间留出一列
		tempColNum++;
		temp.para_num=-1;
		temp.note="";
		temp.type=-1;
		p_ColProp.Add(temp);
		m_List.InsertColumn(tempColNum,"",LVCFMT_LEFT,10,-1);
		m_List.SetItemText(0,tempColNum,"");
		//查找性能特性表参数
		//if(m_mdlPerfparaSet.IsOpen()) m_mdlPerfparaSet.Close();
		//sql_lang.Format("SELECT * FROM [dbo].[partPerfparaTable] WHERE [PrtNum]=%d",mdlNumber);
		m_mdlPerfparaSet.m_strFilter.Format("[PrtNum]=%d",mdlNumber);
		if(m_mdlPerfparaSet.Open())
		{

			if (!m_mdlPerfparaSet.IsBOF()) {
				m_mdlPerfparaSet.MoveFirst();
				while (!m_mdlPerfparaSet.IsEOF()) {
					tempColNum++;

					temp.para_num=m_mdlPerfparaSet.m_Number;
					temp.note=m_mdlPerfparaSet.m_Note;
					temp.type=0;
					p_ColProp.Add(temp);

					m_List.InsertColumn(tempColNum,m_mdlPerfparaSet.m_Name,LVCFMT_LEFT,50,-1);
					m_List.SetItemText(0,tempColNum,m_mdlPerfparaSet.m_Value);
					m_mdlPerfparaSet.MoveNext();

				}
			}	
			//m_mdlPerfparaSet.Close();
		}
		else
		{
			AfxMessageBox("添加性能表参数值时，因为数据库连接失败，没法显示事物特性表信息！");
			return;
		}

	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		delete e;
		//if(m_MdlInfoSet.IsOpen()) m_MdlInfoSet.Close();
		//if(m_mdlPerfparaSet.IsOpen()) m_mdlPerfparaSet.Close();
		//if(m_mdlVariantSet.IsOpen())  m_mdlVariantSet.Close();
		return;
	}


    //添加系列
    AddSeriesData();

}

/************************************************************************/
/* 添加模型的系列                                                       */
/************************************************************************/
void CPartSeriesDlg::AddSeriesData(void)
{
	int        itemNum=1;  //行，从第二行（1）开始添加
	CString    csInsNum;
	int        ColNum=0;
	MdlInsInfo temp; //临时的系列信息
	CpartInsValueTableSet temp_ins_value_Set(p_db);
	CpartInsPerfValueTableSet temp_ins_Perf_Set(p_db);
	try
	{
		if(m_InsTableSet->IsOpen()) m_InsTableSet->Close();
		m_InsTableSet->m_strFilter.Format("[PrtNum]=%d",mdlNumber);
		m_InsTableSet->Open();
		if(m_InsTableSet->IsBOF()) 
		{
			m_InsTableSet->Close();
			return;
		}

		    m_InsTableSet->MoveFirst();
			//从第二列开始进行添加
			while (!m_InsTableSet->IsEOF()) {
				//设置0，1列的值
				csInsNum.Format("%d",itemNum);				
				m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,itemNum,csInsNum,0,0,0,m_InsTableSet->m_InsNum);
				m_List.SetItemText(itemNum,1,m_InsTableSet->m_InsName);
				//获得实例的基本信息
				temp.Name=m_InsTableSet->m_InsName;
				temp.Code=m_InsTableSet->m_InsCode;
				temp.Person=m_InsTableSet->m_InsCreaterName;
				temp.Dept=m_InsTableSet->m_InsCreateDpt;
				temp.Note=m_InsTableSet->m_InsNote;
				temp.mdlNum=mdlNumber;
				temp.m_time=m_InsTableSet->m_InsCreateTime;
				p_InsInfo.Add(temp);
				//首先添加事物特性表
				ColNum=2;
				while (p_ColProp[ColNum].para_num!=-1) {
					temp_ins_value_Set.m_strFilter.Format("[PrtNum]=%d AND [InsNum]=%d AND [Num]=%d"
						,mdlNumber,m_InsTableSet->m_InsNum,p_ColProp[ColNum].para_num);
					temp_ins_value_Set.Open();
					if (!temp_ins_value_Set.IsBOF()) {
						temp_ins_value_Set.MoveFirst();
						m_List.SetItemText(itemNum,ColNum,temp_ins_value_Set.m_Value);
					}
					temp_ins_value_Set.Close();
					ColNum++;
				}
				ColNum++;
				//添加性能参数				
				while (ColNum<(int)p_ColProp.GetCount()) {
					temp_ins_Perf_Set.m_strFilter.Format("[PrtNum]=%d AND [InsNum]=%d AND [Num]=%d"
						,mdlNumber,m_InsTableSet->m_InsNum,p_ColProp[ColNum].para_num);
					temp_ins_Perf_Set.Open();
					if (!temp_ins_Perf_Set.IsBOF()) {
						temp_ins_Perf_Set.MoveFirst();
						m_List.SetItemText(itemNum,ColNum,temp_ins_Perf_Set.m_Value);
					}
					temp_ins_Perf_Set.Close();
					ColNum++;
				}

               //移动到下一条记录
				m_InsTableSet->MoveNext();
				itemNum++;
				ColNum=2;
		     
	        }
	}
	catch (CDBException * e) {
        AfxMessageBox(e->m_strError);
		delete e;
		return;
	}

}
/************************************************************************/
/* 添加系列                                                             */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedAddButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddMdlSeriesDlg dlg;
	CString          sql_lang;
	int              colNum=0;
	int              itemNum=0;
	CString          str="";
	CStringArray     exitSerieName;
	CPartMdlRecordSet         m_MdlInfoSet(p_db);
	m_MdlInfoSet.m_strFilter.Format("[Number]=%d",mdlNumber);	
	m_MdlInfoSet.Open();
	if (m_MdlInfoSet.IsBOF()) 
	{
		m_MdlInfoSet.Close();
		return;
	}
	//获得已经有的系列的名称，跟编码,使新输入的名称和编码不能够同已经有的相同
	dlg.SetMdlNumberAndCode(mdlNumber,m_MdlInfoSet.m_MdlCode,&p_InsInfo);
	m_MdlInfoSet.Close();
	//输入新系列的名称、编码、等等信息
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}    
	//确认之后，改变按钮的状态，使添加按钮无效，使提交和撤销按钮有效
	m_AddButton.EnableWindow(FALSE);
	m_OkButton.EnableWindow(TRUE);
	m_CancelAddButton.EnableWindow(TRUE);
	//获得信息
	newIns=dlg.newMdl;
	//对新的名称
    //设置新系列的默认值
	itemNum=m_List.GetItemCount();
	colNum=m_List.GetColumnCounts();
	str.Format("%d",itemNum);
	//设置新添加的行为当前行
	m_current_item=itemNum;
    m_List.InsertItem(LVIF_PARAM|LVIF_TEXT,itemNum,str,0,0,0,-1);
	m_List.SetItemText(itemNum,1,newIns.Name);
	for(int i=2;i<colNum;i++)
	{
		m_List.SetItemText(itemNum,i,m_List.GetItemText(0,i));
	}
	m_AddButton.EnableWindow(FALSE);
	m_CancelAddButton.EnableWindow(TRUE);
	m_OkButton.EnableWindow(TRUE);
}
/************************************************************************/
/* 提交新系列                                                           */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int      newInsNum=0;
	CString  sql_lang;
	int      ColNum=0;
	int      ColCurrent=0;

	CpartInsValueTableSet     m_InsValueTableSet(p_db);
	CpartInsPerfValueTableSet m_InsPerfValueTableSet(p_db);

	if(!CheckNewSeries()) return;
	newInsNum=GetMdlSeriesMaxNumber()+1;
	if(m_InsTableSet->IsOpen())           m_InsTableSet->Close();
	if (p_db->CanTransact()) {
		if(!m_InsTableSet->Open()) return;
		if(!m_InsValueTableSet.Open()) return;
		if(!m_InsPerfValueTableSet.Open()) return;

		if(!p_db->BeginTrans())  return;
		//添加系列信息
		try{
			m_InsTableSet->AddNew();
			m_InsTableSet->m_PrtNum=mdlNumber;
			m_InsTableSet->m_InsNum=newInsNum;
			m_InsTableSet->m_InsName=newIns.Name;
			m_InsTableSet->m_InsCode=newIns.Code;
			m_InsTableSet->m_InsNote=newIns.Note;
			m_InsTableSet->m_InsCreaterName=newIns.Person;
			m_InsTableSet->m_InsCreateDpt=newIns.Dept;
			CTime  time=CTime::GetCurrentTime();
			m_InsTableSet->m_InsCreateTime=time;
			m_InsTableSet->Update();

			ColNum=m_List.GetColumnCounts();		

			for(ColCurrent=2;ColCurrent<ColNum;ColCurrent++)
			{
				//如果当前项的数据中的参数不为-1				
				if(p_ColProp[ColCurrent].para_num==-1 ) break;
				m_InsValueTableSet.AddNew();
				m_InsValueTableSet.m_PrtNum=mdlNumber;
				m_InsValueTableSet.m_InsNum=newInsNum;
				m_InsValueTableSet.m_Num=p_ColProp[ColCurrent].para_num;
				m_InsValueTableSet.m_Value=m_List.GetItemText(m_current_item,ColCurrent);
				m_InsValueTableSet.Update();
			}

			ColCurrent+=1;
			for(;ColCurrent<ColNum;ColCurrent++)
			{				
				m_InsPerfValueTableSet.AddNew();
				m_InsPerfValueTableSet.m_PrtNum=mdlNumber;
				m_InsPerfValueTableSet.m_InsNum=newInsNum;
				m_InsPerfValueTableSet.m_Num=p_ColProp[ColCurrent].para_num;
				m_InsPerfValueTableSet.m_Value=m_List.GetItemText(m_current_item,ColCurrent);
				m_InsPerfValueTableSet.Update();
			}
			p_db->CommitTrans();

		}
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			delete e;
			p_db->Rollback();
			if(m_InsTableSet->IsOpen())           m_InsTableSet->Close();	
			return;
		}
		m_InsTableSet->Close();	
        p_InsInfo.Add(newIns);

	}

	//设置按钮
	m_AddButton.EnableWindow(TRUE);
	m_CancelAddButton.EnableWindow(FALSE);
	m_OkButton.EnableWindow(FALSE);
	//设置该列为不可编辑	
	m_current_item=-1;
	//列表框列为-1
	combColumn=-1;
	//添加事物特性表
	//添加性能参数

}
/************************************************************************/
/* 撤销新添加的系列                                                     */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_AddButton.EnableWindow(TRUE);
	m_OkButton.EnableWindow(FALSE);
	m_CancelAddButton.EnableWindow(FALSE);
	m_List.DeleteItem(m_current_item);
	m_current_item=-1;
	combColumn=-1;
}
/************************************************************************/
/* 获得最大系列号                                                       */
/************************************************************************/
long CPartSeriesDlg::GetMdlSeriesMaxNumber(void)
{
	long      maxNum=0;
	CString   sql_lang;
	if(m_InsTableSet->IsOpen()) m_InsTableSet->Close();
	m_InsTableSet->m_strFilter.Format("[PrtNum]=%d",mdlNumber);	
	m_InsTableSet->Open();
	if (m_InsTableSet->IsBOF()) {
		m_InsTableSet->Close();
		return maxNum;
	}
	m_InsTableSet->MoveFirst();
	while (!m_InsTableSet->IsEOF()) {
		maxNum=maxNum>=m_InsTableSet->m_InsNum?maxNum:m_InsTableSet->m_InsNum;
		m_InsTableSet->MoveNext();
	}
	m_InsTableSet->Close();
	return maxNum;
	return 0;
}
/************************************************************************/
/* 对新添加的系列进行检查                                               */
/************************************************************************/
bool CPartSeriesDlg::CheckNewSeries(void)
{
      
  for(int i=2;i<m_List.GetColumnCounts();i++)
	{		
		//参数列不能为空
		if(p_ColProp[i].para_num==-1) continue;
		if(m_List.GetItemText(m_current_item,i).IsEmpty())
		{
			CString  temp;
			LVCOLUMN temp_colprop;
			m_List.GetColumn(i,&temp_colprop);			
			temp.Format("参数%s,不能为空！",temp_colprop.pszText);
			AfxMessageBox(temp);
			return false;
		}
	}
	return true;
}

/************************************************************************/
/* 设置当前添加的行可以编辑                                             */
/************************************************************************/
void CPartSeriesDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	int      item=0;
    int      ColNum=0;
	CString  str;
	item=(int)m_List.GetHotItem();	
    //不是当前行的都不可编辑
	if (item!=m_current_item || m_current_item==-1) {
		ColNum=m_List.GetColumnCounts();
		for(int i=2;i<ColNum;i++)
		{
			if(m_List.IsCombo(i))     m_List.SetComboColumns(i,false);
			if(!m_List.IsReadOnly(i))  m_List.SetReadOnlyColumns(i,true);
		}
	}	
	else
	{
		ColNum=m_List.GetColumnCounts();
		for(int i=2;i<ColNum;i++)
		{
			if(m_List.IsCombo(i)) m_List.SetComboColumns(i,false);
			m_List.SetReadOnlyColumns(i,true);
			switch(p_ColProp[i].type) {
				case 0:
					str="";				
					m_List.SetReadOnlyColumns(i,false);
					m_List.SetColumnValidEditCtrlCharacters(str,i);		
					break;
				case 1:
					str="987654321.0";				
					m_List.SetReadOnlyColumns(i,false);
					m_List.SetColumnValidEditCtrlCharacters(str,i);		
					break;
				case 2:
					str="1230456789";				
					m_List.SetReadOnlyColumns(i,false);
					m_List.SetColumnValidEditCtrlCharacters(str,i);				
					break;
				case 3: //如果是布尔型，则添加复合列表框，是、否
					m_List.SetReadOnlyColumns(i,true);
					m_List.SetComboColumns(i,true);
					combColumn=i;
					break;
				default:
					break;
			}
		}

	}	
	*pResult = 0;
}

LRESULT CPartSeriesDlg::OnEndLabelEditVariableCriteria(WPARAM wParam, LPARAM lParam) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)lParam;
	// TODO: Add your control notification handler code here

	return 0;
}
LRESULT CPartSeriesDlg::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());

	// Get the inplace combbox top left
	CRect obWindowRect;

	pInPlaceCombo->GetWindowRect(&obWindowRect);

	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft()); 

	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_List.GetWindowRect(&obWindowRect);

	int iColIndex = (int )wParam;

	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll(); 

	if (obWindowRect.PtInRect(obInPlaceComboTopLeft)) 
	{				
		 
		if(iColIndex==combColumn)
		{
			pComboList->AddTail("1");
			pComboList->AddTail("0");		
		}			
	}
	return true;
}


void CPartSeriesDlg::OnLvnHotTrackList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pW=GetDlgItem(IDC_LIST1);  
	LVHITTESTINFO lvhti;
	POINT myPoint=GetCurrentMessage()->pt;
    ScreenToClient(&myPoint);
    lvhti.pt = myPoint;
	m_List.SubItemHitTest(&lvhti);
	
	if (lvhti.iItem==-1) {
		m_tooltip.UpdateTipText("",pW,0);
		return;
	}
	if (lvhti.iSubItem==-1) {
		m_tooltip.UpdateTipText("",pW,0);
		return;
	}
	//如果是中间空的列则为
	if (lvhti.iSubItem>1 && p_ColProp[lvhti.iSubItem].para_num==-1) {
        m_tooltip.UpdateTipText("",pW,0);
		return;
	}
	m_tooltip.UpdateTipText(p_ColProp[lvhti.iSubItem].note,pW,0);	
	*pResult = 0;
}

BOOL CPartSeriesDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
    m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}
/************************************************************************/
/* 删除系列                                                             */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedDelButton()
{
	// TODO: 在此添加控件通知处理程序代码
	int     itemNum=0;
	int     selectedItem=0;
	CString temp;
	itemNum=m_List.GetItemCount();
	for(int i=1;i<(m_current_item==-1?itemNum:itemNum-1);i++)
	{
         if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) selectedItem=i;
	}
	if(selectedItem==0) return;	
	temp.Format("确定删除系列%s",p_InsInfo[selectedItem].Name);
	
    if(AfxMessageBox(temp,MB_OK)==IDOK)
	{
		//删除实例
		try{
			if(m_InsTableSet->IsOpen()) m_InsTableSet->Close();
			m_InsTableSet->m_strFilter.Format("[PrtNum]=%d AND [InsName]='%s'",mdlNumber,p_InsInfo[selectedItem].Name);
			m_InsTableSet->Open();
			if (!m_InsTableSet->IsBOF()) {
				temp.Format("DELETE FROM [dbo].[partInsPerfValueTable] WHERE [PrtNum]=%d AND [InsNum]=%d",mdlNumber,m_InsTableSet->m_InsNum);
				p_db->ExecuteSQL(temp);
                temp.Format("DELETE FROM [dbo].[partInsValueTable] WHERE [PrtNum]=%d AND [InsNum]=%d",mdlNumber,m_InsTableSet->m_InsNum);
				p_db->ExecuteSQL(temp);
				m_InsTableSet->Delete();
			}
	
		}
		catch (CDBException * e) {
			AfxMessageBox( e->m_strError );
		}
		m_InsTableSet->Close();
        //更新列表
		InitListByMdlInfo();

	}

    
}
/************************************************************************/
/* 查看属性，信息                                                       */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
     CAddMdlSeriesDlg dlg;
	 dlg.newMdl=p_InsInfo[0];
	 dlg.strCaption="查看主模型信息";
	 dlg.SetMdlNumberAndCode(-1,"",NULL);
	 dlg.DoModal();
}
/************************************************************************/
/* 查看模型的样图                                                       */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
    //CPartMdlRecordSet m_mdlInfoSet(p_db);
	CMdlSeriesPicShowDlg dlg;
	dlg.SetMdlNumber(mdlNumber,p_db);
	dlg.DoModal();
    
}
/************************************************************************/
/* 修改系列的参数值                                                     */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedModButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CString                   windowtext;
	CString                   sql;
	int                       itemNum=0;
	int                       selectedItem=-1;
	CpartInsValueTableSet     m_InsValueTableSet(p_db);
	CpartInsPerfValueTableSet m_InsPerfValueTableSet(p_db);
	m_ModifyButton.GetWindowText(windowtext);
	if (windowtext=="修改") {
		if (m_current_item!=-1) {
			AfxMessageBox("请先提交或撤销当前添加的系列！");
			return;
		}
		itemNum=m_List.GetItemCount();
		for(int i=1;i<itemNum;i++)
		{
			
			if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) selectedItem=i;
		}
		if(selectedItem==-1) return;	
		m_current_item=selectedItem;
		m_ModifyButton.SetWindowText("确认");
		m_AddButton.EnableWindow(FALSE);
	}
	else
	{
		//修改
		if(!CheckNewSeries()) return;
		try{
				if(m_InsTableSet->IsOpen()) m_InsTableSet->Close();
				m_InsTableSet->m_strFilter.Format("[PrtNum]=%d AND [InsName]='%s'",mdlNumber,p_InsInfo[m_current_item].Name);
				m_InsTableSet->Open();
				if (m_InsTableSet->IsBOF()) {
					m_InsTableSet->Close();
					InitListByMdlInfo();
					m_current_item=-1;
					m_ModifyButton.SetWindowText("修改");
					m_AddButton.EnableWindow(TRUE);
					return;
				}
				
				int  ColNum=0;
				int ColCurrent=0;
				ColNum=m_List.GetColumnCounts();
				for(ColCurrent=2;ColCurrent<ColNum;ColCurrent++)
				{
					//如果当前项的数据中的参数不为-1				
					if(p_ColProp[ColCurrent].para_num==-1 ) break;
					m_InsValueTableSet.m_strFilter.Format("[PrtNum]=%d AND [InsNum]=%d  AND [Num]=%d"
						,mdlNumber,m_InsTableSet->m_InsNum,p_ColProp[ColCurrent].para_num );
					m_InsValueTableSet.Open();
					m_InsValueTableSet.Edit();
					m_InsValueTableSet.m_Value=m_List.GetItemText(m_current_item,ColCurrent);
					m_InsValueTableSet.Update();
					m_InsValueTableSet.Close();
				}

				ColCurrent+=1;
				for(;ColCurrent<ColNum;ColCurrent++)
				{	
					m_InsPerfValueTableSet.m_strFilter.Format("[PrtNum]=%d AND [InsNum]=%d  AND [Num]=%d"
						,mdlNumber,m_InsTableSet->m_InsNum,p_ColProp[ColCurrent].para_num );
					m_InsPerfValueTableSet.Open();
					m_InsPerfValueTableSet.Edit();
					m_InsPerfValueTableSet.m_Value=m_List.GetItemText(m_current_item,ColCurrent);
					m_InsPerfValueTableSet.Update();
					m_InsPerfValueTableSet.Close();
				}
		  }
		catch (CDBException * e) {
			AfxMessageBox(e->m_strError);
			m_InsTableSet->Close();
			return;
		}
		m_InsTableSet->Close();

		InitListByMdlInfo();
		m_current_item=-1;
		m_ModifyButton.SetWindowText("修改");
		m_AddButton.EnableWindow(TRUE);

	}

}
/************************************************************************/
/* 查看实例信息                                                         */
/************************************************************************/
void CPartSeriesDlg::OnBnClickedViewButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddMdlSeriesDlg dlg;
	int     itemNum=0;
	int     selectedItem=-1;
	CString temp;
	itemNum=m_List.GetItemCount();
	for(int i=0;i<(m_current_item==-1?itemNum:itemNum-1);i++)
	{
		if(m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED) selectedItem=i;
	}
	if(selectedItem==-1) return;	
	dlg.newMdl=p_InsInfo[selectedItem];
	dlg.strCaption="查看系列信息";
	dlg.SetMdlNumberAndCode(-1,"",NULL);
	dlg.DoModal();
}
