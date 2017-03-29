// ParaOperaDlg.cpp : 实现文件
//用于对模型中的参数进行各项操作的对话框
//增加，删除，修改参数

#include "stdafx.h"
#include "Main.h"
#include "ParaOperaDlg.h"
extern int DlgIcon;
CArray<ProParameter,ProParameter> p_data;
ProError ParameterActionFn(ProParameter *Param,ProError status,
						   CArray<ProParameter,ProParameter>* p_pdata) ;
ProError ParameterFilterFn(ProParameter *Param,ProError status,
						   CArray<ProParameter,ProParameter>* p_pdata) ;
void  GetAsmCompHand(ProSolid solid,CArray<ProSolid,ProSolid> * p_solidlist);
// CParaOperaDlg 对话框

IMPLEMENT_DYNAMIC(CParaOperaDlg, CDialog)
CParaOperaDlg::CParaOperaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParaOperaDlg::IDD, pParent)
	, m_Name(_T(""))
	, m_Value(_T(""))
	, m_type(-1)
{
}

CParaOperaDlg::~CParaOperaDlg()
{
}

void CParaOperaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT1, m_Name);
	DDX_Text(pDX, IDC_EDIT2, m_Value);
	DDX_CBIndex(pDX, IDC_COMBO1, m_type);
	DDX_Control(pDX, IDC_COMBO2, m_ComboBox);
	DDX_Control(pDX, IDC_EDIT2, m_ValueEdit);
}


BEGIN_MESSAGE_MAP(CParaOperaDlg, CDialog)
	
	ON_NOTIFY(HDN_ITEMCLICK, IDC_LIST1, OnHdnItemclickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
//	ON_WM_CHAR()
END_MESSAGE_MAP()


// CParaOperaDlg 消息处理程序


BOOL CParaOperaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	int          num=0;
	ProMdl       model;
	ProName      modelname;
	ProMdlType   type;
	ProMdlCurrentGet(&model);
	ProMdlTypeGet(model,&type);
	switch(type) {
	case PRO_MDL_PART:
		solid_list.Add((ProSolid)model);
		ProMdlNameGet(model,modelname);
		m_ComboBox.InsertString(m_ComboBox.GetCount(),_T(CString(modelname)+".PRT"));
		break;
	case PRO_MDL_ASSEMBLY:
		solid_list.Add((ProSolid)model);                     //首先添加装配模型本身的句柄
		GetAsmCompHand((ProSolid)model,&solid_list);          //获得元件的句柄
		num=(int)solid_list.GetCount();
		for (int i=0;i<num;i++) {
            ProMdlNameGet((ProMdl)solid_list[i],modelname);
			ProMdlTypeGet((ProMdl)solid_list[i],&type);
			if (type==PRO_MDL_PART)  m_ComboBox.InsertString(m_ComboBox.GetCount(),_T(CString(modelname)+".PRT"));
			else m_ComboBox.InsertString(m_ComboBox.GetCount(),_T(CString(modelname)+".ASM"));			
		}
		break;
	default:
		break;
	}
	m_ComboBox.SetCurSel(0);
	ListLabelsSet();    
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

/*=========================================================================*\
Function:	ParamsGetGet() 获得当前模型项的全部参数对象
\*=========================================================================*/
void CParaOperaDlg::ParamsGet()
{
	ProModelitem    modelitem;
	ProMdlToModelitem((ProMdl)solid_list[m_ComboBox.GetCurSel()], &modelitem);
	ProParameterVisit(&modelitem,
		(ProParameterFilter)ParameterFilterFn,
		(ProParameterAction )ParameterActionFn ,(CArray<ProParameter,ProParameter>*)&p_data); 
}
/************************************************************************/
/* 列表框显示与更新                                                     */
/************************************************************************/
void CParaOperaDlg::ListLabelsSet(void)
{
	
	int           num=0;
	int           indexNo=0;
	ProCharLine   strtmp;
	//获得当前模型所有参数
	p_data.RemoveAll();
    //判断当前模型类型，如果是零件，则访问零件的
	//，如果是组件，则首先访问出组件的元件，然后得到各个元件的参数
	ParamsGet();
    num=(int)p_data.GetCount();
	m_List.DeleteAllItems();
	m_List.DeleteColumn(0);
	m_List.DeleteColumn(0);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"序号",LVCFMT_LEFT,40,-1);
	m_List.InsertColumn(1,"名称",LVCFMT_LEFT,80,-1);

	//数组中各元素初始化
	for (int i=0;i<num;i++){
   
		sprintf(strtmp,"%d",i+1);
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,num,strtmp,0,0,0,i);
		//获得参数名
		ProWstringToString(strtmp, p_data[i].id);
        m_List.SetItemText(indexNo,1,strtmp);
	}

}
/*=========================================================================*\
Function:	ParameterActionFn() 参数访问动作函数
\*=========================================================================*/
ProError ParameterActionFn(ProParameter *Param,ProError status,
						   CArray<ProParameter,ProParameter>* p_pdata)   
{   

	p_pdata->Add(*Param);
	return (PRO_TK_NO_ERROR);
}

