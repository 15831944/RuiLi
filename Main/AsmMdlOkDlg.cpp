// AsmMdlOkDlg.cpp : 实现文件
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
/* 提交的条件
1.所有的零部件都是经过审核的
2.模型也是经过审核的
3.所有的零部件都是提交审核

  提交的过程
1.基本信息表
2.事务特性表
3.性能参数表
4.组成表
5.清单
6.备份模型到服务器上去
  --|
    |-drawing 文件夹  存放工程图纸
	|-组建文件 .asm   存放最上层的
	|-Backup          所有的备份  防止出现零部件都存在的时候的无法生成模型
	     |-Drawing    所有零部件的工程图
		 |-所有的文件 */
/************************************************************************/
// CAsmMdlOkDlg 对话框

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


// CAsmMdlOkDlg 消息处理程序

BOOL CAsmMdlOkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  在此添加额外的初始化

    m_Prog.SetPos(m_PropCur);
	m_TextEdit.SetWindowText("请确认检查完毕，然后提交模型！");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}
/************************************************************************/
/* 给装配件的元件进行编号,从0开始编号,包括了装配体本身也编号为0         */
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
	//访问下一层
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
/* 提交模型, 进行一系列的检查                                           */
/************************************************************************/
void CAsmMdlOkDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString      buttonLabel;
	m_OkButton.GetWindowText(buttonLabel);
	if(buttonLabel==_T("关闭")) 
	{
		OnOK();
		return;
	}
	m_OkButton.EnableWindow(FALSE);
	try
	{
		//组成元件中最上层为0
		p_AsmCompItem.RemoveAll();
		MarkComp(asm_solid,0);
		//检查
		if(!Check(asm_solid)) return;
        if(!CheckMdlInfo(asm_solid)) return;
		if(!GetSWTXItem())
		{
			SetProg(0,"添加失败");
			return;
		}
		m_pSet.Open();
		if(!m_pSet.m_pDatabase->CanTransact()) return;
		m_pSet.m_pDatabase->BeginTrans();
		if(!InsertAsmPropInfo()){
			SetProg(0,"添加失败");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!InsertAsmMakeupInfo()){
			SetProg(0,"添加失败");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if (!InsertAsmPartCompList())
		{
			SetProg(0,"添加失败");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!InsertSWTX()){
			SetProg(0,"添加失败");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!InsertPerfInfo()){
			SetProg(0,"添加失败");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!InsertAsmPicInfo()){
			SetProg(0,"添加失败");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!CreateTempAsmDirectory(asm_solid,&fileLodcalDir)){
			SetProg(0,"添加失败");
			m_pSet.m_pDatabase->Rollback();if(m_pSet.IsOpen()) m_pSet.Close();return;
		}
		if(!CopyLocalDirToServer()) //100
		{
			SetProg(0,"添加失败");
			RemoveDireAndFile(fileLodcalDir);
			m_pSet.m_pDatabase->Rollback();
			if(m_pSet.IsOpen()) m_pSet.Close();
			return;
		}
        RemoveDireAndFile(fileLodcalDir);
		SetProg(100,"已经完成模型的添加");
		m_pSet.m_pDatabase->CommitTrans();
		//修改模型标识号和类别信息
		SetMdlInfo(asm_solid)->Number=asm_num;
		SetMdlInfo(asm_solid)->MdlType=3;
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		m_pSet.m_pDatabase->Rollback();
		if(m_pSet.IsOpen()) m_pSet.Close();
		return;
	}
	m_OkButton.SetWindowText(_T("关闭"));
	m_OkButton.EnableWindow(TRUE);
}


