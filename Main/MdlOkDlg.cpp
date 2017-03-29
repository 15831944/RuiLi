// MdlOkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Main.h"
#include "MdlOkDlg.h"
#include "FtpFile.h"
#include "partVariantTableSet.h"
#include "partPerfparaTableSet.h"
#include "partRelationSet.h"
#include "partRelativeDimParaTableSet.h"
#include "partPicSet.h"
#include "partDrawingSet.h"
#include "partinstableset.h"
#include "partinsvaluetableset.h"
#include "partinsperfvaluetableset.h"
#include "NPartTypeSet.h"
#include "UsrSWTXTable.h"

CArray<UsrSWTXTable1,UsrSWTXTable1>  p_Rv_UsrTableList; 

IMPLEMENT_DYNAMIC(CMdlOkDlg, CDialog)
CMdlOkDlg::CMdlOkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMdlOkDlg::IDD, pParent)
	, m_progress(0)
	, m_prgpos(0)
	, p_db(NULL)
	, part_solid(NULL)
	, bState(true)
{
	
}

CMdlOkDlg::~CMdlOkDlg()
{
}

void CMdlOkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_PrgCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_TextEdit);
	DDX_Control(pDX, IDOK, m_OKButton);
}


BEGIN_MESSAGE_MAP(CMdlOkDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CMdlOkDlg ��Ϣ�������

void CMdlOkDlg::SetDbConnet(CDatabase * db)
{
	p_db=db;
	if(!p_db->IsOpen()) 
	{
		CString   strConnect=m_pSet.GetDefaultConnect();
		p_db->Open(NULL,FALSE,FALSE,strConnect,FALSE);
	}
	m_pSet.m_pDatabase=p_db;
}
void CMdlOkDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ProSolid     solid=part_solid;
	CString      serDir;
	long         MdlNumber=0;
	CString      tempPathName;  //g:/abc/bcd_temp
	CString      buttonLabel;
	//ProMdlCurrentGet((ProMdl*)&solid);
    m_OKButton.GetWindowText(buttonLabel);
    if(buttonLabel==_T("�ر�"))
	{
		CMdlOkDlg::OnOK();
		return;
	}
    m_OKButton.EnableWindow(FALSE);
	if(!CheckMdlInfo(solid)) return;
	if(!GetRvDimPara(solid)) return;
	if(!CreateTempDir(solid, &tempPathName)) 
	{
		AfxMessageBox(_T("����ģ�͹����г������ȱ����ģ�ͣ�"));
		SetProg(0,"����ʧ��!");
		return;
	}
	try{
		if(!p_db->CanTransact()) return;
		if(!p_db->BeginTrans()) AfxMessageBox("���ݿⲻ֧��������ʧ��");
		if(!InsertMdlPropToDb(solid,&MdlNumber,&serDir))
		{
			p_db->Rollback();
			RemoveDireAndFile(tempPathName);
			return;
		}
		InsertMdlVariantTableToDb(solid,MdlNumber);
		InsertMdlRelToDb(solid,MdlNumber);
		InsertMdlPerfToDb(solid,MdlNumber);
		InsertMdlRvToDb(solid,MdlNumber);
		InsertMdlPicToDb(solid,MdlNumber);
		//InsertDefaultMdlInst(MdlNumber);
        if(!CopyFilesToFtpServer(MdlNumber,tempPathName,serDir))
		{
			p_db->Rollback();
			RemoveDireAndFile(tempPathName);
			return;
		}
		p_db->CommitTrans();
		RemoveDireAndFile(tempPathName);
		SetMdlInfo(solid)->Number = MdlNumber;
		SetMdlInfo(solid)->MdlType= 3;        //��ǰ����Ѿ���Ϊģ��
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		RemoveDireAndFile(tempPathName);
		SetProg(0,"���ʧ�ܣ�");
		p_db->Rollback();
		return;
	}
	m_TextEdit.SetWindowText("�Ѿ����ģ�͵���ӣ�");
	m_PrgCtrl.SetPos(100);
	m_OKButton.SetWindowText(_T("�ر�"));
	m_OKButton.EnableWindow(TRUE);
}
/************************************************************************/
/* ���������Ϣ���Ƿ�����                                               */
/************************************************************************/
bool CMdlOkDlg::CheckMdlInfo(ProSolid  solid)
{
	////���������Ϣ
   UsrPartPorp      temp_partporp;
   CNPartTypeSet    m_set(p_db);
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
   if (!CheckIfCodeHavebeenUsed(temp_partporp.TypeNum,temp_partporp.Code,p_db))
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
/************************************************************************/
/* ��ģ�͵�������Ϣ�������ݿ�                                           */
/************************************************************************/
bool CMdlOkDlg::InsertMdlPropToDb(ProSolid  solid,long * MdlNumber,CString * strDir)
{
	UsrPartPorp      temp_partporp;
	CString          temp;
	CTime			 time=CTime::GetCurrentTime();
	SetProg(m_prgpos+1,"�������������Ϣ");
    temp_partporp=GetMdlInfo(solid);
    m_pSet.AddNew();
	m_pSet.m_Name=temp_partporp.Name;
	m_pSet.m_Code=temp_partporp.Code;
	m_pSet.m_Designer=temp_partporp.Person;
	m_pSet.m_Material=temp_partporp.Material;
	m_pSet.m_TypeNum=temp_partporp.TypeNum;
	m_pSet.m_Datatime=time;
	if (temp_partporp.bHaveDraw)
	{
		m_pSet.m_DrawingFileName=GetMdlFileName(temp_partporp.hDraw);
	}
	m_pSet.m_SolidFileName=GetMdlFileName(temp_partporp.owner);
	m_pSet.Update();
	m_pSet.Requery();	
	m_pSet.MoveLast();
	m_pSet.Edit();
	temp.Format("/���ģ��/%ld\0",m_pSet.m_Number);
	strDir->Format("%s",temp.GetBuffer());
	//���ô洢��λ��
	m_pSet.m_SolidFilePlace  = temp;
	if (temp_partporp.bHaveDraw)
	{		
		m_pSet.m_DrawingFilePlace= temp+"/drawing";	
	}
	m_pSet.Update();
	//��ø���ӵı�ʶ��
	MdlNumber[0]=m_pSet.m_Number;
	return true;
}
/************************************************************************/
/* ��ģ�͵��������Ա���Ϣ��ӵ����ݿ�                                   */
/************************************************************************/
bool CMdlOkDlg::InsertMdlVariantTableToDb(ProSolid  solid,long MdlNumber)
{

	int         num=0;
	long        indexNo=0;  //�������ı��
	
	CpartVariantTableSet m_variantset(p_db);
	SetProg(m_prgpos+1,"����ɱ������Ϣ");
	if(!m_variantset.Open())
	{
		AfxMessageBox(_T("��ӿɱ������Ϣ�����У�û�������ݿ⣡"));
		SetProg(0,"����ʧ��");
		return false;
	}
	
    if(m_variantset.CanAppend())
	{
         num=(int)p_UsrSWTXTableList.GetCount();
		 for(int i=0;i<num;i++)
		 {
			 m_prgpos=m_prgpos+(int)(i*10/num);
			 SetProg(m_prgpos,"����ɱ������Ϣ");
			 if(p_UsrSWTXTableList[i].owner!=(ProMdl)solid) continue;
			 if(p_UsrSWTXTableList[i].isCheck!=1)   continue;
			 indexNo++;
			 m_variantset.AddNew();
			 m_variantset.m_PrtNum =MdlNumber;			
			 m_variantset.m_Number  =indexNo;
			 m_variantset.m_Name  =p_UsrSWTXTableList[i].Name;
			 m_variantset.m_Type =p_UsrSWTXTableList[i].type;
			 m_variantset.m_IdNum =p_UsrSWTXTableList[i].id;
			 m_variantset.m_DimSymbol =p_UsrSWTXTableList[i].dimSymbol;
			 m_variantset.m_ParaFeatName =p_UsrSWTXTableList[i].featName;
			 m_variantset.m_SubType =p_UsrSWTXTableList[i].subType;
			 m_variantset.m_Value =p_UsrSWTXTableList[i].value;
			 m_variantset.m_UpLimit =p_UsrSWTXTableList[i].UpLimit;
			 m_variantset.m_DwLimit =p_UsrSWTXTableList[i].DwLimit;
			 m_variantset.m_Note =p_UsrSWTXTableList[i].Note;
			 m_variantset.Update();
			 m_variantset.Requery();
		 }
	}
	else
	{
		AfxMessageBox(_T("��ӿɱ������Ϣ�����У�û�������ݿ⣡"));
		SetProg(0,"���ʧ�ܣ�");
		 return false;
	}
	return true;
}
/************************************************************************/
/* ��ģ�͵Ĺ�ϵʽ��ӵ����ݿ�                                           */
/************************************************************************/
bool CMdlOkDlg::InsertMdlRelToDb(ProSolid  solid, long MdlNumber)
{
	int					num=0;
	CArray<CRel,CRel&>	p_RelList;
	CArray<int,int&>	p_IndexList;
	CString				rel;
	long				indexNo=0;  //�������ı��
	CpartRelationSet	m_RelSet(p_db);
	SetProg(m_prgpos,"�����ϵ��Ϣ");
	if(!m_RelSet.Open())
	{
		AfxMessageBox(_T("���ģ�͵Ĺ�ϵʽ�����У�û�������ݿ⣡"));
		SetProg(0,"���ʧ��");
		return false;
	}
	g_RelCollection.GetSolidAllRel(solid,false,&p_RelList,&p_IndexList);
	if(m_RelSet.CanAppend())
	{
		num=(int)p_RelList.GetCount();
		for (int i=0;i<num;i++)
		{
			m_prgpos=m_prgpos+(int)(i*10/num);
			SetProg(m_prgpos,"�����ϵ��Ϣ");
			p_RelList[i].GetRelString(rel);
			indexNo++;
			m_RelSet.AddNew();
			m_RelSet.m_PrtNum=MdlNumber;
			m_RelSet.m_Number=indexNo;
			m_RelSet.m_Relation=rel;
			m_RelSet.m_Note=p_RelList[i].GetRelNote();
			m_RelSet.Update();
			m_RelSet.Requery();
		}
	}
	else
	{
		AfxMessageBox(_T("���ģ�͵Ĺ�ϵʽʱ���޷������ݿ⣡"));
		SetProg(0,"���ʧ��");
		return false;
	}
	return true;	
}
/************************************************************************/
/* ��ģ�͵����ܲ�����ӵ����ݿ�                                         */
/************************************************************************/
bool CMdlOkDlg::InsertMdlPerfToDb(ProSolid  solid, long MdlNumber)
{

	int                   num=0;
	long                  indexNo=0;  //�������ı��
	CpartPerfparaTableSet m_PerfSet(p_db);	
	SetProg(m_prgpos,"�������ܲ�����Ϣ");
	if(!m_PerfSet.Open())
	{
		AfxMessageBox(_T("���ģ�͵����ܲ��������У�û�������ݿ⣡"));
		SetProg(0,"���ʧ��");
		return false;
	}
	if(m_PerfSet.CanAppend())
	{
		num=(int)p_UsrPerfParaTableList.GetCount();
		for(int i=0;i<num;i++)
		{
			m_prgpos=m_prgpos+(int)(i*10/num);
			SetProg(m_prgpos,"�������ܲ�����Ϣ");
			if(p_UsrPerfParaTableList[i].owner!=(ProMdl)solid) continue;
			if(p_UsrPerfParaTableList[i].isCheck!=1)   continue;
			indexNo++;
			m_PerfSet.AddNew();
			m_PerfSet.m_PrtNum = MdlNumber;
			m_PerfSet.m_Number = indexNo;
			m_PerfSet.m_Name   = p_UsrPerfParaTableList[i].Name;
			m_PerfSet.m_Value  = p_UsrPerfParaTableList[i].value;
			m_PerfSet.m_Note   = p_UsrPerfParaTableList[i].Note;
			m_PerfSet.m_Type   = p_UsrPerfParaTableList[i].Type;
			m_PerfSet.m_MaxVal = p_UsrPerfParaTableList[i].MaxValue;
			m_PerfSet.m_MinVal = p_UsrPerfParaTableList[i].MinValue;
			m_PerfSet.Update();
			m_PerfSet.Requery();
		}
	}
	else
	{
		AfxMessageBox(_T("���ģ�͵����ܲ���ʱ���޷������ݿ⣡"));
		SetProg(0,"���ʧ��");
		return false;
	}
	return true;
	
}
/************************************************************************/
/* ��ģ�͵���سߴ���Ϣ��ӵ����ݿ�                                     */
/************************************************************************/
bool CMdlOkDlg::InsertMdlRvToDb(ProSolid  solid, long MdlNumber)
{
	////PartRelativeDimParaTable
	int                          num=0;
	long                         indexNo=0;  //�������ı��
	CPartRelativeDimParaTableSet m_RvSet(p_db);
	SetProg(m_prgpos,"��������Ǵ���Ϣ");
	if(!m_RvSet.Open())
	{
		AfxMessageBox(_T("���ģ�͵���سߴ���Ϣʱ���޷������ݿ⣡"));
		SetProg(0,"���ʧ��");
		return false;
	}
	if(m_RvSet.CanAppend())
	{
		num=(int)p_Rv_UsrTableList.GetCount();
		for(int i=0;i<num;i++)
		{
			m_prgpos=m_prgpos+(int)(i*10/num);
			SetProg(m_prgpos,"��������Ǵ���Ϣ");
			if(p_Rv_UsrTableList[i].owner!=(ProMdl)solid) continue;
			if(p_Rv_UsrTableList[i].isCheck!=1)   continue;
			indexNo++;
			m_RvSet.AddNew();
            m_RvSet.m_PrtNum=    MdlNumber;
			m_RvSet.m_Number=    indexNo;
			m_RvSet.m_Type=      p_Rv_UsrTableList[i].type;
			m_RvSet.m_IdNum=     p_Rv_UsrTableList[i].id;
			m_RvSet.m_DimSymbol= p_Rv_UsrTableList[i].dimSymbol;
			m_RvSet.m_Name=      p_Rv_UsrTableList[i].Name;
			m_RvSet.m_SubType=   p_Rv_UsrTableList[i].subType;
			m_RvSet.m_Value=     p_Rv_UsrTableList[i].value;
			m_RvSet.Update();
			m_RvSet.Requery();
		}
	}
	else
	{		
		AfxMessageBox(_T("���ģ�͵���سߴ���Ϣʱ���޷������ݿ⣡"));
		SetProg(0,"���ʧ��");
		return false;
	}
	return true;
	
}
/************************************************************************/
/*��ͼƬ��Ϣ�������ݿ�                                                  */
/************************************************************************/
bool CMdlOkDlg::InsertMdlPicToDb(ProSolid solid, long MdlNumber)
{

	int         num=0;
	long        indexNo=0;  //�������ı��
	CpartPicSet m_PicSet(p_db);	
	SetProg(m_prgpos,"����ͼƬ��Ϣ");
	if(!m_PicSet.Open())
	{
		AfxMessageBox(_T("���ģ�͵�ͼƬ��Ϣ�����У�û�������ݿ⣡"));
		SetProg(0,"���ʧ��");
		return false;
	}
	if(m_PicSet.CanAppend())
	{
		num=(int)p_UsrPicture.GetCount();
		for(int i=0;i<num;i++)
		{
			m_prgpos=m_prgpos+(int)(i*10/num);
			SetProg(m_prgpos,"����ͼƬ��Ϣ");

			if(p_UsrPicture[i].owner!=(ProMdl)solid) continue;
			if(p_UsrPicture[i].isCheck!=1)   continue;
			indexNo++;
			try
			{
				CString error_info;
				m_PicSet.AddNew();
			    m_PicSet.m_PrtNum=MdlNumber;
				m_PicSet.m_Number=indexNo;
				m_PicSet.m_Name=  p_UsrPicture[i].fileName;
				m_PicSet.m_Note=  p_UsrPicture[i].Note;
				m_PicSet.m_IsMain=p_UsrPicture[i].isMain;
				DBSavePic(&m_PicSet.m_File,p_UsrPicture[i].filePath+p_UsrPicture[i].fileName,&m_PicSet,error_info);
				m_PicSet.Update();
			}
			catch (CDBException* e) {
				AfxMessageBox(e->m_strError);
								
				m_PicSet.Close();
				AfxMessageBox(_T("���ģ�͵�ͼƬ��Ϣ�����У�û�������ݿ⣡"));
				m_PrgCtrl.SetPos(0);
				return false;
			}
			m_PicSet.Requery();
		}
	}
	else
	{
		AfxMessageBox(_T("���ģ�͵Ĺ�ϵʽ�����У�û�������ݿ⣡"));
		SetProg(0,"���ʧ��");
		m_PicSet.Close();
		return false;
	}
	return true;
}
/************************************************************************/
/* �ѵ��ص��ļ���������������                                           */
/************************************************************************/
bool CMdlOkDlg::CopyFilesToFtpServer(long MdlNumber, CString localDir,CString serDir)
{
	
	CFtpFile     ftp;
	SetProg(m_prgpos+5,"���ڱ���ģ��");
	ftp.CreateDirectory(myFtp,serDir);
	//��û��ʵ��
	if(!ftp.UploadDirectorytoDirectory(localDir,serDir,myFtp))
	{
		AfxMessageBox(_T("����ģ���ļ���������������ʧ�ܣ�"));
		m_PrgCtrl.SetPos(0);
		return false;
	}	
	SetProg(m_prgpos+5,"���ڱ���ģ��");
	return true;
}
/************************************************************************/
/* ���ʳߴ�ķ��ŵ�                                                     */
/************************************************************************/
ProError RvSolidDimensionVisAct(ProDimension *dimension,
							  ProError status,CArray<UsrSWTXTable1,UsrSWTXTable1> * p_RvDimPara)
{

	int                num=0;
	CString            id="";
	CString            csvalue;
	ProSolid           solid;    

	ProMdlCurrentGet((ProMdl*)&solid);
	id.Format("d%d",dimension->id);
    
    //�ж�����ߴ��Ƿ��Ѿ����������Ա��У�����ǵ��򷵻�
	num=(int)p_UsrSWTXTableList.GetCount();

	for(int i=0;i<num;i++)
	{
		if(p_UsrSWTXTableList[i].owner!=solid) continue;
		if(p_UsrSWTXTableList[i].isCheck!=1)   continue;
		if(p_UsrSWTXTableList[i].id==id) 
		{
			//����
			return PRO_TK_NO_ERROR;
		}
	}
    UsrSWTXTable1       temp_UsrSWTXTable(solid,dimension);
	p_RvDimPara->Add(temp_UsrSWTXTable);              //��ӵ�������

	return PRO_TK_NO_ERROR;
}
/************************************************************************/
/* �������ʷ��ʺ���                                                     */
/************************************************************************/
ProError RvParameterActionFn2(ProParameter *Param,ProError status,
							CArray<UsrSWTXTable1,UsrSWTXTable1> * p_RvDimPara)
{
	
	int num;                     //��¼�Զ���ṹp_UsrSWTXTableList�����ݴ�С
	ProSolid        solid;
	
	CString           id;
	CString           csValue;
	CString           csType;
    ProParameter      temp_para;
	temp_para=*Param;
	//��õ�ǰģ��
	ProMdlCurrentGet((ProMdl*)&solid);
	//��ò�����ID
	id=CString(temp_para.id);
	//ȡ���Ѿ����������Ա��е���,����ID
    num=(int)p_UsrSWTXTableList.GetCount();
    for(int i=0;i<num;i++)
	{
		if(p_UsrSWTXTableList[i].owner!=solid) continue;
		if(p_UsrSWTXTableList[i].isCheck!=1) continue;
		if(p_UsrSWTXTableList[i].id==id) 
		{
			//����
			return PRO_TK_NO_ERROR;
		}
	}
	UsrSWTXTable1   temp_UsrSWTXTable(solid,Param);
	p_RvDimPara->Add(temp_UsrSWTXTable);              //��ӵ�������

	return PRO_TK_NO_ERROR;

}
/************************************************************************/
/* �������˺���,ֵ����DOUBLe                                            */
/************************************************************************/
ProError RvParameterFilterFn2(ProParameter *Param,ProError status,
							CArray<UsrSWTXTable1,UsrSWTXTable1> * p_RvDimPara)
{ 
	ProParamvalueType r_type;
	ProParamvalue proval;
	ProParameterValueGet(Param,&proval);
	ProParamvalueTypeGet(&proval,&r_type);
	if(r_type==PRO_PARAM_DOUBLE||r_type==PRO_PARAM_INTEGER)
	{
		return PRO_TK_NO_ERROR;
	}
	return PRO_TK_CONTINUE;
}
/************************************************************************/
/* �����سߴ磬��������Ϣ                                             */
/************************************************************************/
bool CMdlOkDlg::GetRvDimPara(ProSolid solid)
{
	CStringArray                        temp_rv;
//	ProModelitem                        modelitem;
	int                                 num=0;
	CArray<UsrSWTXTable1,UsrSWTXTable1>   p_temp_DimPara;
	p_Rv_UsrTableList.RemoveAll();             //����ڲ����е�����
	for (int i=0;i<p_UsrSWTXTableList.GetCount();i++)
	{
		if (p_UsrSWTXTableList[i].owner==solid && p_UsrSWTXTableList[i].isCheck==1)
		{
			p_temp_DimPara.Add(p_UsrSWTXTableList[i]);
		}
	}
    g_RelCollection.GetRvDimAndPara(&p_temp_DimPara,solid,&p_Rv_UsrTableList);
	return true;
}
BOOL CMdlOkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	USR_SET_DLG_ICON()
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (p_db==NULL) {
		AfxMessageBox("���ݿ�û�����ӣ�");
		return FALSE;
	}
    m_PrgCtrl.SetRange(0,100);
	SetProg(0,"�Ƿ�Ҫȷ������ģ�ͣ�����Ѿ������ϣ���ȷ�ϣ�");
	m_pSet.Open();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣��OCX ����ҳӦ���� FALSE
}