/*=========================================================================*\
Function:	ParameterFilterFn() 参数访问过滤函数
\*=========================================================================*/
ProError ParameterFilterFn(ProParameter *Param,ProError status,
						  CArray<ProParameter,ProParameter>* p_pdata)   
{   
	//ProCharLine strtmp;
	//ProWstringToString(strtmp, Param->id);
	//if (strcmp(strtmp,"DESCRIPTION")==0 ||strcmp(strtmp,"MODELED_BY")==0 )
	//	return (PRO_TK_CONTINUE);
	//else
		return (PRO_TK_NO_ERROR);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CParaOperaDlg::OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}

/************************************************************************/
/* 单击列表框，进行选择                                                 */
/************************************************************************/
void CParaOperaDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		if(m_List.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED)
		{
			ProCharName name;
			ProParamvalue proval;
			//ProCharLine str_line;
			//wchar_t str_line[80];

			ProWstringToString(name,p_data[m_List.GetItemData(i)].id);

			m_Name.Format("%s",name);
			ProParameterValueGet(&p_data[m_List.GetItemData(i)],&proval);

			switch(proval.type) {
		case PRO_PARAM_DOUBLE:
			m_Value.Format("%0f",proval.value.d_val);
			m_type=0;
			break;
		case PRO_PARAM_STRING:

			//ProWstringToString(str_line,proval.value.s_val);
			//wsprintf(str_line,"%S",proval.value.s_val);
            m_Value=CString(proval.value.s_val);
			//m_Value.Format("%s",str_line);
			m_type=2;
			break;
		case PRO_PARAM_INTEGER:

			m_Value.Format("%0d",proval.value.i_val);
			m_type=1;
			break;
		case PRO_PARAM_BOOLEAN:
			if(proval.value.l_val==1)
			{m_Value="YES";}
			if(proval.value.l_val==0)
			{m_Value="NO";}
			//m_Value.Format("%0d",proval.value.l_val);
			m_type=3;
			break;
		default:
			//csValue="默认";
			m_type=-1;
			break;

			}
			UpdateData(FALSE);
			break;

		}
	}
	*pResult = 0;
}
/************************************************************************/
/* 删除操作                                                                     */
/************************************************************************/
void CParaOperaDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	char name[32];
	CString csName;

	UpdateData(TRUE);
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		   ProWstringToString(name,p_data[m_List.GetItemData(i)].id);
		   csName.Format("%s",name);
		   if(csName==m_Name.GetString()) 
		   {
			   if(ProParameterDelete(&p_data[m_List.GetItemData(i)])!=PRO_TK_NO_ERROR) AfxMessageBox("参数可能在关系式中有定义,删除不成功！");
			   break;
		   }
           
	}
	ListLabelsSet();
}

