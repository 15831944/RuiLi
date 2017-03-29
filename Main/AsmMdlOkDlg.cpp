// AsmMdlOkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "AsmMdlOkDlg.h"
//#include "CAsmMakeupSet.h"
#include "NAsmBomTable.h"
#include "NAsmComListTable.h"
#include "NAsmType.h"
#include "CAsmVarSet.h"
#include "CAsmPerfSet.h"
#include "CAsmRelSet.h"
#include "CAsmRvRelDimParaSet.h"
#include "CAsmPicSet.h"
#include "FtpFile.h"
#include "CAsmCompVariantTableSet.h"
#include "CAsmDrwTableSet.h"
#include "checkmdlifhaschange.h"
/************************************************************************/
/* �ύ������
1.���е��㲿�����Ǿ�����˵�
2.ģ��Ҳ�Ǿ�����˵�
3.���е��㲿�������ύ���

  �ύ�Ĺ���
1.������Ϣ��
2.�������Ա�
3.���ܲ�����
4.��ɱ�
5.�嵥
6.����ģ�͵���������ȥ
  --|
    |-drawing �ļ���  ��Ź���ͼֽ
	|-�齨�ļ� .asm   ������ϲ��
	|-Backup          ���еı���  ��ֹ�����㲿�������ڵ�ʱ����޷�����ģ��
	     |-Drawing    �����㲿���Ĺ���ͼ
		 |-���е��ļ� */
/************************************************************************/
// CAsmMdlOkDlg �Ի���

IMPLEMENT_DYNAMIC(CAsmMdlOkDlg, CDialog)
CAsmMdlOkDlg::CAsmMdlOkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsmMdlOkDlg::IDD, pParent)
	, asm_num(-1)
	, fileDirectory("")
	, fileLodcalDir("")
	, m_PropCur(0)
	, bState(true)
	, asm_solid(NULL)
{
}

CAsmMdlOkDlg::~CAsmMdlOkDlg()
{
}

void CAsmMdlOkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_TextEdit);
	DDX_Control(pDX, IDC_PROGRESS1, m_Prog);
	DDX_Control(pDX, IDOK, m_OkButton);
}


BEGIN_MESSAGE_MAP(CAsmMdlOkDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	//ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CAsmMdlOkDlg ��Ϣ�������

BOOL CAsmMdlOkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

    m_Prog.SetPos(m_PropCur);
	m_TextEdit.SetWindowText("��ȷ�ϼ����ϣ�Ȼ���ύģ�ͣ�");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}