/************************************************************************/
/* 获得模型的编号                                                       */
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
/* 把装配体的事务特性进行转换                                           */
/************************************************************************/
BOOL CAsmMdlOkDlg::GetSWTXItem(void)
{
	int               num=0;
	CString           temp;
	ProName           symbol;            //尺寸符号
	double            value;           //尺寸值
	ProDimensiontype  type;            //尺寸类型
	double            upper_limit;     //尺寸的上偏差  
	double            lower_limit;     //尺寸的下偏差 
	int               postId=0;          //零件的后缀
	ProError          status;
	ProParamvalue     proval;
	CString           csType;
	CString           csValue;
	Asm_Para_item     tempParaItem;
	int               solidnum=-1;
	SetProg(m_PropCur+1,"正在进行事物特性表的转换");
	num=(int)p_AsmSmlItemList.GetCount();
	for (int i=0;i<num;i++) {
		SetProg(m_PropCur+(int)(i*10/num),"正在进行事物特性表的转换");
		switch(p_AsmSmlItemList[i].marsk) {
		case TYPE_DIM:
			if(p_AsmSmlItemList[i].owner!=asm_solid) continue;
			solidnum=GetSolidNum((ProSolid)p_AsmSmlItemList[i].dim.owner);
			status=ProDimensionValueGet(&p_AsmSmlItemList[i].dim,&value);
			status=ProDimensionTypeGet(&p_AsmSmlItemList[i].dim,&type);
			status=ProDimensionSymbolGet(&p_AsmSmlItemList[i].dim,symbol);
			status=ProDimensionToleranceGet(&p_AsmSmlItemList[i].dim,&upper_limit,&lower_limit);
			//如果参数在模型已经不存在，则要移除
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
			AfxMessageBox("事物特性表中，存在未知类型的尺寸！");
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
				AfxMessageBox("事物特性表中存在无法识别的参数类型(PRO_PARAM_VOID)，请确认！");
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
/* 向数据库添加模型的基本信息                                           */
/************************************************************************/
BOOL CAsmMdlOkDlg::InsertAsmPropInfo(void)
{
	UsrPartPorp      temp_porp;
	CString          temp;
	CTime			 time=CTime::GetCurrentTime();
	SetProg(m_PropCur+1,"正在添加属性信息");
	temp_porp=GetMdlInfo(asm_solid);
	m_pSet.AddNew();
	m_pSet.m_Name=temp_porp.Name;
	m_pSet.m_Code=temp_porp.Code;
	m_pSet.m_Designer=temp_porp.Person;
	m_pSet.m_TypeNum=temp_porp.TypeNum;
	m_pSet.m_SolidFileName="无";
	m_pSet.m_SolidFilePlace="无";
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
	temp.Format("/装配模型/%ld\0",m_pSet.m_Number);
	fileDirectory.Format("%s",temp.GetBuffer());
	//设置存储的位置
	m_pSet.m_SolidFilePlace  = temp;
	if (temp_porp.bHaveDraw)
	{		
		m_pSet.m_DrawingFilePlace= temp+"/drawing";		
	}
	m_pSet.Update();
	//获得刚添加的标识号
	asm_num=m_pSet.m_Number;	
	return TRUE;
}
/************************************************************************/
/* 向数据添加模型的模型组成信息                                         */
/************************************************************************/
BOOL CAsmMdlOkDlg::InsertAsmMakeupInfo(void)
{
	int					num=0,indexNo=-1,compItem=-1;
	UsrPartPorp         temp_porp;
	CNAsmBomTable       m_Set(m_pSet.m_pDatabase);

	num =	(int)p_AsmCompItem.GetCount();
	m_Set.Open();
	//从开始，零为装配件
	for (int i=1;i<num;i++)
	{
		SetProg(m_PropCur+(int)(i*10/num),"向数据添加模型的模型组成信息");
		temp_porp=GetMdlInfo(p_AsmCompItem[i].mdl);
		m_Set.AddNew();
		m_Set.m_AsmNum    =    asm_num;
		m_Set.m_ComNum    =    p_AsmCompItem[i].num;
		m_Set.m_PartMdlNum= temp_porp.MdlNum;   //元件的所产生的模型，如果元件没有模型则为-1
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
/* 添加零件清单                                                         */
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
/*  向数据库添加模型的事物特性表信息                                    */
/************************************************************************/
BOOL CAsmMdlOkDlg::InsertSWTX(void)
{
	int						num=0;
	CAsmVarSet				m_Set(m_pSet.m_pDatabase);
	m_Set.Open();
	num=(int)P_temp_AsmSmlItem.GetCount();
    for (int i=0;i<num;i++)
    {
		SetProg(m_PropCur+(int)(i*10/num),"向数据库添加模型的事物特性表信息");
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
/* 向数据库添加模型的性能参数信息                                       */
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
		SetProg(m_PropCur+(int)(i*10/num),"向数据库添加模型的性能参数信息");
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
/*  向数据库图片信息                                                    */
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
		SetProg(m_PropCur+(int)(i*10/num),"向数据库添加图片信息");
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
/* 把文件传到服务器
   过程：首先在服务器的【组件模型】目录下建立一个以模型标识号命名的文件夹
         然后再建立一个备份文件夹(被组件下的所有模型和工程图)，供以后恢复使用
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
	//建立连接完成
	if(pConnection!=NULL)
	{
		//在服务器创件几个文件夹
		//|-<标识号>
		//	|-<drawing>
		//	|-<backup>
		//	|-<drawing>
		//	|-文件
		for (int i=0;i<strDirAry.GetCount();i++)
		{
			BOOL b=pConnection->CreateDirectory(strDirAry[i]);
			if(!b) isok=b;
		}
		//保存约定fileDirectory 后面没有"\\",必须添加上
		//本地文件 c:\\dd\dd.txt
		//服务器   /aa/dd.txt

		if(fileLodcalDir.Right(1)!="\\") fileLodcalDir+="\\";

		//1.传输顶层组件文件.asm 
		tempfilename	= GetMdlFileName(asm_solid)+".1";
		BOOL c			= pConnection->PutFile(fileLodcalDir+tempfilename,strDirAry[0]+"\\"+tempfilename);

		//2.传输顶层组件的工程图
		if(GetMdlInfo(asm_solid).bHaveDraw) 
		{
			tempfilename	= GetMdlFileName(GetMdlInfo(asm_solid).hDraw)+".1";
			BOOL           c= pConnection->PutFile(fileLodcalDir+"drawing\\"+tempfilename,strDirAry[1]+"\\"+tempfilename);
		}

		//3.传输所有模型文件到备份文件
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
/* 检查 
1.所有的零部件都是经过审核的
2.模型也是经过审核的
3.所有的零部件都是提交审核
4.检查零部件是否又被修改过，如果有被修改，则必须先通过审核（针对经过审核的零件）
5.把零部件与零部件数据库表中的东西进行比较，判断是否有改变，
6.判断零部件的关系式，是否与原来的相同
7.检查零部件
8.如果上层部件内添加的关系式，将不被使用                                */
/************************************************************************/
BOOL CAsmMdlOkDlg::Check(ProSolid solid)
{
	CArray<ProSolid,ProSolid>   solidlist;	
	UsrPartPorp					temp_porp;
	CString                     error_info;
	CString                     unAudited;
	CCheckMdlIfHasChange		check_class;
	GetAsmCompHand(solid,&solidlist);      //不包括solid模型
	for (int i=0;i<solidlist.GetCount();i++)
	{
		temp_porp=GetMdlInfo(solidlist[i]);
		//2.检查零部件是否都有在数据表中记录
		if (temp_porp.Number<=0)
		{
			AfxMessageBox(GetMdlFileName(temp_porp.owner)+"在零部件库中不存在，无法提交\n请先把这个提交零部件库！");
			return FALSE;
		}
		//是否有被修改过
		//1.是否是有模型的实例
		if (temp_porp.MdlType==1)
		{
			switch(GetMdlType(solidlist[i])) {
			case PRO_MDL_PART:
				//检查是否有改变
				if (!check_class.CheckPartHasPrtMdl(m_pSet.m_pDatabase,solidlist[i],temp_porp.MdlNum,0))
				{
					//检查是否是新的实例
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
		//是否有审核
		if (!temp_porp.IsAudited)
			unAudited+=GetMdlFileName(solidlist[i])+";"; 

		//判断其他的信息
		
	}
	return TRUE;
}
/************************************************************************/
/* 检查顶层组件所有信息的是否完整                                       */
/************************************************************************/
bool CAsmMdlOkDlg::CheckMdlInfo(ProSolid  solid)
{
	UsrPartPorp      temp_partporp;
	CNAsmType        m_set(m_pSet.m_pDatabase);
	temp_partporp=GetMdlInfo(solid);
	//1.检查名称是否为空
	if (temp_partporp.Name.IsEmpty())
	{
		AfxMessageBox("名称不能为空！");
		SetProg(0,"名称不能为空");
		return false;
	}
	//2.检查编码是否已经存在
	if (temp_partporp.Code.IsEmpty())
	{
		AfxMessageBox("模型的编码不能为空！");
		SetProg(0,"添加失败");
		return false;
	}
	if (!CheckIfCodeHavebeenUsed(temp_partporp.TypeNum,temp_partporp.Code,m_pSet.m_pDatabase))
	{
		AfxMessageBox("模型的编码已经被使用,请修改!");
		SetProg(0,"添加失败");
		return false;
	}
	//3.检查模型的类别是否已经定义,以及是否合法
	if (temp_partporp.TypeNum==-1)
	{
		AfxMessageBox("模型的类别还没有定义!");
		SetProg(0,"添加失败");
		return false;
	}
	//4.检查模型的类别是否还存在
	m_set.m_strFilter.Format("[Number]=%ld",temp_partporp.TypeNum);
	if (!m_set.Open())
	{
		AfxMessageBox("数据库连接失败！");
		return false;
	}
	if (m_set.IsBOF())
	{
		AfxMessageBox("所选则的模型的类别不存在!");
		SetProg(0,"添加失败");
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
		//组成元件中最上层为0
		p_AsmCompItem.RemoveAll();
		MarkComp(asm_solid,0);
		//检查
		if(!Check(asm_solid)) return false; 
		if(!GetSWTXItem())
		{
			return false;
		}		
		if(!db->CanTransact())
		{
			AfxMessageBox("数据库不支持事务处理！");
			return false;
		}
		db->BeginTrans();
		m_pSet.m_strFilter.Format("[Number]=%ld",MdlNumber);
		m_pSet.Open();
		if (m_pSet.IsBOF())
		{
			AfxMessageBox("模型不存在");
			return false;
		}
		m_pSet.MoveFirst();
		//删除原来的
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
		//修改基本信息
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
		m_pSet.m_SolidFilePlace.Format("/装配模型/%ld\0",MdlNumber);
		if (temp_porp.bHaveDraw)
		{
			m_pSet.m_DrawingFileName = GetMdlFileName(temp_porp.hDraw);
			m_pSet.m_DrawingFilePlace.Format("/装配模型/%ld/drawing\0",MdlNumber);		
		}		
		m_pSet.Update();
		m_pSet.Requery();	
		m_pSet.MoveLast();
		fileDirectory.Format("/装配模型/%ld\0",MdlNumber);

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
		//修改模型标识号和类别信息
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