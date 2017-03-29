#include "StdAfx.h"
#include "asmtempsave.h"
#include "AsmTempCompVariantTable.h"
#include "AsmTempDrwTable.h"
#include "AsmTempMakeupTable.h"
#include "AsmTempPerformanceTable.h"
#include "AsmTempPic.h"
#include "AsmTempRelationTable.h"
#include "AsmTempVariantTable.h"
#include "FtpFile.h"

CAsmTempSave::CAsmTempSave(void):asm_num(-1)
,fileDirectory("")
,fileLodcalDir("")
,asm_father_num(-1)
{
}

CAsmTempSave::~CAsmTempSave(void)
{
}
/************************************************************************/
/* �ύģ��, ����һϵ�еļ��                                           */
/************************************************************************/
BOOL CAsmTempSave::Save(ProSolid solid,long fatherNum,CDatabase * p_db)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pSet=new CAsmTempManageTableSet(p_db);
	asm_solid=solid;
	asm_father_num=fatherNum;
	try
	{
		MarkComp(asm_solid,0);
	/*	if(!ReplaceRelPostIdWithMdlNum()) return FALSE;*/
		if(!GetSWTXItem()) return FALSE;
		m_pSet->Open();
		if(!m_pSet->m_pDatabase->CanTransact()) return FALSE;
		m_pSet->m_pDatabase->BeginTrans();
		if(!InsertAsmPropInfo()){
			m_pSet->m_pDatabase->Rollback();
			delete m_pSet;
			return FALSE;
		}
		if(!InsertAsmMakeupInfo()){
			m_pSet->m_pDatabase->Rollback();
			delete m_pSet;
			return FALSE;
		}//28
		if(!InsertSWTX()){
			m_pSet->m_pDatabase->Rollback();
			delete m_pSet;
			return FALSE;
		}//28
		if(!InsertPerfInfo()){
			m_pSet->m_pDatabase->Rollback();
			delete m_pSet;
			return FALSE;
		}//28
		//if(!InsertRelInfo()){
		//	m_pSet->m_pDatabase->Rollback();
		//	delete m_pSet;
		//	return FALSE;
		//}//28
		if(!InsertAsmPicInfo()){
			m_pSet->m_pDatabase->Rollback();
			delete m_pSet;
			return FALSE;
		}//28
		//if(!InsertDrwInfo()){
		//	m_pSet->m_pDatabase->Rollback();
		//	delete m_pSet;
		//	return FALSE;
		//}//28
		if(!CreateTempAsmDirectory(asm_solid,&fileLodcalDir)){
			m_pSet->m_pDatabase->Rollback();
			delete m_pSet;
			return FALSE;
		}//28
		if(!CopyLocalDirToServer()) //100
		{
			RemoveDireAndFile(fileLodcalDir);
			m_pSet->m_pDatabase->Rollback();
			delete m_pSet;
			return FALSE;
		}
		RemoveDireAndFile(fileLodcalDir);
		m_pSet->m_pDatabase->CommitTrans();
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		m_pSet->m_pDatabase->Rollback();
		delete m_pSet;
		return FALSE;
	}
	delete m_pSet;
	return TRUE;
}
/************************************************************************/
/* ��װ�����Ԫ�����б��,��0��ʼ���,������װ���屾��Ҳ���Ϊ0         */
/************************************************************************/
void CAsmTempSave::MarkComp(ProSolid comp, int upLevel)
{
	asm_comp_item                 tempitem;
	ProError                      status;
	ProFeattype                   ftype;
	ProSolid                      mdl;
	CArray<ProFeature,ProFeature> feat_list;
	tempitem.mdl=comp;
	tempitem.upLevel=upLevel;
	tempitem.num=(int)p_AsmCompItem.GetCount();
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

///************************************************************************/
///* ��ģ�͵ı�ţ��滻��ϵʽ�еĺ�׺                                     */
///************************************************************************/
//BOOL CAsmTempSave::ReplaceRelPostIdWithMdlNum(void)
//{
//
//	int                           num=0;
//	int                           cur=0;
//	CArray<CRel,CRel&>			  p_RelList;
//	CArray<int,int&>			  p_IndexList;
//	g_RelCollection.GetSolidAllRel(asm_solid,true,&p_RelList,&p_IndexList);
//	//num=(int)p_AsmRelItemList.GetCount();
//	//for (int i=0;i<num;i++)
//	//{
//	//	if(p_AsmRelItemList[i].owner!=asm_solid) continue;
//	//	tempRelList.Add(p_AsmRelItemList[i]);
//	//}
//	num=(int)p_RelList.GetCount();
//	for (int i=0;i<num;i++)
//	{
//		AsmRelItem	tempItem;
//		tempItem.owner=asm_solid;
//		p_RelList[i].GetRelStringAsComp(tempItem.relation);
//		tempItem.note=p_RelList[i].GetRelNote();
//		tempRelList.Add(tempItem);
//	}
//
//	num=(int)tempRelList.GetCount();
//	for (int i=0;i<num;i++)
//	{
//		//�滻
//		cur=0;
//		int tempCur1=0;
//		do {
//			cur=tempCur1;
//			tempCur1=tempRelList[i].relation.Find(':',cur);
//			CString postID_temp;
//			if (tempCur1!=-1)
//			{
//				int tempCur2=tempCur1;
//				int Flag=0;
//				postID_temp=tempRelList[i].relation.Tokenize(":+-*/()=<>{}[]^%",tempCur2);
//				for (int j=0;j<(int)p_AsmCompItem.GetCount();j++)
//				{
//					CString postID;
//					postID.Format("%d",p_AsmCompItem[j].postId);
//					if (postID_temp==postID)
//					{
//						tempRelList[i].relation.Delete(tempCur1+1,postID_temp.GetLength());
//						postID.Format("%d",p_AsmCompItem[j].num);
//						tempRelList[i].relation.Insert(tempCur1+1,postID);						
//						tempCur1=tempCur1+1;
//						Flag=1;
//						break;
//					}
//				}
//				if (Flag==0)
//				{
//					return FALSE;
//				}
//
//			}
//		} while(tempCur1!=-1);
//	}
//	return TRUE;
//}
/************************************************************************/
/* ���ģ�͵ı��                                                       */
/************************************************************************/
int CAsmTempSave::GetSolidNum(ProSolid solid)
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
BOOL CAsmTempSave::GetSWTXItem(void)
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
	num=(int)p_AsmSmlItemList.GetCount();
	for (int i=0;i<num;i++) {
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
/* �����ݿ����ģ�͵Ļ�����Ϣ,ע�⣺���ݿ����������Ѿ���              */
/************************************************************************/
BOOL CAsmTempSave::InsertAsmPropInfo(void)
{
	int					indexNo=-1;
	CString				FatherDirectory;
	UsrPartPorp         temp_porp;
	temp_porp=GetMdlInfo(asm_solid);
	//4.�����Ϣ
	m_pSet->AddNew();
	m_pSet->m_FatherNumber =asm_father_num;
	m_pSet->m_Name = temp_porp.Name;
	m_pSet->m_Type = _T("mdl");		
	m_pSet->m_MdlCode =temp_porp.Code;
	m_pSet->m_Note = temp_porp.Note;
    m_pSet->m_FileName = GetMdlFileName(asm_solid);
	//
	m_pSet->m_FileDirectory ="��";
	m_pSet->m_MdlerName =temp_porp.Person;
	m_pSet->m_MdlTypeNum =temp_porp.TypeNum;
	m_pSet->m_MdlTypeName=temp_porp.TypeName;
	CTime time=CTime::GetCurrentTime();
	m_pSet->m_Datatime=time;
	m_pSet->Update();
	m_pSet->Requery();
	m_pSet->MoveLast();
	m_pSet->Edit();
	fileDirectory.Format("/��ģ������/���/%ld\0",m_pSet->m_Number);
	m_pSet->m_FileDirectory=fileDirectory;
	if (temp_porp.bHaveDraw)
	{
		m_pSet->m_DrawFileName=GetMdlFileName(temp_porp.hDraw);
		m_pSet->m_DrawFileDirectory=fileDirectory;
	}
	m_pSet->Update();
	asm_num=m_pSet->m_Number;	    //��ø���ӵı�ʶ��	
	return TRUE;
}
/************************************************************************/
/* ���������ģ�͵�ģ�������Ϣ                                         */
/************************************************************************/
BOOL CAsmTempSave::InsertAsmMakeupInfo(void)
{
	int					num=0;
	int					indexNo=-1;
	int					compItem=-1;
	UsrPartPorp         temp_porp;
	CAsmTempMakeupTable       m_Set(m_pSet->m_pDatabase);
	num=(int)p_AsmCompItem.GetCount();
	m_Set.Open();
	//�ӿ�ʼ����Ϊװ���
	for (int i=1;i<num;i++)
	{
		temp_porp=GetMdlInfo(p_AsmCompItem[i].mdl);
		m_Set.AddNew();
		m_Set.m_AsmNum    =  asm_num;
		m_Set.m_ComNum    =  p_AsmCompItem[i].num;
		m_Set.m_PartMdlNum=  temp_porp.MdlNum;   //Ԫ������������ģ�ͣ����Ԫ��û��ģ����Ϊ-1
		m_Set.m_PartNum  =   temp_porp.Number;
		m_Set.m_UpComNum =   p_AsmCompItem[i].upLevel;
		m_Set.m_Name     =   temp_porp.Name;
		m_Set.m_FileName =   GetMdlFileName(temp_porp.owner).MakeUpper();
		m_Set.m_Type     =   GetMdlFileName(temp_porp.owner).Right(3).MakeLower();//_T("asm"):_T("prt");
		m_Set.m_Note     =  temp_porp.Note;
		m_Set.Update();
		m_Set.Requery();
	}
	return TRUE;
}
/************************************************************************/
/*  �����ݿ����ģ�͵��������Ա���Ϣ                                    */
/************************************************************************/
BOOL CAsmTempSave::InsertSWTX(void)
{
	int        num=0;
	CAsmTempVariantTable m_Set(m_pSet->m_pDatabase);
	m_Set.Open();
	num=(int)P_temp_AsmSmlItem.GetCount();
	for (int i=0;i<num;i++)
	{
		m_Set.AddNew();
		m_Set.m_AsmNum=asm_num;
		m_Set.m_Name=P_temp_AsmSmlItem[i].Name;
		m_Set.m_Number=i+1;
		m_Set.m_PostVariant=P_temp_AsmSmlItem[i].solid_num;
		m_Set.m_Type=P_temp_AsmSmlItem[i].type;
		m_Set.m_IdNum=P_temp_AsmSmlItem[i].id;
		m_Set.m_DimSymbol=P_temp_AsmSmlItem[i].DimSymbol;
		m_Set.m_ParaFeatName=P_temp_AsmSmlItem[i].featName;
		m_Set.m_SubType=P_temp_AsmSmlItem[i].SubType;
		m_Set.m_Value=P_temp_AsmSmlItem[i].Value;
		m_Set.m_UpLimit=P_temp_AsmSmlItem[i].UpLimit;
		m_Set.m_DwLimit=P_temp_AsmSmlItem[i].DwLimit;
		m_Set.m_Note=P_temp_AsmSmlItem[i].Note;
		m_Set.Update();
		m_Set.Requery();
	}	
	return TRUE;
}
///************************************************************************/
///* �����ݿ����ģ�͵Ĺ�ϵʽ��Ϣ                                         */
///************************************************************************/
//BOOL CAsmTempSave::InsertRelInfo(void)
//{
//	int        num=0;
//	CAsmTempRelationTable m_Set(m_pSet->m_pDatabase);
//	num=(int)tempRelList.GetCount();
//	m_Set.Open();
//	for (int i=0;i<num;i++)
//	{
//		m_Set.AddNew();
//		m_Set.m_AsmNum=asm_num;
//		m_Set.m_Number=i+1;
//		m_Set.m_Relation=tempRelList[i].relation;
//		m_Set.m_Note=tempRelList[i].note;
//		m_Set.Update();
//		m_Set.Requery();
//	}
//	return TRUE;
//}
/************************************************************************/
/* �����ݿ����ģ�͵����ܲ�����Ϣ                                       */
/************************************************************************/
BOOL CAsmTempSave::InsertPerfInfo(void)
{
	int         num=0;
	int         indexNo=0;
	CAsmTempPerformanceTable m_Set(m_pSet->m_pDatabase);
	m_Set.Open();
	num=(int)p_UsrPerfParaTableList.GetCount();
	for (int i=0;i<num;i++)
	{
		if(p_UsrPerfParaTableList[i].owner!=(ProMdl)asm_solid) continue;
		if(p_UsrPerfParaTableList[i].isCheck!=1)               continue;
		indexNo++;
		m_Set.AddNew();
		m_Set.m_AsmNum=asm_num;
		m_Set.m_Number=indexNo;
		m_Set.m_Name=p_UsrPerfParaTableList[i].Name;
		m_Set.m_Value=p_UsrPerfParaTableList[i].value;
		m_Set.m_Type   = p_UsrPerfParaTableList[i].Type;
		m_Set.m_MaxVal = p_UsrPerfParaTableList[i].MaxValue;
		m_Set.m_MinVal = p_UsrPerfParaTableList[i].MinValue;
		m_Set.Update();
		m_Set.Requery();
	}
	return TRUE;
}
/************************************************************************/
/*  �����ݿ�ͼƬ��Ϣ                                                    */
/************************************************************************/
BOOL CAsmTempSave::InsertAsmPicInfo(void)
{
	int                        num=0;
	int                        indexNo=0;
	CArray<ProSolid,ProSolid>  solidlist;
	CAsmTempPic m_Set(m_pSet->m_pDatabase);	
	solidlist.Add(asm_solid);
	GetAsmCompHand(asm_solid,&solidlist);
	num=(int)p_UsrPicture.GetCount();
	m_Set.Open();
	for (int i=0;i<num;i++) {
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

BOOL CAsmTempSave::CopyLocalDirToServer(void)
{
	CFtpFile					ftp;
	BOOL						isok=TRUE;
	CString						drawDir1;
	CString						backupDir;
	CString						backdrawDir;
	CStringArray				strDirAry;
	CString						tempfilename;
	CArray<ProSolid,ProSolid>   solidlist;
	drawDir1 =					fileDirectory+ "/drawing" ;
	backupDir=					fileDirectory+ "/backup"  ;
	backdrawDir=				fileDirectory+ "/backup/drawing" ;
	strDirAry.Add(fileDirectory);
	strDirAry.Add(drawDir1);
	//strDirAry.Add(backupDir);
	//strDirAry.Add(backdrawDir);

	CInternetSession* pSession;
	CFtpConnection* pConnection;
	CFtpFileFind* pFileFind;
	pConnection=NULL;
	pFileFind=NULL;

	pSession=new CInternetSession();
	try
	{
		pConnection=pSession->GetFtpConnection(myFtp.strFtp,myFtp.strFtpUser,myFtp.strFtpPwd,myFtp.iFtpPort);
	}
	catch(CInternetException* e)
	{
		e->Delete();
		pConnection=NULL;
		isok=FALSE;
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
		tempfilename=GetMdlFileName(asm_solid)+".1";
		BOOL c=pConnection->PutFile(fileLodcalDir+tempfilename,strDirAry[0]+"\\"+tempfilename);
		//2.���䶥������Ĺ���ͼ
		if(GetMdlInfo(asm_solid).bHaveDraw) 
		{
			tempfilename=GetMdlFileName(GetMdlInfo(asm_solid).hDraw)+".1";
			BOOL c=pConnection->PutFile(fileLodcalDir+"drawing\\"+tempfilename,strDirAry[1]+"\\"+tempfilename);
		}
		//3.��������ģ���ļ��������ļ�
		//GetAsmCompHand(asm_solid,&solidlist);
		//for (int i=0;i<solidlist.GetCount();i++)
		//{
		//	tempfilename=GetMdlFileName(solidlist[i])+".1";
		//	BOOL c=pConnection->PutFile(fileLodcalDir+tempfilename,strDirAry[2]+"\\"+tempfilename);
		//	if(!c) isok=c;
		//	if(GetMdlInfo(solidlist[i]).bHaveDraw) 
		//	{
		//		tempfilename=GetMdlFileName(GetMdlInfo(solidlist[i]).hDraw)+".1";
		//		BOOL c=pConnection->PutFile(fileLodcalDir+"drawing\\"+tempfilename,strDirAry[3]+"\\"+tempfilename);
		//		if(!c) isok=c;
		//	}
		//}
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return TRUE;
}