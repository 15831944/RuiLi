// TestRelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Main.h"
#include "TestRelDlg.h"
#include "Rel.h"

// CTestRelDlg 对话框

IMPLEMENT_DYNAMIC(CTestRelDlg, CDialog)
CTestRelDlg::CTestRelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestRelDlg::IDD, pParent),solid(NULL)
{
}

CTestRelDlg::~CTestRelDlg()
{
}

void CTestRelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListSwtx);
	DDX_Control(pDX, IDC_LIST3, m_ListRel);
	DDX_Control(pDX, IDC_LIST2, m_ListRv);
}


BEGIN_MESSAGE_MAP(CTestRelDlg, CDialog)
	ON_BN_CLICKED(IDC_SORT_BTN, OnBnClickedSortBtn)
	ON_BN_CLICKED(IDC_CAL_BTN, OnBnClickedCalBtn)
	ON_BN_CLICKED(IDC_REG_BTN, OnBnClickedRegBtn)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnNMClickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, OnNMClickList2)
	ON_BN_CLICKED(IDC_RESUME_BTN, OnBnClickedResumeBtn)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBnClose)
END_MESSAGE_MAP()


// CTestRelDlg 消息处理程序
BOOL  CTestRelDlg::Calculate(CString strFormula,double * dblReValue)
{
	CMathString mathStr;
	CString    m_strError;
	mathStr.Initial(strFormula.GetBuffer());
	if (!mathStr.checkString())
	{
		(*dblReValue)=0.0;
		m_strError+=_T("表达式有误");
		return FALSE;          //表达式有误
	}
	mathStr.stringToBolan();
	if (mathStr.compvalue()!=0)
	{
		(*dblReValue)=0.0;
		m_strError+=_T("计算中出错");
		return FALSE;         //计算中出错，根据返回值还可一进一步确定原因
	}
	(*dblReValue)=mathStr.getvalue(); 
	return TRUE;
}
//对关系式进行排序,需要首先获得事物特性表项
//把进来的关系式,根据主参数与非主参数的比例来进行排序,提取出第一个
//每次只排一个,出来之后,其右边的项,被计算出来的值将作为主参数
//不断重复
BOOL  CTestRelDlg::SortRelSet()
{
	CArray<rel_swtx_item,rel_swtx_item&>	p_tempswtxlist;
	rel_swtx_item							temp_swtx;
	CArray<CRel,CRel&>						p_tempRelList;		//等号类型的关系式组
	CArray<CRel,CRel&>						p_tempRelList2;		//非等号类型的关系组
	p_tempswtxlist.Copy(p_Swtx);//获得事物特性表,作为初始的主参数
	p_RelListSorted.RemoveAll();
	for (int i=0;i<p_RelListUnSorted.GetCount();i++)
	{
		if (p_RelListUnSorted[i].m_iRelType==CRel::rel_type::REL_TYPE_EQUAL)
			p_tempRelList.Add(p_RelListUnSorted[i]);
		else
			p_tempRelList2.Add(p_RelListUnSorted[i]);
	}
	//对等号类型的关系式排序
	while (p_tempRelList.GetCount()>0)
	{
		int index=0;
		CArray<ProDimension,ProDimension&> p_dimlist;
		CArray<ProParameter,ProParameter&> p_paralist;
		SortRelSet(&p_tempRelList,&p_tempswtxlist,&index);
		p_RelListSorted.Add(p_tempRelList[index]);
		//获得关系式左边的参数
		p_tempRelList[index].GetLeftDimList(&p_dimlist);
		p_tempRelList[index].GetLeftParamList(&p_paralist);
		for (int m=0;m<p_dimlist.GetCount();m++)
		{
			temp_swtx.type=0;
			temp_swtx.dim=p_dimlist[m];
			temp_swtx.value1=UsrGetDimParaValue(&p_dimlist[m],NULL);
			p_tempswtxlist.Add(temp_swtx);
		}
		for (int m=0;m<p_paralist.GetCount();m++)
		{
			temp_swtx.type=1;
			temp_swtx.para=p_paralist[m];
			temp_swtx.value1=UsrGetDimParaValue(NULL,&p_paralist[m]);
			p_tempswtxlist.Add(temp_swtx);
		}
		p_tempRelList.RemoveAt(index);
	}

	//////////////////////////////////////////////////////////////////////////
	p_RelListSorted.Append(p_tempRelList2);

	return FALSE;
}
/************************************************************************/
/* 对关系式进行排序，按照右边可变参数与相关的比值                       */
/************************************************************************/
BOOL  CTestRelDlg::SortRelSet(CArray<CRel,CRel&>	* p_InRelList
							  ,CArray<rel_swtx_item,rel_swtx_item&>  *p_MainParaList,int * index)
{
	double  *  flag;
	int    MaxIndex=0; //最大数
	CArray<ProDimension,ProDimension&> p_dimlist;
	CArray<ProParameter,ProParameter&> p_paralist;
	flag=new double[(int)p_InRelList->GetCount()];      //最小值用0.1来代替,防止分母为0
	for (int i=0;i<p_InRelList->GetCount();i++)
	{
		p_dimlist.RemoveAll();
		p_paralist.RemoveAll();
		flag[i]=0.1;
		//关系中的得到的尺寸和参数不能够有重复
		p_InRelList->GetAt(i).GetRightDimList(&p_dimlist);
		p_InRelList->GetAt(i).GetRightParamList(&p_paralist);
		//计算关系式中在p_MainParaList中的个数
		for (int j=0;j<p_MainParaList->GetCount();j++)
		{
			switch(p_MainParaList->GetAt(j).type) {
			case 0:         //尺寸的个数
				for (int m=0;m<p_dimlist.GetCount();m++)
				{
					if (ComparaDimension(&(p_MainParaList->GetAt(j).dim),&(p_dimlist[m])))
						flag[i]++;
				}
				break;
			case 1:         //参数的个数
				for (int m=0;m<p_paralist.GetCount();m++)
				{
					if (ComparaParameter(&(p_MainParaList->GetAt(j).para),&(p_paralist[m])))
						flag[i]++;
				}
				break;
			default:
				break;
			}
		}
		flag[i]=flag[i]/(p_dimlist.GetCount()+p_paralist.GetCount());
	}
	MaxIndex=0;
	for (int i=0;i<p_InRelList->GetCount();i++)
	{
		if (flag[MaxIndex]<flag[i])
		{
			MaxIndex=i;
		}
	}
	(*index)=MaxIndex;
	delete [] flag;
	return FALSE;
}
//模型重生
BOOL  CTestRelDlg::Regeneralte()
{
	return FALSE;
}
/************************************************************************/
/* 获得模型的关系式组以及事务特性表                                     */
/************************************************************************/
void CTestRelDlg::GetSolidRelSetAndSwtxSet()
{
	//1.获得关系式
	CArray<int,int&>	p_IndexList;
	rel_swtx_item		temp_swtx;
	ProParamvalue		proval;
	int                 postid;
	CArray<UsrSWTXTable1,UsrSWTXTable1> tempUsrSwtx;
	g_RelCollection.GetSolidAllRel(solid,true,&p_RelListUnSorted,&p_IndexList);
	//2.获得事物特性表
	switch(GetMdlType(solid)) {
	case PRO_MDL_PART:
		for(int i=0;i<(int)p_UsrSWTXTableList.GetCount();i++)
		{
			//更新事物特性表项
			p_UsrSWTXTableList[i].Update();
			if(p_UsrSWTXTableList[i].isCheck!=1)	    continue;
			if(p_UsrSWTXTableList[i].owner!=solid)		continue;
			if (p_UsrSWTXTableList[i].type=="dim")
			{
				temp_swtx.type=0;
				temp_swtx.dim=p_UsrSWTXTableList[i].Dim;
				temp_swtx.name=p_UsrSWTXTableList[i].Name;
				temp_swtx.value2=temp_swtx.value1=UsrGetDimParaValue(&(temp_swtx.dim),NULL);
			}
			else if (p_UsrSWTXTableList[i].type=="para" )
			{
				ProParameterValueGet(&p_UsrSWTXTableList[i].Para,&proval);
				if(proval.type	!=PRO_PARAM_DOUBLE && proval.type	!=PRO_PARAM_INTEGER) continue;
				temp_swtx.type=1;
				temp_swtx.para	=	p_UsrSWTXTableList[i].Para;
				temp_swtx.name	=	p_UsrSWTXTableList[i].Name;
				temp_swtx.value2=	temp_swtx.value1=UsrGetDimParaValue(NULL,&(temp_swtx.para));
			}
			p_Swtx.Add(temp_swtx);
		}
		break;
	case PRO_MDL_ASSEMBLY:
		for (int i=0;i<p_AsmSmlItemList.GetCount();i++) 
		{
			//检查及过滤
			if(p_AsmSmlItemList[i].owner!=solid) continue;
			if (!CheckAsmSWTX(solid,&p_AsmSmlItemList[i]))
			{
				p_AsmSmlItemList.RemoveAt(i,1);
				i--;
				continue;
			}
			temp_swtx.name=p_AsmSmlItemList[i].name;
			switch(p_AsmSmlItemList[i].marsk) {
			case TYPE_DIM:
				temp_swtx.type=0;
				temp_swtx.dim=p_AsmSmlItemList[i].dim;
				temp_swtx.value2=temp_swtx.value1=UsrGetDimParaValue(&(temp_swtx.dim),NULL);
				break;
			case TYPE_PARA:
				ProParameterValueGet(&p_AsmSmlItemList[i].para,&proval);
				if(proval.type != PRO_PARAM_DOUBLE && proval.type != PRO_PARAM_INTEGER) continue;
				temp_swtx.type=1;
				temp_swtx.para=p_AsmSmlItemList[i].para;
				temp_swtx.value2=temp_swtx.value1=UsrGetDimParaValue(NULL,&(temp_swtx.para));
				break;
			default:
				continue;
				break;
			}
			p_Swtx.Add(temp_swtx);
		}
		break;
	default:
		break;
	}
	//3.获得相关尺寸.参数
	g_RelCollection.GetSolidAllRelDimPara(solid,tempUsrSwtx);
	for (int i=0;i<tempUsrSwtx.GetCount();i++)
	{
		int flag=0;
		for (int j=0;j<p_Swtx.GetCount();j++)
		{
			if (tempUsrSwtx[i].type=="dim" 
				&& p_Swtx[j].type==0 && ComparaDimension(&tempUsrSwtx[i].Dim,&p_Swtx[j].dim))
			{
				flag=1;
				break;
			}
			else if (tempUsrSwtx[i].type=="para" 
				&& p_Swtx[j].type == 1 && ComparaParameter(&tempUsrSwtx[i].Para,&p_Swtx[j].para))
			{
				flag=1;
				break;
			}
		}
		if(flag==0)
		{
			if (tempUsrSwtx[i].type=="dim")
			{
				temp_swtx.type=0;
				if(GetMdlType(GetCurrentMdl())==PRO_MDL_ASSEMBLY) 
				{
					ProSolidToPostfixId((ProSolid)tempUsrSwtx[i].Dim.owner,&postid);
					temp_swtx.name.Format("%s:%d",tempUsrSwtx[i].Name,postid);
				}
				else
				{
					temp_swtx.name=tempUsrSwtx[i].Name;
				}				
				temp_swtx.dim=tempUsrSwtx[i].Dim;
				temp_swtx.value1=temp_swtx.value2=UsrGetDimParaValue(&temp_swtx.dim,NULL);
				p_Rv.Add(temp_swtx);
			}
			else if (tempUsrSwtx[i].type=="para")
			{				
				temp_swtx.type=1;
				if(GetMdlType(GetCurrentMdl())==PRO_MDL_ASSEMBLY) 
				{
					ProSolidToPostfixId(UsrGetParameterOwner(&tempUsrSwtx[i].Para) ,&postid);
					temp_swtx.name.Format("%s:%d",tempUsrSwtx[i].Name,postid);
				}
				else
				{
					temp_swtx.name=tempUsrSwtx[i].Name;
				}
				temp_swtx.para=tempUsrSwtx[i].Para;
				temp_swtx.value1=temp_swtx.value2=UsrGetDimParaValue(NULL,&temp_swtx.para);
				p_Rv.Add(temp_swtx);
			}
		}
	}
}
BOOL CTestRelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化
	CString csValidCharacter="0123456789.";
	m_ListRel.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListSwtx.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListRv.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    m_ListRel.InsertColumn(0,"编号",LVCFMT_LEFT,40,-1);
	m_ListRel.InsertColumn(1,"关系式",LVCFMT_LEFT,150,-1);
	m_ListRel.InsertColumn(2,"叠代式",LVCFMT_LEFT,300,-1);

	m_ListSwtx.InsertColumn(0,"名称",LVCFMT_LEFT,140,-1);
	m_ListSwtx.InsertColumn(1,"初始值",LVCFMT_LEFT,80,-1);
	m_ListSwtx.InsertColumn(2,"变型测试值",LVCFMT_LEFT,80,-1);
    m_ListSwtx.SetReadOnlyColumns(2,false);
	m_ListSwtx.SetColumnValidEditCtrlCharacters(csValidCharacter,2);
	m_ListRv.InsertColumn(0,"名称",LVCFMT_LEFT,140,-1);
	m_ListRv.InsertColumn(1,"初始值",LVCFMT_LEFT,80,-1);
	m_ListRv.InsertColumn(2,"变型计算值",LVCFMT_LEFT,80,-1);
    //得到关系式，可变参数、相关尺寸等
	GetSolidRelSetAndSwtxSet();
	//把没有排序的关系式，拷贝的到排序的数组中，来初始化，同时不破坏没有的排序的数组
	p_RelListSorted.Copy(p_RelListUnSorted);
	//把事物特性表添加到列表中
	ShowSwtxList();
	//把相关尺寸、参数添加到列表
	ShowRvList();
	//显示关系式，这个关系是没有排序情况下
	ShowRelList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
