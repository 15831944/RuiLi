// ParaOperaDlg.cpp : ʵ���ļ�
//���ڶ�ģ���еĲ������и�������ĶԻ���
//���ӣ�ɾ�����޸Ĳ���

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
// CParaOperaDlg �Ի���

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


// CParaOperaDlg ��Ϣ�������


BOOL CParaOperaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
		solid_list.Add((ProSolid)model);                     //�������װ��ģ�ͱ���ľ��
		GetAsmCompHand((ProSolid)model,&solid_list);          //���Ԫ���ľ��
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
	// �쳣��OCX ����ҳӦ���� FALSE
}

/*=========================================================================*\
Function:	ParamsGetGet() ��õ�ǰģ�����ȫ����������
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
/* �б����ʾ�����                                                     */
/************************************************************************/
void CParaOperaDlg::ListLabelsSet(void)
{
	
	int           num=0;
	int           indexNo=0;
	ProCharLine   strtmp;
	//��õ�ǰģ�����в���
	p_data.RemoveAll();
    //�жϵ�ǰģ�����ͣ���������������������
	//�����������������ȷ��ʳ������Ԫ����Ȼ��õ�����Ԫ���Ĳ���
	ParamsGet();
    num=(int)p_data.GetCount();
	m_List.DeleteAllItems();
	m_List.DeleteColumn(0);
	m_List.DeleteColumn(0);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_List.InsertColumn(0,"���",LVCFMT_LEFT,40,-1);
	m_List.InsertColumn(1,"����",LVCFMT_LEFT,80,-1);

	//�����и�Ԫ�س�ʼ��
	for (int i=0;i<num;i++){
   
		sprintf(strtmp,"%d",i+1);
		indexNo=m_List.InsertItem(LVIF_TEXT|LVIF_PARAM,num,strtmp,0,0,0,i);
		//��ò�����
		ProWstringToString(strtmp, p_data[i].id);
        m_List.SetItemText(indexNo,1,strtmp);
	}

}
/*=========================================================================*\
Function:	ParameterActionFn() �������ʶ�������
\*=========================================================================*/
ProError ParameterActionFn(ProParameter *Param,ProError status,
						   CArray<ProParameter,ProParameter>* p_pdata)   
{   

	p_pdata->Add(*Param);
	return (PRO_TK_NO_ERROR);
}

/*=========================================================================*\
Function:	ParameterFilterFn() �������ʹ��˺���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}

/************************************************************************/
/* �����б�򣬽���ѡ��                                                 */
/************************************************************************/
void CParaOperaDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			//csValue="Ĭ��";
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
/* ɾ������                                                                     */
/************************************************************************/
void CParaOperaDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char name[32];
	CString csName;

	UpdateData(TRUE);
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		   ProWstringToString(name,p_data[m_List.GetItemData(i)].id);
		   csName.Format("%s",name);
		   if(csName==m_Name.GetString()) 
		   {
			   if(ProParameterDelete(&p_data[m_List.GetItemData(i)])!=PRO_TK_NO_ERROR) AfxMessageBox("���������ڹ�ϵʽ���ж���,ɾ�����ɹ���");
			   break;
		   }
           
	}
	ListLabelsSet();
}

/************************************************************************/
/* ��Ӳ���                                                             */
/************************************************************************/
void CParaOperaDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
	//�ж��Ƿ��Ѿ�����
	for(int i=0;i<m_List.GetItemCount();i++)
	{
		if(m_List.GetItemText(i,1)==m_Name) 
		{
			AfxMessageBox("�Ѿ����ڣ�");
			return;
		}
	}
    //����
	sprintf(name,"%s",m_Name);
	ProStringToWstring(s_value,name);
    //���ͣ�ֵ
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
			AfxMessageBox("������ֵ����ȷ��ӦΪYES/NO");
			return;
		} 
		break;
	default:
		AfxMessageBox("û��ѡ�����ͣ�");
		m_type=-1;
		UpdateData(FALSE);
		return;
		break;
	}
	//�����ƽ��м��
	if(m_Name.FindOneOf("*&^%$#@!~()-=+\\|/[]{}'';:?>,.>�������򣡡��������������죫���������������������£�������")!=-1)
	{
		AfxMessageBox("���ڲ�֧�ֵķ��ţ��������ɹ���");
		return;
	}
    
	if(ProParameterCreate(&modelitem,s_value,&proval, &param)!=PRO_TK_NO_ERROR) AfxMessageBox("����ʧ�ܣ�");
	sprintf(name,"%d",m_type);

	m_type=-1;
	UpdateData(FALSE);
	ListLabelsSet();
}

void CParaOperaDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
}

/************************************************************************/
/* �������޸ģ�����                                                     */
/************************************************************************/
void CParaOperaDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
    //�������
	ProStringToWstring(ParamName,strtmp);
	//������ʼ�������ֵָ��
	ProParameterInit(&modelitem,ParamName,&param);
    sprintf(strtmp,"%s",_T(m_Value));
	ProParameterValueGet(&param, &value);
	switch( value.type )
	{
	case PRO_PARAM_DOUBLE:
		//���ֵ�Ƿ�Ϊ����		
		value.value.d_val=atof(strtmp);
		break;
	case PRO_PARAM_INTEGER:
		value.value.i_val=atoi(strtmp);
		break;
	case PRO_PARAM_STRING:
		ProStringToWstring(value.value.s_val,strtmp);
		break;
	}    

	//�µĲ���ֵ����
	ProParameterValueSet(&param, &value);
	ProSolidRegenerate(solid_list[m_ComboBox.GetCurSel()],PRO_REGEN_NO_FLAGS);
	int window_id;
	if(ProWindowCurrentGet(&window_id)==PRO_TK_NO_ERROR)  ProWindowRepaint(window_id);
}

void CParaOperaDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);


	// TODO: �ڴ������Ϣ����������
	UsrRepaintWindow();
}
/************************************************************************/
/* �˳�����                                                             */
/************************************************************************/
void CParaOperaDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CParaOperaDlg::OnCancel();
}

void CParaOperaDlg::OnCbnSelchangeCombo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ListLabelsSet();  
}
/************************************************************************/
/* ��������ַ�����Ч��                                                 */
/************************************************************************/
BOOL CParaOperaDlg::CheckValidCString(void)
{
	//CString m_strValidChars;
	//if(m_Name.FindOneOf("*&^%$#@!~()-=+\\|/[]{}'';:?>,.>�������򣡡��������������죫���������������������£�������")!=-1)
	//{
	//	AfxMessageBox("���ڲ�֧�ֵķ��ţ��������ɹ���");
	//	return FALSE;
	//}
	//switch(m_type) {
	//case 0:  //DOUBLE��
	//	m_strValidChars=_T("0123456789.");
	//	break;
	//case 1:  //����
	//	m_strValidChars=_T("0123456789");
	//	break;
	//case 2:  //�ַ���
	//	m_strValidChars=_T("");
	//	break;
	//case 3:  //������
	//	m_strValidChars=_T("YESNO");
	//	break;
	//default: //
	//	break;
	//}
	//if(m_Value.Find(m_strValidChars,0));
	//m_ValueEdit.m_strValidChars=m_strValidChars;
	return FALSE;
}