/************************************************************************/
/* ����Ĭ�ϵ�ʵ��                                                       */
/************************************************************************/
bool CMdlOkDlg::InsertDefaultMdlInst(long MdlNumber)
{
	//CPartInsTableSet          tableSet(p_db);
	//CpartInsValueTableSet     vSet(p_db);
	//CpartInsPerfValueTableSet pSet(p_db);
	//CpartPerfparaTableSet     m_PerfSet(p_db);	
	//CpartVariantTableSet      m_variantset(p_db);
	//if(m_pSet.IsOpen()) m_pSet.Close();
	//m_pSet.m_strFilter.Format("[Number]=%ld",MdlNumber);
	//m_pSet.Open();
	//m_pSet.MoveFirst();
	//tableSet.Open();
	//tableSet.AddNew();
	//tableSet.m_PrtNum=MdlNumber;
	//tableSet.m_InsNum=1;
	//tableSet.m_InsCode=m_pSet.m_MdlCode+".0";
	//tableSet.m_InsName="Ĭ��ʵ��";
	//tableSet.m_InsNote="ģ�͵ĳ�ʼֵ,��Ʒ��ʵ��,��ϵͳ�Զ�����";
	//tableSet.m_InsCreaterName=m_pSet.m_MdlerName;
	//tableSet.m_InsCreateDpt=m_pSet.m_MdlerDepartment;
	//tableSet.Update();
	//m_variantset.m_strFilter.Format("[PrtNum]=%ld",MdlNumber);
	//m_variantset.Open();
	//vSet.Open();
	//if (!m_variantset.IsBOF())
	//{
	//	m_variantset.MoveFirst();
	//	do {
	//		vSet.AddNew();
	//		vSet.m_PrtNum=MdlNumber;
	//		vSet.m_InsNum=1;
	//		vSet.m_Num=m_variantset.m_Number;
	//		vSet.m_Value=m_variantset.m_Value;
	//		vSet.Update();
	//		m_variantset.MoveNext();
	//	} while(!m_variantset.IsEOF());
	//}
	//m_PerfSet.m_strFilter.Format("[PrtNum]=%ld",MdlNumber);
	//m_PerfSet.Open();
	//pSet.Open();
	//if (!m_PerfSet.IsBOF())
	//{
	//	m_PerfSet.MoveFirst();
	//	do {
	//		pSet.AddNew();
	//		pSet.m_PrtNum=MdlNumber;
	//		pSet.m_InsNum=1;
	//		pSet.m_Num=m_PerfSet.m_Number;
	//		pSet.m_Value=m_PerfSet.m_Value;
	//		pSet.Update();
	//		m_PerfSet.MoveNext();
	//	} while(!m_PerfSet.IsEOF());
	//}
	//m_pSet.Close();
	return true;
}
void CMdlOkDlg::SetProg(int cur,CString message)
{
	if (bState)
	{
		m_TextEdit.SetWindowText(message);
		m_PrgCtrl.SetPos(cur);
		m_prgpos=cur;
	}
}
bool CMdlOkDlg::UpdateMdl(ProSolid solid=NULL,long MdlNumber=-1,CDatabase * db=NULL)
{
	CString							serDir;
	CString							tempPathName;  //g:/abc/bcd_temp
	CpartVariantTableSet			m_set1(db);
	CpartPerfparaTableSet			m_set2(db);
	CpartRelationSet				m_set3(db);
	CPartRelativeDimParaTableSet    m_set4(db);
	CpartPicSet						m_set5(db);
	CFtpFile						ftp;
	UsrPartPorp						temp_partporp;
	CString							temp;
	CTime							time=CTime::GetCurrentTime();
	bState=false;
	if(!CheckMdlInfo(solid)) return false;
	if(!GetRvDimPara(solid)) return false;
	if(!CreateTempDir(solid, &tempPathName)) 
	{
		AfxMessageBox(_T("����ģ�͹����г������ȱ����ģ�ͣ�"));
		return false;
	}
	try{
			if(db==NULL || MdlNumber<=0) 
			{
				AfxMessageBox("��Ϣ��������");
				return false;
			}
			p_db=db;
			CString							sql;

			if(!p_db->CanTransact()) 
			{
				AfxMessageBox("���ݿⲻ֧��������ʧ��");
				return false;
			}
			if(!p_db->BeginTrans())
			{
				AfxMessageBox("���ݿⲻ֧��������ʧ��");
				return false;
			}
			m_pSet.m_pDatabase=p_db;
			if(!m_pSet.IsOpen()) 
			{
				m_pSet.Open();
			}
			m_pSet.m_strFilter.Format("[Number]=%ld",MdlNumber);
			m_pSet.Requery();
			if (m_pSet.IsBOF())
			{
				AfxMessageBox("ģ�Ͳ�����");
				return false;
			}
			serDir=m_pSet.m_SolidFilePlace;
			if (serDir.Find("/���ģ��/",0)==-1)
			{
				AfxMessageBox("ԭ��ģ����Ϣ���ƻ�");
				return false;
			}
			//�����û����ʹ��
			//ɾ��ԭ����
			//ɾ��ԭ�е���Ϣ
			sql.Format("DELETE FROM %s  WHERE [PrtNum]=%ld ",m_set1.GetDefaultSQL(),MdlNumber);
			p_db->ExecuteSQL(sql);
			sql.Format("DELETE FROM %s  WHERE [PrtNum]=%ld ",m_set2.GetDefaultSQL(),MdlNumber);
			p_db->ExecuteSQL(sql);
			sql.Format("DELETE FROM %s  WHERE [PrtNum]=%ld ",m_set3.GetDefaultSQL(),MdlNumber);
			p_db->ExecuteSQL(sql);
			sql.Format("DELETE FROM %s  WHERE [PrtNum]=%ld ",m_set4.GetDefaultSQL(),MdlNumber);
			p_db->ExecuteSQL(sql);
			sql.Format("DELETE FROM %s  WHERE [PrtNum]=%ld ",m_set5.GetDefaultSQL(),MdlNumber);
			p_db->ExecuteSQL(sql);
			if (!ftp.DeleteDirAndFile(serDir,myFtp))
			{
				AfxMessageBox("ԭ������ļ�û��ɾ����ʧ��");
				p_db->Rollback();
				return false;
			}

			temp_partporp=GetMdlInfo(solid);
			m_pSet.Edit();
			m_pSet.m_Name=temp_partporp.Name;
			m_pSet.m_Code=temp_partporp.Code;
			m_pSet.m_Designer=temp_partporp.Person;
			m_pSet.m_Material=temp_partporp.Material;
			m_pSet.m_TypeNum=temp_partporp.TypeNum;
			m_pSet.m_Datatime=time;
			m_pSet.m_SolidFilePlace=serDir;
			if (temp_partporp.bHaveDraw)
			{
				m_pSet.m_DrawingFileName=GetMdlFileName(temp_partporp.hDraw);
				m_pSet.m_DrawingFilePlace=serDir+"/drawing";	
			}
			else
			{
				m_pSet.m_DrawingFileName.Empty();
				m_pSet.m_DrawingFilePlace.Empty();
			}
			m_pSet.m_SolidFileName=GetMdlFileName(temp_partporp.owner);
			m_pSet.Update();
			m_pSet.Requery();	
			m_pSet.MoveLast();

			InsertMdlVariantTableToDb(solid,MdlNumber);
			InsertMdlRelToDb(solid,MdlNumber);
			InsertMdlPerfToDb(solid,MdlNumber);
			InsertMdlRvToDb(solid,MdlNumber);
			InsertMdlPicToDb(solid,MdlNumber);
			//InsertDefaultMdlInst(MdlNumber);
			if(!CopyFilesToFtpServer(MdlNumber,tempPathName,serDir))
			{
				p_db->Rollback();
				RemoveDireAndFile(tempPathName);
				return false;
			}
			p_db->CommitTrans();
			RemoveDireAndFile(tempPathName);
			SetMdlInfo(solid)->Number = MdlNumber;
			SetMdlInfo(solid)->MdlType= 3;        //��ǰ����Ѿ���Ϊģ��
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		RemoveDireAndFile(tempPathName);
		SetProg(0,"����ʧ�ܣ�");
		p_db->Rollback();
		return false;
	}
	return true;
}
void CMdlOkDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: �ڴ������Ϣ����������
	UsrRepaintWindow();
}