/************************************************************************/
/* 添加操作                                                             */
/************************************************************************/
void CParaOperaDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	
	char name[32];                           //ProCharName
	wchar_t s_value[PRO_NAME_SIZE];
	char strtmpv[81];                        //ProCharLine
	ProParameter param;
	ProParamvalue proval;
	//ProMdl   model;
	ProModelitem modelitem;
	//ProMdlCurrentGet(&model);
	ProMdlToModelitem((ProMdl)solid_list[m_ComboBox.GetCurSel()],&modelitem);
	//判断是否已经存在
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		if(m_List.GetItemText(i,1)==m_Name) 
		{
			AfxMessageBox("已经存在！");
			return;
		}
	}
    //名称
	sprintf(name,"%s",m_Name);
	ProStringToWstring(s_value,name);
    //类型，值
	sprintf(strtmpv,"%s",m_Value);
	switch(m_type) {
	case 0:
		proval.type=PRO_PARAM_DOUBLE;
		proval.value.d_val=atof(strtmpv);
		break;
	case 1:
		proval.type=PRO_PARAM_INTEGER;
		proval.value.i_val=atoi(strtmpv);
		break;
	case 2:
		proval.type=PRO_PARAM_STRING;
		ProStringToWstring(proval.value.s_val,strtmpv);
		break;
	case 3:
		proval.type=PRO_PARAM_BOOLEAN;
		if(m_Value=="YES")
		{
			proval.value.l_val=1;
		}
		else if (m_Value=="NO") {
			proval.value.l_val=0;
		}
		else  {
			AfxMessageBox("布尔型值不正确，应为YES/NO");
			return;
		} 
		break;
	default:
		AfxMessageBox("没有选择类型！");
		m_type=-1;
		UpdateData(FALSE);
		return;
		break;
	}
	//对名称进行检查
	if(m_Name.FindOneOf("*&^%$#@!~()-=+\\|/[]{}'';:?>,.>※￥＃◎！～……（）——§＋『』【】“‘：；？、÷，。《》")!=-1)
	{
		AfxMessageBox("存在不支持的符号，创建不成功！");
		return;
	}
    
	if(ProParameterCreate(&modelitem,s_value,&proval, &param)!=PRO_TK_NO_ERROR) AfxMessageBox("创建失败！");
	sprintf(name,"%d",m_type);

	m_type=-1;
	UpdateData(FALSE);
	ListLabelsSet();
}

void CParaOperaDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码	
}

/************************************************************************/
/* 再生（修改）操作                                                     */
/************************************************************************/
void CParaOperaDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	ProName ParamName;
	ProParameter param;
	//ProMdl   model;
	ProModelitem modelitem;
	ProParamvalue value;
	ProCharLine strtmp;
	//ProMdlCurrentGet(&model);
	ProMdlToModelitem((ProMdl)solid_list[m_ComboBox.GetCurSel()], &modelitem);
    UpdateData(TRUE);
	sprintf(strtmp,"%s",_T(m_Name));
    //获得名称
	ProStringToWstring(ParamName,strtmp);
	//参数初始化，获得值指针
	ProParameterInit(&modelitem,ParamName,&param);
    sprintf(strtmp,"%s",_T(m_Value));
	ProParameterValueGet(&param, &value);
	switch( value.type )
	{
	case PRO_PARAM_DOUBLE:
		//检查值是否为数字		
		value.value.d_val=atof(strtmp);
		break;
	case PRO_PARAM_INTEGER:
		value.value.i_val=atoi(strtmp);
		break;
	case PRO_PARAM_STRING:
		ProStringToWstring(value.value.s_val,strtmp);
		break;
	}    

	//新的参数值设置
	ProParameterValueSet(&param, &value);
	ProSolidRegenerate(solid_list[m_ComboBox.GetCurSel()],PRO_REGEN_NO_FLAGS);
	int window_id;
	if(ProWindowCurrentGet(&window_id)==PRO_TK_NO_ERROR)  ProWindowRepaint(window_id);
}

void CParaOperaDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);


	// TODO: 在此添加消息处理程序代码
	UsrRepaintWindow();
}
/************************************************************************/
/* 退出操作                                                             */
/************************************************************************/
void CParaOperaDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CParaOperaDlg::OnCancel();
}

void CParaOperaDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	ListLabelsSet();  
}
/************************************************************************/
/* 检查输入字符的有效性                                                 */
/************************************************************************/
BOOL CParaOperaDlg::CheckValidCString(void)
{
	//CString m_strValidChars;
	//if(m_Name.FindOneOf("*&^%$#@!~()-=+\\|/[]{}'';:?>,.>※￥＃◎！～……（）——§＋『』【】“‘：；？、÷，。《》")!=-1)
	//{
	//	AfxMessageBox("存在不支持的符号，创建不成功！");
	//	return FALSE;
	//}
	//switch(m_type) {
	//case 0:  //DOUBLE型
	//	m_strValidChars=_T("0123456789.");
	//	break;
	//case 1:  //整型
	//	m_strValidChars=_T("0123456789");
	//	break;
	//case 2:  //字符型
	//	m_strValidChars=_T("");
	//	break;
	//case 3:  //布尔型
	//	m_strValidChars=_T("YESNO");
	//	break;
	//default: //
	//	break;
	//}
	//if(m_Value.Find(m_strValidChars,0));
	//m_ValueEdit.m_strValidChars=m_strValidChars;
	return FALSE;
}