/************************************************************************/
/* ��װ�����Ԫ�����б��,��0��ʼ���,������װ���屾��Ҳ���Ϊ0         */
/************************************************************************/
void CAsmMdlOkDlg::MarkComp(ProSolid comp, int upLevel)
{
	asm_comp_item                 tempitem;
	ProError                      status;
	ProFeattype                   ftype;
	ProSolid                      mdl;
	CArray<ProFeature,ProFeature> feat_list;

	tempitem.mdl		= comp;
	tempitem.upLevel	= upLevel;
	tempitem.num		= (int)p_AsmCompItem.GetCount();
	ProSolidToPostfixId(comp,&tempitem.postId);

	p_AsmCompItem.Add(tempitem);
	//������һ��
	status=ProSolidFeatVisit(comp,(ProFeatureVisitAction)FeatureAsmVisitAcFn
		,NULL,(CArray<ProFeature,ProFeature>*)&feat_list);

	int num=(int)feat_list.GetCount();
	for (int i=0;i<num;i++) {		
		status = ProFeatureTypeGet (&feat_list[i], &ftype);
		if (ftype == PRO_FEAT_COMPONENT){			
			ProAsmcompMdlGet((ProAsmcomp *)(&feat_list[i])
				,(ProMdl *)&mdl);
			MarkComp(mdl,tempitem.num);
		}
	}
}
/************************************************************************/
/* �ύģ��, ����һϵ�еļ��                                           */
/************************************************************************/
void CAsmMdlOkDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString      buttonLabel;
	m_OkButton.GetWindowText(buttonLabel);
	if(buttonLabel==_T("�ر�")) 
	{
		OnOK();
		return;
	}
	m_OkButton.EnableWindow(FALSE);
	try
	{
		//���Ԫ�������ϲ�Ϊ0
		p_AsmCompItem.RemoveAll();
		MarkComp(asm_solid,0);
		//���
		if(!Check(asm_solid)) return;
        if(!CheckMdlInfo(asm_solid)) return;
		if(!GetSWTXItem())
		{
			SetProg(0,"���ʧ��");
			return;
		}
		m_pSet.Open();
		if(!m_pSet.m_pDatabase->CanTransact()) return;
		m_pSet.m_pDatabase->BeginTrans();
		if(!InsertAsmPropInfo()){
			SetProg(0,"���ʧ��");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!InsertAsmMakeupInfo()){
			SetProg(0,"���ʧ��");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if (!InsertAsmPartCompList())
		{
			SetProg(0,"���ʧ��");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!InsertSWTX()){
			SetProg(0,"���ʧ��");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!InsertPerfInfo()){
			SetProg(0,"���ʧ��");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!InsertAsmPicInfo()){
			SetProg(0,"���ʧ��");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!CreateTempAsmDirectory(asm_solid,&fileLodcalDir)){
			SetProg(0,"���ʧ��");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!CopyLocalDirToServer()) //100
		{
			SetProg(0,"���ʧ��");
			RemoveDireAndFile(fileLodcalDir);
			m_pSet.m_pDatabase->Rollback();
			if(m_pSet.IsOpen()) m_pSet.Close();
			return;
		}
        RemoveDireAndFile(fileLodcalDir);
		SetProg(100,"�Ѿ����ģ�͵����");
		m_pSet.m_pDatabase->CommitTrans();
		//�޸�ģ�ͱ�ʶ�ź������Ϣ
		SetMdlInfo(asm_solid)->Number=asm_num;
		SetMdlInfo(asm_solid)->MdlType=3;
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		m_pSet.m_pDatabase->Rollback();
		if(m_pSet.IsOpen()) m_pSet.Close();
		return;
	}
	m_OkButton.SetWindowText(_T("�ر�"));
	m_OkButton.EnableWindow(TRUE);
}


/************************************************************************/
/* ���ģ�͵ı��                                                       */
/************************************************************************/
int CAsmMdlOkDlg::GetSolidNum(ProSolid solid)
{
	int num;
	num=(int)p_AsmCompItem.GetCount();
	for (int i=0;i<num;i++)
	{
		if (p_AsmCompItem[i].mdl==solid)
		{
			return p_AsmCompItem[i].num;
		}
	}
	return -1;
}

