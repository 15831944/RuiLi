// TestRelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "TestRelDlg.h"
#include "Rel.h"

// CTestRelDlg �Ի���

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


// CTestRelDlg ��Ϣ�������
BOOL  CTestRelDlg::Calculate(CString strFormula,double * dblReValue)
{
	CMathString mathStr;
	CString    m_strError;
	mathStr.Initial(strFormula.GetBuffer());
	if (!mathStr.checkString())
	{
		(*dblReValue)=0.0;
		m_strError+=_T("���ʽ����");
		return FALSE;          //���ʽ����
	}
	mathStr.stringToBolan();
	if (mathStr.compvalue()!=0)
	{
		(*dblReValue)=0.0;
		m_strError+=_T("�����г���");
		return FALSE;         //�����г������ݷ���ֵ����һ��һ��ȷ��ԭ��
	}
	(*dblReValue)=mathStr.getvalue(); 
	return TRUE;
}
//�Թ�ϵʽ��������,��Ҫ���Ȼ���������Ա���
//�ѽ����Ĺ�ϵʽ,��������������������ı�������������,��ȡ����һ��
//ÿ��ֻ��һ��,����֮��,���ұߵ���,�����������ֵ����Ϊ������
//�����ظ�
BOOL  CTestRelDlg::SortRelSet()
{
	CArray<rel_swtx_item,rel_swtx_item&>	p_tempswtxlist;
	rel_swtx_item							temp_swtx;
	CArray<CRel,CRel&>						p_tempRelList;		//�Ⱥ����͵Ĺ�ϵʽ��
	CArray<CRel,CRel&>						p_tempRelList2;		//�ǵȺ����͵Ĺ�ϵ��
	p_tempswtxlist.Copy(p_Swtx);//����������Ա�,��Ϊ��ʼ��������
	p_RelListSorted.RemoveAll();
	for (int i=0;i<p_RelListUnSorted.GetCount();i++)
	{
		if (p_RelListUnSorted[i].m_iRelType==CRel::rel_type::REL_TYPE_EQUAL)
			p_tempRelList.Add(p_RelListUnSorted[i]);
		else
			p_tempRelList2.Add(p_RelListUnSorted[i]);
	}
	//�ԵȺ����͵Ĺ�ϵʽ����
	while (p_tempRelList.GetCount()>0)
	{
		int index=0;
		CArray<ProDimension,ProDimension&> p_dimlist;
		CArray<ProParameter,ProParameter&> p_paralist;
		SortRelSet(&p_tempRelList,&p_tempswtxlist,&index);
		p_RelListSorted.Add(p_tempRelList[index]);
		//��ù�ϵʽ��ߵĲ���
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
/* �Թ�ϵʽ�������򣬰����ұ߿ɱ��������صı�ֵ                       */
/************************************************************************/
BOOL  CTestRelDlg::SortRelSet(CArray<CRel,CRel&>	* p_InRelList
							  ,CArray<rel_swtx_item,rel_swtx_item&>  *p_MainParaList,int * index)
{
	double  *  flag;
	int    MaxIndex=0; //�����
	CArray<ProDimension,ProDimension&> p_dimlist;
	CArray<ProParameter,ProParameter&> p_paralist;
	flag=new double[(int)p_InRelList->GetCount()];      //��Сֵ��0.1������,��ֹ��ĸΪ0
	for (int i=0;i<p_InRelList->GetCount();i++)
	{
		p_dimlist.RemoveAll();
		p_paralist.RemoveAll();
		flag[i]=0.1;
		//��ϵ�еĵõ��ĳߴ�Ͳ������ܹ����ظ�
		p_InRelList->GetAt(i).GetRightDimList(&p_dimlist);
		p_InRelList->GetAt(i).GetRightParamList(&p_paralist);
		//�����ϵʽ����p_MainParaList�еĸ���
		for (int j=0;j<p_MainParaList->GetCount();j++)
		{
			switch(p_MainParaList->GetAt(j).type) {
			case 0:         //�ߴ�ĸ���
				for (int m=0;m<p_dimlist.GetCount();m++)
				{
					if (ComparaDimension(&(p_MainParaList->GetAt(j).dim),&(p_dimlist[m])))
						flag[i]++;
				}
				break;
			case 1:         //�����ĸ���
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
//ģ������
BOOL  CTestRelDlg::Regeneralte()
{
	return FALSE;
}
/************************************************************************/
/* ���ģ�͵Ĺ�ϵʽ���Լ��������Ա�                                     */
/************************************************************************/
void CTestRelDlg::GetSolidRelSetAndSwtxSet()
{
	//1.��ù�ϵʽ
	CArray<int,int&>	p_IndexList;
	rel_swtx_item		temp_swtx;
	ProParamvalue		proval;
	int                 postid;
	CArray<UsrSWTXTable1,UsrSWTXTable1> tempUsrSwtx;
	g_RelCollection.GetSolidAllRel(solid,true,&p_RelListUnSorted,&p_IndexList);
	//2.����������Ա�
	switch(GetMdlType(solid)) {
	case PRO_MDL_PART:
		for(int i=0;i<(int)p_UsrSWTXTableList.GetCount();i++)
		{
			//�����������Ա���
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
			//��鼰����
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
	//3.�����سߴ�.����
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
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString csValidCharacter="0123456789.";
	m_ListRel.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListSwtx.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListRv.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    m_ListRel.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	m_ListRel.InsertColumn(1,"��ϵʽ",LVCFMT_LEFT,150,-1);
	m_ListRel.InsertColumn(2,"����ʽ",LVCFMT_LEFT,300,-1);

	m_ListSwtx.InsertColumn(0,"����",LVCFMT_LEFT,140,-1);
	m_ListSwtx.InsertColumn(1,"��ʼֵ",LVCFMT_LEFT,80,-1);
	m_ListSwtx.InsertColumn(2,"���Ͳ���ֵ",LVCFMT_LEFT,80,-1);
    m_ListSwtx.SetReadOnlyColumns(2,false);
	m_ListSwtx.SetColumnValidEditCtrlCharacters(csValidCharacter,2);
	m_ListRv.InsertColumn(0,"����",LVCFMT_LEFT,140,-1);
	m_ListRv.InsertColumn(1,"��ʼֵ",LVCFMT_LEFT,80,-1);
	m_ListRv.InsertColumn(2,"���ͼ���ֵ",LVCFMT_LEFT,80,-1);
    //�õ���ϵʽ���ɱ��������سߴ��
	GetSolidRelSetAndSwtxSet();
	//��û������Ĺ�ϵʽ�������ĵ�����������У�����ʼ����ͬʱ���ƻ�û�е����������
	p_RelListSorted.Copy(p_RelListUnSorted);
	//���������Ա���ӵ��б���
	ShowSwtxList();
	//����سߴ硢������ӵ��б�
	ShowRvList();
	//��ʾ��ϵʽ�������ϵ��û�����������
	ShowRelList();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SortRelSet();
	ShowRelList();
}
/************************************************************************/
/* ����                                                                 */
/************************************************************************/
void CTestRelDlg::OnBnClickedCalBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������������Ա��ֵ
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
/*����                                                                  */
/************************************************************************/
void CTestRelDlg::OnBnClickedRegBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProError		status;
	status=ProSolidRegenerate(solid , PRO_REGEN_NO_FLAGS);
	switch(status) {
	case PRO_TK_NO_ERROR:
		AfxMessageBox("ģ������ͨ��");
		break;
	case PRO_TK_REGEN_AGAIN:
		AfxMessageBox("ģ��̫����,�޷�����");
		break;
	case PRO_TK_GENERAL_ERROR:
		AfxMessageBox("����,ģ��û�б��޸�");
		break;
	case PRO_TK_UNATTACHED_FEATS:
		AfxMessageBox("�����޹�����,ģ������ͨ��");
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
	// TODO: �ڴ����ר�ô����/����û���
	ResumeMdl();
	return CDialog::DestroyWindow();
}

void CTestRelDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    ResumeMdl();
	CDialog::OnClose();
}

void CTestRelDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ResumeMdl();
}
void CTestRelDlg::OnBnClose()
{
	ResumeMdl();
	SendMessage(WM_CLOSE);
}