int CTestRelDlg::ShowRelList(void)
{
	CString   csIndex;
	int       index;
	CString   csRel;
	m_ListRel.DeleteAllItems();
	for (int i=0;i<p_RelListSorted.GetCount();i++)
	{
		csRel.Empty();
		switch(GetMdlType(GetCurrentMdl())) {
		case PRO_MDL_PART:
			p_RelListSorted[i].GetRelString(csRel);
			break;
		case PRO_MDL_ASSEMBLY:
			p_RelListSorted[i].GetRelStringAsComp(csRel);
			break;
		default:
			return 0;
			break;
		}		
		csIndex.Format("%d",m_ListRel.GetItemCount()+1);
		index=m_ListRel.InsertItem(LVIF_TEXT|LVIF_PARAM,m_ListRel.GetItemCount(),csIndex,0,0,0,i);
		m_ListRel.SetItemText(index,1,csRel);
		m_ListRel.SetItemText(index,2,"");
	}
	return 0;
}


int CTestRelDlg::ShowSwtxList(void)
{
	int       index;
	CString   csValue1;
	CString   csValue2;
	m_ListSwtx.DeleteAllItems();
	for (int i=0;i<p_Swtx.GetCount();i++)
	{
		csValue1.Format("%f",p_Swtx[i].value1);
		csValue2.Format("%f",p_Swtx[i].value2);
		index=m_ListSwtx.InsertItem(LVIF_TEXT|LVIF_PARAM,m_ListSwtx.GetItemCount(),p_Swtx[i].name,0,0,0,i);
		m_ListSwtx.SetItemText(index,1,csValue1);
		m_ListSwtx.SetItemText(index,2,csValue2);
	}
	return 0;
}