/************************************************************************/
/* ��װ������������Խ���ת��                                           */
/************************************************************************/
BOOL CAsmMdlOkDlg::GetSWTXItem(void)
{
	int               num=0;
	CString           temp;
	ProName           symbol;            //�ߴ����
	double            value;           //�ߴ�ֵ
	ProDimensiontype  type;            //�ߴ�����
	double            upper_limit;     //�ߴ����ƫ��  
	double            lower_limit;     //�ߴ����ƫ�� 
	int               postId=0;          //����ĺ�׺
	ProError          status;
	ProParamvalue     proval;
	CString           csType;
	CString           csValue;
	Asm_Para_item     tempParaItem;
	int               solidnum=-1;
	SetProg(m_PropCur+1,"���ڽ����������Ա��ת��");
	num=(int)p_AsmSmlItemList.GetCount();
	for (int i=0;i<num;i++) {
		SetProg(m_PropCur+(int)(i*10/num),"���ڽ����������Ա��ת��");
		switch(p_AsmSmlItemList[i].marsk) {
		case TYPE_DIM:
			if(p_AsmSmlItemList[i].owner!=asm_solid) continue;
			solidnum=GetSolidNum((ProSolid)p_AsmSmlItemList[i].dim.owner);
			status=ProDimensionValueGet(&p_AsmSmlItemList[i].dim,&value);
			status=ProDimensionTypeGet(&p_AsmSmlItemList[i].dim,&type);
			status=ProDimensionSymbolGet(&p_AsmSmlItemList[i].dim,symbol);
			status=ProDimensionToleranceGet(&p_AsmSmlItemList[i].dim,&upper_limit,&lower_limit);
			//���������ģ���Ѿ������ڣ���Ҫ�Ƴ�
			if (status!=PRO_TK_NO_ERROR) 
			{
				return FALSE;
			}
			switch(type) {
		case PRODIMTYPE_LINEAR:
			csType=_T("PRODIMTYPE_LINEAR");
			break;
		case PRODIMTYPE_RADIUS:
			csType=_T("PRODIMTYPE_RADIUS");
			break;
		case PRODIMTYPE_DIAMETER:
			csType=_T("PRODIMTYPE_DIAMETER");
			break;
		case PRODIMTYPE_ANGLE:
			csType=_T("PRODIMTYPE_ANGLE");
			break;
		default:
			AfxMessageBox("�������Ա��У�����δ֪���͵ĳߴ磡");
			return FALSE;
			break;
			}
			tempParaItem.solid_num=solidnum;
			tempParaItem.id.Format("d%d:%d",p_AsmSmlItemList[i].dim.id,solidnum);
			tempParaItem.Name=p_AsmSmlItemList[i].name;
			tempParaItem.featName=_T("");
			tempParaItem.DimSymbol.Format("%s:%d",CString(symbol),solidnum);
			tempParaItem.type=_T("dim");
			tempParaItem.SubType=csType;
			tempParaItem.Note=p_AsmSmlItemList[i].note;
            tempParaItem.Value.Format("%f",value);
			tempParaItem.UpLimit.Format("%f",upper_limit);
			tempParaItem.DwLimit.Format("%f",lower_limit);
			P_temp_AsmSmlItem.Add(tempParaItem);
			break;
		case TYPE_PARA:
			if(p_AsmSmlItemList[i].owner!=asm_solid) continue;
			solidnum=GetSolidNum((ProSolid)(p_AsmSmlItemList[i].para.owner.type==PRM_MODEL?p_AsmSmlItemList[i].para.owner.who.model:p_AsmSmlItemList[i].para.owner.who.item.owner));			
			status=ProParameterValueGet(&p_AsmSmlItemList[i].para,&proval);
			if (status!=PRO_TK_NO_ERROR) return FALSE;
			temp.Format("%s:%d",CString(p_AsmSmlItemList[i].para.id),solidnum);	
			switch(proval.type) {
			case PRO_PARAM_DOUBLE:
				csType="PRO_PARAM_DOUBLE";
				csValue.Format("%f",proval.value.d_val);
				break;
			case PRO_PARAM_STRING:
				csType="PRO_PARAM_STRING";
				csValue=CString(proval.value.s_val);
				break;
			case PRO_PARAM_INTEGER:
				csValue.Format("%d",proval.value.i_val);
				csType="PRO_PARAM_INTEGER";
				break;
			case PRO_PARAM_BOOLEAN:
				csType="PRO_PARAM_BOOLEAN";
				csValue.Format("%d",proval.value.l_val);
				break;
			case PRO_PARAM_NOTE_ID:
				csType="PRO_PARAM_NOTE_ID";
				csValue="PRO_PARAM_NOTE_ID";
				break;
			default:
				AfxMessageBox("�������Ա��д����޷�ʶ��Ĳ�������(PRO_PARAM_VOID)����ȷ�ϣ�");
				return FALSE;
				break;
				}
            tempParaItem.solid_num=solidnum;
			tempParaItem.id=temp;
			tempParaItem.Name=p_AsmSmlItemList[i].name;
			tempParaItem.DimSymbol=_T("");
			tempParaItem.featName=temp;
			tempParaItem.type=_T("para");
			tempParaItem.Note=p_AsmSmlItemList[i].note;
			tempParaItem.SubType=csType;
			tempParaItem.Value=csValue;
			tempParaItem.DwLimit=_T("");
			tempParaItem.UpLimit=_T("");
			P_temp_AsmSmlItem.Add(tempParaItem);
			break;
		case TYPE_FEAT:
			//if(p_AsmSmlItemList[i].dim.owner!=(ProMdl)solid) continue;
			break;
		case TYPE_COMP:
			//if(p_AsmSmlItemList[i].dim.owner!=(ProMdl)solid) continue;
			break;
		default:
			break;
		}
	}
	return TRUE;
}
/************************************************************************/
/* �����ݿ����ģ�͵Ļ�����Ϣ                                           */
/************************************************************************/
BOOL CAsmMdlOkDlg::InsertAsmPropInfo(void)
{
	UsrPartPorp      temp_porp;
	CString          temp;
	CTime			 time=CTime::GetCurrentTime();
	SetProg(m_PropCur+1,"�������������Ϣ");
	temp_porp=GetMdlInfo(asm_solid);
	m_pSet.AddNew();
	m_pSet.m_Name=temp_porp.Name;
	m_pSet.m_Code=temp_porp.Code;
	m_pSet.m_Designer=temp_porp.Person;
	m_pSet.m_TypeNum=temp_porp.TypeNum;
	m_pSet.m_SolidFileName="��";
	m_pSet.m_SolidFilePlace="��";
	m_pSet.m_Datatime=time;
	if (temp_porp.bHaveDraw)
	{
		m_pSet.m_DrawingFileName=GetMdlFileName(temp_porp.hDraw);
	}
	m_pSet.m_SolidFileName=GetMdlFileName(temp_porp.owner);
	m_pSet.Update();
	m_pSet.Requery();	
	m_pSet.MoveLast();
	m_pSet.Edit();
	temp.Format("/װ��ģ��/%ld\0",m_pSet.m_Number);
	fileDirectory.Format("%s",temp.GetBuffer());
	//���ô洢��λ��
	m_pSet.m_SolidFilePlace  = temp;
	if (temp_porp.bHaveDraw)
	{		
		m_pSet.m_DrawingFilePlace= temp+"/drawing";		
	}
	m_pSet.Update();
	//��ø���ӵı�ʶ��
	asm_num=m_pSet.m_Number;	
	return TRUE;
}
/************************************************************************/
/* ���������ģ�͵�ģ�������Ϣ                                         */
/************************************************************************/
BOOL CAsmMdlOkDlg::InsertAsmMakeupInfo(void)
{
	int					num=0,indexNo=-1,compItem=-1;
	UsrPartPorp         temp_porp;
	CNAsmBomTable       m_Set(m_pSet.m_pDatabase);

	num =	(int)p_AsmCompItem.GetCount();
	m_Set.Open();
	//�ӿ�ʼ����Ϊװ���
	for (int i=1;i<num;i++)
	{
		SetProg(m_PropCur+(int)(i*10/num),"���������ģ�͵�ģ�������Ϣ");
		temp_porp=GetMdlInfo(p_AsmCompItem[i].mdl);
		m_Set.AddNew();
		m_Set.m_AsmNum    =    asm_num;
		m_Set.m_ComNum    =    p_AsmCompItem[i].num;
		m_Set.m_PartMdlNum= temp_porp.MdlNum;   //Ԫ������������ģ�ͣ����Ԫ��û��ģ����Ϊ-1
		m_Set.m_PartNum  =  temp_porp.Number;
		m_Set.m_UpComNum =  p_AsmCompItem[i].upLevel;
        m_Set.m_Name     =  temp_porp.Name;
        m_Set.m_FileName =  GetMdlFileName(temp_porp.owner).MakeUpper();
		m_Set.m_Type     =  GetMdlFileName(temp_porp.owner).Right(3).MakeLower();//_T("asm"):_T("prt");
        m_Set.m_Note     =  temp_porp.Note;
		m_Set.Update();
		m_Set.Requery();
	}
	return TRUE;
}
/************************************************************************/
/* �������嵥                                                         */
/************************************************************************/
BOOL CAsmMdlOkDlg::InsertAsmPartCompList(void)
{
	CNAsmComListTable			m_Set(m_pSet.m_pDatabase);
	CArray<ProSolid,ProSolid>	solidlist;
	CArray<int,int>				numlist;
	UsrPartPorp					temp_porp;
	m_Set.Open();
	GetAsmPartCompHand(asm_solid,&solidlist,&numlist);
	for (int i=0;i<solidlist.GetCount();i++)
	{
		temp_porp=GetMdlInfo(solidlist[i]);
		m_Set.AddNew();
		m_Set.m_AsmNum  = asm_num;
		m_Set.m_PartNum = temp_porp.Number; 
		m_Set.m_Count   = numlist[i];
		m_Set.Update();
		m_Set.Requery();
	}
	return TRUE;
}
/************************************************************************/
/*  �����ݿ����ģ�͵��������Ա���Ϣ                                    */
/************************************************************************/
BOOL CAsmMdlOkDlg::InsertSWTX(void)
{
	int						num=0;
	CAsmVarSet				m_Set(m_pSet.m_pDatabase);
	m_Set.Open();
	num=(int)P_temp_AsmSmlItem.GetCount();
    for (int i=0;i<num;i++)
    {
		SetProg(m_PropCur+(int)(i*10/num),"�����ݿ����ģ�͵��������Ա���Ϣ");
		m_Set.AddNew();
		m_Set.m_AsmNum	=asm_num;
		m_Set.m_Name	=P_temp_AsmSmlItem[i].Name;
		m_Set.m_Number	=i+1;
		m_Set.m_PostVariant	=P_temp_AsmSmlItem[i].solid_num;
		m_Set.m_Type	=P_temp_AsmSmlItem[i].type;
		m_Set.m_IdNum	=P_temp_AsmSmlItem[i].id;
		m_Set.m_DimSymbol	=P_temp_AsmSmlItem[i].DimSymbol;
		m_Set.m_ParaFeatName=P_temp_AsmSmlItem[i].featName;
		m_Set.m_SubType	=P_temp_AsmSmlItem[i].SubType;
		m_Set.m_Value	=P_temp_AsmSmlItem[i].Value;
		m_Set.m_UpLimit	=P_temp_AsmSmlItem[i].UpLimit;
		m_Set.m_DwLimit	=P_temp_AsmSmlItem[i].DwLimit;
	    m_Set.m_Note	=P_temp_AsmSmlItem[i].Note;
		m_Set.Update();
		m_Set.Requery();
    }	
	return TRUE;
}

/************************************************************************/
/* �����ݿ����ģ�͵����ܲ�����Ϣ                                       */
/************************************************************************/
BOOL CAsmMdlOkDlg::InsertPerfInfo(void)
{
	int         num=0;
	int         indexNo=0;
	CAsmPerfSet m_Set(m_pSet.m_pDatabase);
	m_Set.Open();
	num=(int)p_UsrPerfParaTableList.GetCount();
	for (int i=0;i<num;i++)
	{
		SetProg(m_PropCur+(int)(i*10/num),"�����ݿ����ģ�͵����ܲ�����Ϣ");
		if(p_UsrPerfParaTableList[i].owner!=(ProMdl)asm_solid) continue;
		if(p_UsrPerfParaTableList[i].isCheck!=1)               continue;
		indexNo++;
		m_Set.AddNew();
		m_Set.m_AsmNum	=asm_num;
		m_Set.m_Number	=indexNo;
		m_Set.m_Name	=p_UsrPerfParaTableList[i].Name;
		m_Set.m_Value	=p_UsrPerfParaTableList[i].value;
		m_Set.m_Type	= p_UsrPerfParaTableList[i].Type;
		m_Set.m_MaxVal	= p_UsrPerfParaTableList[i].MaxValue;
		m_Set.m_MinVal	= p_UsrPerfParaTableList[i].MinValue;
		m_Set.Update();
		m_Set.Requery();
	}
	return TRUE;
}