int CTestRelDlg::ShowRvList(void)
{
	int       index;
	CString   csValue1;
	CString   csValue2;
	m_ListRv.DeleteAllItems();
	for (int i=0;i<p_Rv.GetCount();i++)
	{
		csValue1.Format("%f",p_Rv[i].value1);
		csValue2.Format("%f",p_Rv[i].value2);
		index=m_ListRv.InsertItem(LVIF_TEXT|LVIF_PARAM,m_ListRv.GetItemCount(),p_Rv[i].name,0,0,0,i);
		m_ListRv.SetItemText(index,1,csValue1);
		m_ListRv.SetItemText(index,2,csValue2);
	}
	return 0;
}

void CTestRelDlg::OnBnClickedSortBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	SortRelSet();
	ShowRelList();
}
/************************************************************************/
/* 计算                                                                 */
/************************************************************************/
void CTestRelDlg::OnBnClickedCalBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	//获得输入的事物特性表的值
	double   dbvalue=0.0;
	for (int i=0;i<p_Swtx.GetCount();i++)
	{
		p_Swtx[i].value2=atof(m_ListSwtx.GetItemText(i,2));
		switch(p_Swtx[i].type) {
		case 0:
			SetDimParamterValue(&p_Swtx[i].dim,NULL,p_Swtx[i].value2);
			break;
		case 1:
			SetDimParamterValue(NULL,&p_Swtx[i].para,p_Swtx[i].value2);
			break;
		default:
			break;
		}
	}
	for (int i=0;i<p_RelListSorted.GetCount();i++)
	{
		CArray<ProDimension,ProDimension&> p_dimlist;
		CArray<ProParameter,ProParameter&> p_paralist;
		Calculate(p_RelListSorted[i].GetRelRightFormula(),&dbvalue);
        p_RelListSorted[i].GetLeftDimList(&p_dimlist);
		p_RelListSorted[i].GetLeftParamList(&p_paralist);
		for (int m=0;m<p_dimlist.GetCount();m++)
		{
			SetDimParamterValue(&p_dimlist[m],NULL,dbvalue);
		}
		for (int m=0;m<p_paralist.GetCount();m++)
		{
			SetDimParamterValue(NULL,&p_paralist[m],dbvalue);
		}
		m_ListRel.SetItemText(i,2,p_RelListSorted[i].GetRelFormula());
	}
	for (int i=0;i<p_Rv.GetCount();i++)
	{
		switch(p_Rv[i].type) {
		case 0:
			p_Rv[i].value2=UsrGetDimParaValue(&p_Rv[i].dim,NULL);
			break;
		case 1:
			p_Rv[i].value2=UsrGetDimParaValue(NULL,&p_Rv[i].para);
			break;
		default:
			break;
		}
	}
	ShowRvList();
}
/************************************************************************/
/*再生                                                                  */
/************************************************************************/
void CTestRelDlg::OnBnClickedRegBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	ProError		status;
	status=ProSolidRegenerate(solid , PRO_REGEN_NO_FLAGS);
	switch(status) {
	case PRO_TK_NO_ERROR:
		AfxMessageBox("模型再生通过");
		break;
	case PRO_TK_REGEN_AGAIN:
		AfxMessageBox("模型太复杂,无法重生");
		break;
	case PRO_TK_GENERAL_ERROR:
		AfxMessageBox("出错,模型没有被修改");
		break;
	case PRO_TK_UNATTACHED_FEATS:
		AfxMessageBox("发现无关特征,模型再生通过");
		break;
	default:
		break;
	}
	if (status!=PRO_TK_NO_ERROR && status!=PRO_TK_UNATTACHED_FEATS)
		ResumeMdl();
}
BOOL  CTestRelDlg::SetDimParamterValue(ProDimension * dim=NULL,ProParameter * para=NULL,double value=0.0)
{
	ProParamvalue     proval;
	int				  iValue=(int)value;
	if (dim!=NULL)
	{
		ProDimensionValueSet(dim,value);
	}
	if (para!=NULL)
	{
		ProParameterValueGet(para,&proval);
		switch(proval.type) {
		case PRO_PARAM_DOUBLE:
			proval.value.d_val=value;
			break;
		case PRO_PARAM_INTEGER:
			proval.value.i_val=iValue;
			break;
		default:
			break;
		}
		ProParameterValueSet(para,&proval);
	}
	return TRUE;
}
void CTestRelDlg::ResumeMdl(void)
{
	for (int i=0;i<p_Swtx.GetCount();i++)
	{
		switch(p_Swtx[i].type) {
		case 0:
			SetDimParamterValue(&p_Swtx[i].dim,NULL,p_Swtx[i].value1);
			break;
		case 1:
			SetDimParamterValue(NULL,&p_Swtx[i].para,p_Swtx[i].value1);
			break;
		default:
			break;
		}
	}
	for (int i=0;i<p_Rv.GetCount();i++)
	{
		switch(p_Rv[i].type) {
		case 0:
			SetDimParamterValue(&p_Rv[i].dim,NULL,p_Rv[i].value1);
				break;
		case 1:
			SetDimParamterValue(NULL,&p_Rv[i].para,p_Rv[i].value1);
				break;
		default:
			break;
		}
	}
	ProSolidRegenerate(solid , PRO_REGEN_NO_FLAGS);
}
BOOL CTestRelDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ResumeMdl();
	return CDialog::DestroyWindow();
}

void CTestRelDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    ResumeMdl();
	CDialog::OnClose();
}

void CTestRelDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_ListSwtx.GetFirstSelectedItemPosition();
	int nItem = m_ListSwtx.GetNextSelectedItem(pos);
	if (nItem>=0)
	{
		if (p_Swtx[nItem].type==0)
		{
			UsrShowDim(solid,&p_Swtx[nItem].dim);
		}
	}
	*pResult = 0;
}

void CTestRelDlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_ListRv.GetFirstSelectedItemPosition();
	int nItem = m_ListRv.GetNextSelectedItem(pos);
	if (nItem>=0)
	{
		if (p_Rv[nItem].type==0)
		{
			UsrShowDim(solid,&p_Rv[nItem].dim);
		}
	}
	*pResult = 0;
}

void CTestRelDlg::OnBnClickedResumeBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	ResumeMdl();
}
void CTestRelDlg::OnBnClose()
{
	ResumeMdl();
	SendMessage(WM_CLOSE);
}