/************************************************************************/
/*  �����ݿ�ͼƬ��Ϣ                                                    */
/************************************************************************/
BOOL CAsmMdlOkDlg::InsertAsmPicInfo(void)
{
	int							num=0,indexNo=0;
	CArray<ProSolid,ProSolid>	solidlist;
	CAsmPicSet					m_Set(m_pSet.m_pDatabase);	
	solidlist.Add(asm_solid);
	GetAsmCompHand(asm_solid,&solidlist);
	num=(int)p_UsrPicture.GetCount();
	m_Set.Open();
	for (int i=0;i<num;i++) {
		SetProg(m_PropCur+(int)(i*10/num),"�����ݿ����ͼƬ��Ϣ");
		int numSolid=(int)solidlist.GetCount();
		int listIndex=0;
		for (int j=0;j<numSolid;j++) {
			if(p_UsrPicture[i].owner!=(ProSolid)solidlist[j]) continue;
			if(p_UsrPicture[i].isCheck!=1) continue;
            indexNo++;
			CString error_info;
			m_Set.AddNew();
			m_Set.m_AsmNum=asm_num;
			m_Set.m_Number=indexNo;
			m_Set.m_Name=p_UsrPicture[i].fileName;
			m_Set.m_IsMain=p_UsrPicture[i].owner==asm_solid?p_UsrPicture[i].isMain:0;
			m_Set.m_Note=p_UsrPicture[i].Note;
			DBSavePic(&m_Set.m_File,p_UsrPicture[i].filePath+p_UsrPicture[i].fileName,&m_Set,error_info);
			m_Set.Update();
			m_Set.Requery();
			break;
		}
	}
	return TRUE;
}
/************************************************************************/
/* ���ļ�����������
   ���̣������ڷ������ġ����ģ�͡�Ŀ¼�½���һ����ģ�ͱ�ʶ���������ļ���
         Ȼ���ٽ���һ�������ļ���(������µ�����ģ�ͺ͹���ͼ)�����Ժ�ָ�ʹ��
		 */
/************************************************************************/
BOOL CAsmMdlOkDlg::CopyLocalDirToServer(void)
{
	CFtpFile		ftp;
	BOOL			isok=TRUE;
	CString			drawDir1;
	CString			backupDir;
	CString			backdrawDir;
	CStringArray	strDirAry;
	CString			tempfilename;
	CArray<ProSolid,ProSolid>  solidlist;

	drawDir1	=   fileDirectory+ "/drawing" ;
	backupDir	=   fileDirectory+ "/backup"  ;
	backdrawDir	=	fileDirectory+ "/backup/drawing" ;

	strDirAry.Add(fileDirectory);
	strDirAry.Add(drawDir1);
	//strDirAry.Add(backupDir);
	//strDirAry.Add(backdrawDir);

	CInternetSession*	pSession;
	CFtpConnection*		pConnection;
	CFtpFileFind*		pFileFind;
	pConnection	=	NULL;
	pFileFind	=	NULL;

	pSession	=new CInternetSession();
	try
	{
		pConnection=pSession->GetFtpConnection(myFtp.strFtp,myFtp.strFtpUser,myFtp.strFtpPwd,myFtp.iFtpPort);
	}
	catch(CInternetException* e)
	{
		e->Delete();
		pConnection	=NULL;
		isok		=FALSE;
	}
	//�����������
	if(pConnection!=NULL)
	{
		//�ڷ��������������ļ���
		//|-<��ʶ��>
		//	|-<drawing>
		//	|-<backup>
		//	|-<drawing>
		//	|-�ļ�
		for (int i=0;i<strDirAry.GetCount();i++)
		{
			BOOL b=pConnection->CreateDirectory(strDirAry[i]);
			if(!b) isok=b;
		}
		//����Լ��fileDirectory ����û��"\\",���������
		//�����ļ� c:\\dd\dd.txt
		//������   /aa/dd.txt

		if(fileLodcalDir.Right(1)!="\\") fileLodcalDir+="\\";

		//1.���䶥������ļ�.asm 
		tempfilename	= GetMdlFileName(asm_solid)+".1";
		BOOL c			= pConnection->PutFile(fileLodcalDir+tempfilename,strDirAry[0]+"\\"+tempfilename);

		//2.���䶥������Ĺ���ͼ
		if(GetMdlInfo(asm_solid).bHaveDraw) 
		{
			tempfilename	= GetMdlFileName(GetMdlInfo(asm_solid).hDraw)+".1";
			BOOL           c= pConnection->PutFile(fileLodcalDir+"drawing\\"+tempfilename,strDirAry[1]+"\\"+tempfilename);
		}

		//3.��������ģ���ļ��������ļ�
  //      GetAsmCompHand(asm_solid,&solidlist);
		//for (int i=0;i<solidlist.GetCount();i++)
		//{
		//	tempfilename=GetMdlFileName(solidlist[i])+".1";
		//	BOOL c=pConnection->PutFile(fileLodcalDir+tempfilename,strDirAry[2]+"\\"+tempfilename);
		//	if(!c) isok=c;
		//	if(GetMdlInfo(solidlist[i]).bHaveDraw) 
		//	{
		//		tempfilename = GetMdlFileName(GetMdlInfo(solidlist[i]).hDraw)+".1";
		//		BOOL		c= pConnection->PutFile(fileLodcalDir+"drawing\\"+tempfilename,strDirAry[3]+"\\"+tempfilename);
		//		if(!c) isok	 = c;
		//	}
		//}
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return TRUE;
}
/************************************************************************/
/* ��� 
1.���е��㲿�����Ǿ�����˵�
2.ģ��Ҳ�Ǿ�����˵�
3.���е��㲿�������ύ���
4.����㲿���Ƿ��ֱ��޸Ĺ�������б��޸ģ��������ͨ����ˣ���Ծ�����˵������
5.���㲿�����㲿�����ݿ���еĶ������бȽϣ��ж��Ƿ��иı䣬
6.�ж��㲿���Ĺ�ϵʽ���Ƿ���ԭ������ͬ
7.����㲿��
8.����ϲ㲿������ӵĹ�ϵʽ��������ʹ��                                */
/************************************************************************/
BOOL CAsmMdlOkDlg::Check(ProSolid solid)
{
	CArray<ProSolid,ProSolid>   solidlist;	
	UsrPartPorp					temp_porp;
	CString                     error_info;
	CString                     unAudited;
	CCheckMdlIfHasChange		check_class;
	GetAsmCompHand(solid,&solidlist);      //������solidģ��
	for (int i=0;i<solidlist.GetCount();i++)
	{
		temp_porp=GetMdlInfo(solidlist[i]);
		//2.����㲿���Ƿ��������ݱ��м�¼
		if (temp_porp.Number<=0)
		{
			AfxMessageBox(GetMdlFileName(temp_porp.owner)+"���㲿�����в����ڣ��޷��ύ\n���Ȱ�����ύ�㲿���⣡");
			return FALSE;
		}
		//�Ƿ��б��޸Ĺ�
		//1.�Ƿ�����ģ�͵�ʵ��
		if (temp_porp.MdlType==1)
		{
			switch(GetMdlType(solidlist[i])) {
			case PRO_MDL_PART:
				//����Ƿ��иı�
				if (!check_class.CheckPartHasPrtMdl(m_pSet.m_pDatabase,solidlist[i],temp_porp.MdlNum,0))
				{
					//����Ƿ����µ�ʵ��
					if(!check_class.CheckPartHasPrtMdl(m_pSet.m_pDatabase,solidlist[i],temp_porp.MdlNum,1))
					{
						temp_porp.IsAudited=false;
						temp_porp.ResourceType=0;
						temp_porp.Number=-1;
					}
					else
					{
					}
				}
				break;
			case PRO_MDL_ASSEMBLY:
				break;
			default:
				break;
			}
		}	
		//�Ƿ������
		if (!temp_porp.IsAudited)
			unAudited+=GetMdlFileName(solidlist[i])+";"; 

		//�ж���������Ϣ
		
	}
	return TRUE;
}
/************************************************************************/
/* ��鶥�����������Ϣ���Ƿ�����                                       */
/************************************************************************/
bool CAsmMdlOkDlg::CheckMdlInfo(ProSolid  solid)
{
	UsrPartPorp      temp_partporp;
	CNAsmType        m_set(m_pSet.m_pDatabase);
	temp_partporp=GetMdlInfo(solid);
	//1.��������Ƿ�Ϊ��
	if (temp_partporp.Name.IsEmpty())
	{
		AfxMessageBox("���Ʋ���Ϊ�գ�");
		SetProg(0,"���Ʋ���Ϊ��");
		return false;
	}
	//2.�������Ƿ��Ѿ�����
	if (temp_partporp.Code.IsEmpty())
	{
		AfxMessageBox("ģ�͵ı��벻��Ϊ�գ�");
		SetProg(0,"���ʧ��");
		return false;
	}
	if (!CheckIfCodeHavebeenUsed(temp_partporp.TypeNum,temp_partporp.Code,m_pSet.m_pDatabase))
	{
		AfxMessageBox("ģ�͵ı����Ѿ���ʹ��,���޸�!");
		SetProg(0,"���ʧ��");
		return false;
	}
	//3.���ģ�͵�����Ƿ��Ѿ�����,�Լ��Ƿ�Ϸ�
	if (temp_partporp.TypeNum==-1)
	{
		AfxMessageBox("ģ�͵����û�ж���!");
		SetProg(0,"���ʧ��");
		return false;
	}
	//4.���ģ�͵�����Ƿ񻹴���
	m_set.m_strFilter.Format("[Number]=%ld",temp_partporp.TypeNum);
	if (!m_set.Open())
	{
		AfxMessageBox("���ݿ�����ʧ�ܣ�");
		return false;
	}
	if (m_set.IsBOF())
	{
		AfxMessageBox("��ѡ���ģ�͵���𲻴���!");
		SetProg(0,"���ʧ��");
		return false;
	}
	return true;
}
void CAsmMdlOkDlg::SetProg(int cur,CString message)
{
	if (bState)
	{
		m_TextEdit.SetWindowText(message);
		m_Prog.SetPos(cur);
		m_PropCur=cur;
	}
}
bool CAsmMdlOkDlg::UpdateMdl(ProSolid solid,long MdlNumber,CDatabase * db)
{
	CString						sql;
	CNAsmBomTable				m_set1(db);
	CNAsmComListTable			m_set2(db);
	CAsmPicSet					m_set3(db);
	CAsmVarSet       			m_set4(db);
    CAsmPerfSet					m_set5(db);
	asm_solid			= solid;
	m_pSet.m_pDatabase	= db;
	asm_num				= MdlNumber;
	bState				= false;
	m_pSet.m_pDatabase	= db;
	try
	{
		//���Ԫ�������ϲ�Ϊ0
		p_AsmCompItem.RemoveAll();
		MarkComp(asm_solid,0);
		//���
		if(!Check(asm_solid)) return false; 
		if(!GetSWTXItem())
		{
			return false;
		}		
		if(!db->CanTransact())
		{
			AfxMessageBox("���ݿⲻ֧��������");
			return false;
		}
		db->BeginTrans();
		m_pSet.m_strFilter.Format("[Number]=%ld",MdlNumber);
		m_pSet.Open();
		if (m_pSet.IsBOF())
		{
			AfxMessageBox("ģ�Ͳ�����");
			return false;
		}
		m_pSet.MoveFirst();
		//ɾ��ԭ����
		sql.Format("DELETE FROM %s  WHERE [AsmNum]=%ld ",m_set1.GetDefaultSQL(),MdlNumber);
		db->ExecuteSQL(sql);
		sql.Format("DELETE FROM %s  WHERE [AsmNum]=%ld ",m_set2.GetDefaultSQL(),MdlNumber);
		db->ExecuteSQL(sql);
		sql.Format("DELETE FROM %s  WHERE [AsmNum]=%ld ",m_set3.GetDefaultSQL(),MdlNumber);
		db->ExecuteSQL(sql);
		sql.Format("DELETE FROM %s  WHERE [AsmNum]=%ld ",m_set4.GetDefaultSQL(),MdlNumber);
		db->ExecuteSQL(sql);
		sql.Format("DELETE FROM %s  WHERE [AsmNum]=%ld ",m_set5.GetDefaultSQL(),MdlNumber);
		db->ExecuteSQL(sql);
		//�޸Ļ�����Ϣ
		UsrPartPorp      temp_porp;
		CString          temp;
		CTime			 time=CTime::GetCurrentTime();
		temp_porp=GetMdlInfo(asm_solid);		
		m_pSet.Edit();
		m_pSet.m_Name	= temp_porp.Name;
		m_pSet.m_Code	= temp_porp.Code;
		m_pSet.m_Designer	= temp_porp.Person;
		m_pSet.m_TypeNum	= temp_porp.TypeNum;
		m_pSet.m_Datatime	= time;
		m_pSet.m_SolidFileName = GetMdlFileName(temp_porp.owner);
		m_pSet.m_SolidFilePlace.Format("/װ��ģ��/%ld\0",MdlNumber);
		if (temp_porp.bHaveDraw)
		{
			m_pSet.m_DrawingFileName = GetMdlFileName(temp_porp.hDraw);
			m_pSet.m_DrawingFilePlace.Format("/װ��ģ��/%ld/drawing\0",MdlNumber);		
		}		
		m_pSet.Update();
		m_pSet.Requery();	
		m_pSet.MoveLast();
		fileDirectory.Format("/װ��ģ��/%ld\0",MdlNumber);

		if(!InsertAsmMakeupInfo()){
			db->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return false;
		}
		if (!InsertAsmPartCompList())
		{
			db->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return false;
		}
		if(!InsertSWTX()){
			db->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return false;
		}
		if(!InsertPerfInfo()){
			db->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return false;
		}
		if(!InsertAsmPicInfo()){
			db->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return false;
		}
		if(!CreateTempAsmDirectory(asm_solid,&fileLodcalDir)){
			db->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return false;
		}
		if(!CopyLocalDirToServer()) //100
		{
			RemoveDireAndFile(fileLodcalDir);
			db->Rollback();
			if(m_pSet.IsOpen()) m_pSet.Close();
			return false;
		}
		RemoveDireAndFile(fileLodcalDir);
		db->CommitTrans();
		//�޸�ģ�ͱ�ʶ�ź������Ϣ
		SetMdlInfo(asm_solid)->Number=asm_num;
		SetMdlInfo(asm_solid)->MdlType=3;
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		db->Rollback();
		if(m_pSet.IsOpen()) m_pSet.Close();
		return false;
	}
	return true;
}