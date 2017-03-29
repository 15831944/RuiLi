#include "StdAfx.h"
#include "asmtempopen.h"
#include "AsmTempCompVariantTable.h"
#include "AsmTempDrwTable.h"
#include "AsmTempMakeupTable.h"
#include "AsmTempPerformanceTable.h"
#include "AsmTempPic.h"
#include "AsmTempRelationTable.h"
#include "AsmTempVariantTable.h"
#include "UsrSWTXTable.h"
#include "FtpFile.h"
#include "NProductInsInfoTable.h"
#include "NAsmType.h"
#include "AsmManageTableSet.h"
//#include "AsmInsInfoTable.h"

#include "NPartInsInfoTable.h"
#include "NPartTypeSet.h"
#include "partPerfparaTableSet.h"
#include "partVariantTableSet.h"
#include "partPicSet.h"
#include "partRelationSet.h"
CAsmTempOpen::CAsmTempOpen(void):asm_num(-1)
     ,asm_solid(NULL)
	 ,localDir(_T("")) 
	 ,m_pSet(NULL)
	 ,asm_drw(NULL)
{
}

CAsmTempOpen::~CAsmTempOpen(void)
{
}
/************************************************************************/
/* ��ģ��                                                             */
/************************************************************************/
BOOL CAsmTempOpen::Open(CDatabase * p_db,long num)
{
	m_pSet=new CAsmTempManageTableSet(p_db);	
	asm_num=num;
	m_pSet->m_strFilter.Format("[Number]=%ld",asm_num);
	m_pSet->Open();
	if (m_pSet->IsBOF())
	{
		AfxMessageBox("��ǰģ���Ѿ������ڣ�");
		delete m_pSet;
		return FALSE;
	}
	if (!CheckCurrentMdl(num))
	{
		delete m_pSet;
		return FALSE;
	}
	if (!LoadSolid(num)) 
	{
		delete m_pSet;
		return FALSE;
	}
	if(!AddAsmSolidInfo(num))
	{
		delete m_pSet;
		return FALSE;
	}
	if (!AddCompInfo(num))
	{
		delete m_pSet;
		return FALSE;
	}
	if(!GetParaPicDrwInfo(num)) 
	{
		delete m_pSet;
		return FALSE;
	}
	delete m_pSet;
	return TRUE;
}
BOOL CAsmTempOpen::CheckCurrentMdl(long num)
{
	CNProductInsInfoTable   asm_set(m_pSet->m_pDatabase);
	CAsmTempMakeupTable     bom_set(m_pSet->m_pDatabase);
	CAsmTempManageTableSet  asm_m_set(m_pSet->m_pDatabase);
	CNPartInsInfoTable      prt_ins_set(m_pSet->m_pDatabase);
    CString					error_info;
	//�õ�����Ԫ���Ĵ��λ��
	bom_set.m_strFilter.Format("[AsmNum]=%ld",num);
	bom_set.Open();
	prt_ins_set.Open();
	asm_set.Open();
	if (!bom_set.IsBOF())
	{
		bom_set.MoveFirst();		
		do {
			error_info.Empty();
			//����㲿���Ĵ��λ�ø���ַ
			if (bom_set.m_Type.MakeLower()=="asm" && bom_set.m_PartNum>0 )
			{
				asm_set.m_strFilter.Format("[PdtNum]=%ld",bom_set.m_PartNum);
				asm_set.Requery();
				if (!asm_set.IsBOF())
				{
					asm_set.MoveFirst();
					if (UsrCheckCurrentMdl(PRO_MDL_ASSEMBLY,asm_set.m_SolidFileName,asm_set.m_PdtNum)==CHECK_SNAME_NNUM)
					{
						error_info.Format("ϵͳ���Ѿ�����ͬ���ļ���(%s)����",asm_set.m_SolidFileName);
						error_info.Format("%s�Ƿ������(���ܻ�ʹװ��ʧ��)!",error_info);
						if (AfxMessageBox(error_info,MB_YESNO|MB_ICONQUESTION)!=IDYES)
						{
							return FALSE;
						}
					}
				}
			}
			else if(bom_set.m_Type.MakeLower()=="prt" && bom_set.m_PartNum>0)
			{
				prt_ins_set.m_strFilter.Format("[PrtNum]=%ld",bom_set.m_PartNum);
				prt_ins_set.Requery();
				if (!prt_ins_set.IsBOF())
				{
					prt_ins_set.MoveFirst();
					//////////////////////////////////////////////////////////////////////////
					if (UsrCheckCurrentMdl(PRO_MDL_PART,prt_ins_set.m_SolidFileName,prt_ins_set.m_PrtNum)==CHECK_SNAME_NNUM)
					{
						error_info.Format("ϵͳ���Ѿ�����ͬ���ļ���(%s)����",prt_ins_set.m_SolidFileName);
						error_info.Format("%s�Ƿ������(���ܻ�ʹװ��ʧ��)!",error_info);
						if (AfxMessageBox(error_info,MB_YESNO|MB_ICONQUESTION)!=IDYES)
						{
							return FALSE;
						}
					}
					
				}
			}
			bom_set.MoveNext();
		} while(!bom_set.IsEOF());
	}
	return TRUE;
}
/************************************************************************/
/* ��ģ�͵�����                                                         */
/************************************************************************/
BOOL CAsmTempOpen::LoadSolid(long num)
{
	CFtpFile                m_ftp;
    ProError                status;
    ProPath                 name;
	ProType                 type;
	ProName                 w_name;
	CStringArray			files,localfiles;       //��Ҫ���ص��ļ�,��¼�ļ��ڷ������ϵĴ��λ������
	CString                 fileTemp,csError,fullFileName;
	int                     w_id;

	CNProductInsInfoTable   asm_set(m_pSet->m_pDatabase);
	CAsmTempMakeupTable     bom_set(m_pSet->m_pDatabase);
	CAsmTempManageTableSet  asm_m_set(m_pSet->m_pDatabase);
	CNPartInsInfoTable      prt_ins_set(m_pSet->m_pDatabase);

	//�õ�����Ԫ���Ĵ��λ��
	bom_set.m_strFilter.Format("[AsmNum]=%ld",num);
	bom_set.Open();
	prt_ins_set.Open();
	asm_set.Open();
	//
	if (!bom_set.IsBOF())
	{
		bom_set.MoveFirst();
		do {
			//����㲿���Ĵ��λ�ø���ַ
			if (bom_set.m_Type.MakeLower()=="asm" && bom_set.m_PartNum>0 )
			{
				asm_set.m_strFilter.Format("[PdtNum]=%ld",bom_set.m_PartNum);
				asm_set.Requery();
				if (!asm_set.IsBOF())
				{
					asm_set.MoveFirst();
					if(asm_set.m_PDMState=="�ύ")
					{
						CString  fileTemp1="",fileTemp2="";
						if (!UsrGetPartAndProductFromPdm(asm_set.m_PDMNum,0,fileTemp1,fileTemp2))
						{
							fileTemp2.Format("���%s��PDMϵͳ��û���ҵ�",asm_set.m_SolidFileName );
							AfxMessageBox(fileTemp2);
							return FALSE;
						}
						if(!fileTemp1.IsEmpty()) files.Add(fileTemp1);
						if(!fileTemp2.IsEmpty()) files.Add(fileTemp2);
						//��PDMȥ����
					}
					else if (asm_set.m_LocalState=="����")
					{
						//�ӱ��ص��ԣ�ע����ܲ��Ǳ������ԣ�
						//����Ǳ������ԣ���������
						fileTemp=asm_set.m_SolidFilePlace+"\\"+asm_set.m_SolidFileName;
						int Flag=0;
						for (int i=0;i<localfiles.GetCount();i++)
						{
							if(localfiles[i]==fileTemp) Flag=1;
						}
						if(Flag==0) localfiles.Add(fileTemp);
						//�ж����޹���ͼ
						if (!asm_set.m_DrawingFileName.IsEmpty())
						{
							fileTemp=asm_set.m_DrawingFilePlace +"\\"+asm_set.m_DrawingFileName;
							int Flag=0;
							for (int i=0;i<localfiles.GetCount();i++)
							{
								if(localfiles[i]==fileTemp) Flag=1;
							}
							if(Flag==0) localfiles.Add(fileTemp);
						}
					}
					else if (asm_set.m_LocalState=="������")
					{
						fileTemp=asm_set.m_SolidFilePlace+"/"+asm_set.m_SolidFileName+".1";
						int Flag=0;
						for (int i=0;i<files.GetCount();i++)
						{
							if(files[i]==fileTemp) Flag=1;
						}
						if(Flag==0) files.Add(fileTemp);
						//����ͼ
						if (!asm_set.m_DrawingFileName.IsEmpty())
						{
							fileTemp=asm_set.m_DrawingFilePlace +"/"+asm_set.m_DrawingFileName+".1";
							int Flag=0;
							for (int i=0;i<files.GetCount();i++)
							{
								if(files[i]==fileTemp) Flag=1;
							}
							if(Flag==0) files.Add(fileTemp);
						}
					}
				}
			}
			else if(bom_set.m_Type.MakeLower()=="prt" && bom_set.m_PartNum>0)
			{
				prt_ins_set.m_strFilter.Format("[PrtNum]=%ld",bom_set.m_PartNum);
				prt_ins_set.Requery();
				if (!prt_ins_set.IsBOF())
				{
					prt_ins_set.MoveFirst();
					if(prt_ins_set.m_PDMState=="�ύ")
					{
						CString  fileTemp1="",fileTemp2="";
						if (!UsrGetPartAndProductFromPdm(prt_ins_set.m_PDMNum,0,fileTemp1,fileTemp2))
						{
							fileTemp2.Format("���%s��PDMϵͳ��û���ҵ�",prt_ins_set.m_SolidFileName );
							AfxMessageBox(fileTemp2);
							return FALSE;
						}
						if(!fileTemp1.IsEmpty()) files.Add(fileTemp1);
						if(!fileTemp2.IsEmpty()) files.Add(fileTemp2);
						//��PDMȥ����
					}
					else if (prt_ins_set.m_LocalState=="����")
					{
						//�ӱ��ص��ԣ�ע����ܲ��Ǳ������ԣ�
						//����Ǳ������ԣ���������
						fileTemp=prt_ins_set.m_SolidPlace+"\\"+prt_ins_set.m_SolidFileName;
						int Flag=0;
						for (int i=0;i<localfiles.GetCount();i++)
						{
							if(localfiles[i]==fileTemp) Flag=1;
						}
						if(Flag==0) localfiles.Add(fileTemp);
						//�ж����޹���ͼ
						if (!prt_ins_set.m_DrawingName.IsEmpty())
						{
							fileTemp=prt_ins_set.m_DrawingPlace +"\\"+prt_ins_set.m_DrawingName ;
							int Flag=0;
							for (int i=0;i<localfiles.GetCount();i++)
							{
								if(localfiles[i]==fileTemp) Flag=1;
							}
							if(Flag==0) localfiles.Add(fileTemp);
						}
					}
					else if (prt_ins_set.m_LocalState=="������")
					{
						fileTemp=prt_ins_set.m_SolidPlace +"/"+prt_ins_set.m_SolidFileName+".1";
						int Flag=0;
						for(int i=0;i<files.GetCount();i++)
						{
							if(files[i]==fileTemp) Flag=1;
						}
						if(Flag==0) files.Add(fileTemp);
						//��ӹ���ͼ
						if (!prt_ins_set.m_DrawingName.IsEmpty())
						{
							fileTemp=prt_ins_set.m_DrawingPlace +"/"+prt_ins_set.m_DrawingName+".1";
							int Flag=0;
							for(int i=0;i<files.GetCount();i++)
							{
								if(files[i]==fileTemp) Flag=1;
							}
							if(Flag==0) files.Add(fileTemp);
						}
					}
				}
			}
			bom_set.MoveNext();
		} while(!bom_set.IsEOF());
	}


	//�򿪣�����
	CString		newDir;//�����������ļ���
	if (!SelectAnDirectory(localDir,""))
	{
		return FALSE;
	}	

	asm_m_set.m_strFilter.Format("[Number]=%ld",num);
	asm_m_set.Open();
	if(asm_m_set.IsBOF()) return FALSE;
	asm_m_set.MoveFirst();
	if (!CreateDirectoryGivenPathAndName(localDir,asm_m_set.m_Name,newDir))
	{
		return FALSE;
	}

	localDir=localDir+newDir;//c:\\dd\\new
	files.Add(asm_m_set.m_FileDirectory +"/"+asm_m_set.m_FileName+".1");
	if (!asm_m_set.m_DrawFileName.IsEmpty())
	{
		files.Add(asm_m_set.m_DrawFileDirectory +"/drawing/"+asm_m_set.m_DrawFileName+".1");
	}
	if (!m_ftp.DownloadFilestoDirectory(&files,localDir,myFtp,csError))
	{
		AfxMessageBox(csError);
		return FALSE;
	}
	//2.2��鱾���ļ��Ƿ����
	if (!IsFilesExists(localfiles,csError))
	{
		AfxMessageBox(csError);
		return FALSE;
	}

	//��ģ�͵����ڴ�
	fullFileName=localDir+"\\"+asm_m_set.m_FileName;
	ProStringToWstring(name,fullFileName.GetBuffer());
	status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&asm_solid); 
	if (!asm_m_set.m_DrawFileName.IsEmpty() )
	{
		fullFileName=localDir+"\\"+asm_m_set.m_DrawFileName;
		ProStringToWstring(name,fullFileName.GetBuffer());
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&asm_drw); 
	}
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ģ��ʧ��");
		m_pSet->Close();
		return FALSE;
	}
	status = ProMdlTypeGet(asm_solid, (ProMdlType*)&type);
	status= ProMdlNameGet(asm_solid, w_name);
	status= ProObjectwindowCreate(w_name, type, &w_id);
	if(!status== PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	status = ProMdlDisplay(asm_solid);
	status = ProWindowActivate(w_id);	
	return TRUE;
}
/************************************************************************/
/* ������Ļ�����Ϣ��ӽ���                                             */
/************************************************************************/
BOOL CAsmTempOpen::AddAsmSolidInfo(long num)
{	
	CNAsmType			asm_type(m_pSet->m_pDatabase);
	m_pSet->m_strFilter.Format("[Number]=%ld",num);
	m_pSet->Requery();
	if (!m_pSet->IsBOF())
	{
		m_pSet->MoveFirst();
		UsrPartPorp tempProp;
		tempProp.owner=asm_solid;
		tempProp.Name=m_pSet->m_Name;
		tempProp.Person=m_pSet->m_MdlerName;
		tempProp.ResourceType=1;
		tempProp.Note=m_pSet->m_Note;	
		tempProp.Code=m_pSet->m_MdlCode;
		tempProp.MdlNum=-1;
		tempProp.MdlName="";
		tempProp.TypeNum=m_pSet->m_MdlTypeNum;
		asm_type.m_strFilter.Format("[Number]=%ld",m_pSet->m_MdlTypeNum);
		asm_type.Open();
		if (!asm_type.IsBOF())
		{
			asm_type.MoveFirst();
			tempProp.TypeName=asm_type.m_Name;
			tempProp.TypeNum=m_pSet->m_MdlTypeNum;
		}
		if (asm_drw!=NULL)
		{
			tempProp.bHaveDraw=true;
			tempProp.hDraw=asm_drw;
		}		
		//��ģ����Ϣ��ӵ�ȫ�ֱ���
		SetMdlInfo(asm_solid,tempProp);
		return TRUE;
	}
	return FALSE;
}
/************************************************************************/
/* ���ģ�͵Ĳ�����ͼƬ��(����ͼ��)��ϵ��Ϣ                               */
/************************************************************************/
BOOL CAsmTempOpen::GetParaPicDrwInfo(long asm_num)
{
	CAsmTempPerformanceTable m_Set1(m_pSet->m_pDatabase);
	CAsmTempPic              m_Set2(m_pSet->m_pDatabase);
	//CAsmTempRelationTable    m_Set3(m_pSet->m_pDatabase);
	CAsmTempMakeupTable      bom_set(m_pSet->m_pDatabase);
	CAsmTempVariantTable     m_Set4(m_pSet->m_pDatabase);
	CArray<ProSolid,ProSolid>	solidlist;
	ProSolid					tempsolid;
	ProName						name;
	ProMdlType                  mdltype;
	CArray<long,long>			comnumlist;
	try
	{
		m_Set1.m_strFilter.Format("[AsmNum]=%ld",asm_num);
		m_Set2.m_strFilter.Format("[AsmNum]=%ld",asm_num);
		m_Set4.m_strFilter.Format("[AsmNum]=%ld",asm_num);
        EraseMdlInfo(asm_solid,ERASE_ALL);
		//���Ԫ�����ŵĶ�Ӧ��ϵ
		solidlist.Add(asm_solid);
		comnumlist.Add(0);
		bom_set.m_strFilter.Format("[AsmNum]=%ld",asm_num);
		bom_set.Open();
		if (!bom_set.IsBOF())
		{
			bom_set.MoveFirst();
			do {
				bom_set.m_Type=="prt"?(mdltype=PRO_MDL_PART):(mdltype=PRO_MDL_ASSEMBLY);
				ProStringToWstring(name,bom_set.m_FileName.Left(bom_set.m_FileName.GetLength()-4).GetBuffer());  // ȥ����׺�õ��ļ���
				if (ProMdlInit(name,mdltype,(ProMdl*)&tempsolid)==PRO_TK_NO_ERROR)
				{
					solidlist.Add(tempsolid);
					comnumlist.Add(bom_set.m_ComNum);
				}			
				bom_set.MoveNext();
			} while(!bom_set.IsEOF());
		}

		m_Set1.Open();
		if (!m_Set1.IsBOF())
		{
			m_Set1.MoveFirst();
			do {
				UsrPerfParaTable temp;
				temp.owner=asm_solid;
				temp.Name=m_Set1.m_Name;
				temp.Note=m_Set1.m_Note;
				temp.value=m_Set1.m_Value;
				temp.isCheck=1;
				temp.Type=m_Set1.m_Type;
				temp.MaxValue=m_Set1.m_MaxVal;
				temp.MinValue=m_Set1.m_MinVal;
				p_UsrPerfParaTableList.Add(temp);
				m_Set1.MoveNext();
			} while(!m_Set1.IsEOF());
		}
		m_Set2.Open();
		if (!m_Set2.IsBOF())
		{
			m_Set2.MoveFirst();
			do {
				UsrPicture temp2;
				CString	   error_info;
				CString    temp_full_filename;
				int        tempnum=0;
				CString    mdlName=GetMdlInfo(asm_solid).Name;
				temp2.owner=asm_solid;
				temp2.fileName=mdlName+".jpg";
				temp2.filePath=GetMdlPath(asm_solid);
				temp2.isCheck=1;
				temp2.isMain=m_Set2.m_IsMain;
				temp_full_filename=temp2.filePath+temp2.fileName;
				while (PathFileExistsA(temp_full_filename.GetBuffer()))
				{
					tempnum++;
					temp2.fileName.Format("%s_%d.jpg",mdlName,tempnum);
					temp_full_filename=temp2.filePath+temp2.fileName;
				}
				if (temp2.isMain==1)
				{
					SetMdlInfo(asm_solid)->pic_path=temp2.filePath+temp2.fileName;
				}
				temp2.Note=m_Set2.m_Note;
				DBOpenPicSaveAsFile(&m_Set2.m_File,temp2.filePath+temp2.fileName);
				p_UsrPicture.Add(temp2);
				m_Set2.MoveNext();
			} while(!m_Set2.IsEOF());
		}
		//��ӿɱ������Ϣ
		m_Set4.Open();
		if (!m_Set4.IsBOF())
		{
			m_Set4.MoveFirst();
			do {
				AsmSmlItem temp4;
				CString    csTemp=_T("");
				temp4.owner=asm_solid;
				temp4.name=m_Set4.m_Name;
				temp4.note=m_Set4.m_Note;
				if (m_Set4.m_Type=="dim") 
				{
					temp4.marsk=TYPE_DIM;
					UsrSWTXTable1 swtx1;
					csTemp.Format(":%ld",m_Set4.m_PostVariant);
					swtx1.id=m_Set4.m_IdNum.Left(m_Set4.m_IdNum.GetLength()-csTemp.GetLength());
					swtx1.type=_T("dim");
					for (int i=0;i<solidlist.GetCount();i++)
					{
						if (m_Set4.m_PostVariant==comnumlist[i])
						{
							swtx1.SetDimPara(solidlist[i]);
							temp4.dim=swtx1.Dim;
							p_AsmSmlItemList.Add(temp4);
							break;
						}
					}
				}
				else if(m_Set4.m_Type=="para") 
				{
					temp4.marsk=TYPE_PARA;
					UsrSWTXTable1 swtx2;
					swtx2.type=_T("para");
					csTemp.Format(":%ld",m_Set4.m_PostVariant);
					swtx2.id=m_Set4.m_IdNum.Left(m_Set4.m_IdNum.GetLength()-csTemp.GetLength());
					for (int i=0;i<solidlist.GetCount();i++)
					{
						if (m_Set4.m_PostVariant==comnumlist[i])
						{
							swtx2.SetDimPara(solidlist[i]);
							temp4.para=swtx2.Para;
							p_AsmSmlItemList.Add(temp4);
							break;
						}
					}
				}
				else if(m_Set4.m_Type=="feat") 
				{
					temp4.marsk=TYPE_FEAT;
					p_AsmSmlItemList.Add(temp4);
				}
				m_Set4.MoveNext();
			} while(!m_Set4.IsEOF());
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		return FALSE;
	}
	return TRUE;
}
///************************************************************************/
///* ���ݱ�Ż��ģ�͵ľ��                                               */
///************************************************************************/
//ProSolid CAsmTempOpen::GetComp(int num)
//{
//	for (int i=0;i<(int)p_temp_AsmMdlItemList.GetCount();i++)
//	{
//		if (num==p_temp_AsmMdlItemList[i].num)
//		{
//			return p_temp_AsmMdlItemList[i].mdl;
//		}
//	}
//	return NULL;
//}
/************************************************************************/
/* ���Ԫ����Ϣ                                                         */
/************************************************************************/
BOOL CAsmTempOpen::AddCompInfo(long num)
{
	CAsmTempMakeupTable     bom_set(m_pSet->m_pDatabase);
	CNProductInsInfoTable   asm_set(m_pSet->m_pDatabase);
	CNPartInsInfoTable      prt_ins_set(m_pSet->m_pDatabase);
	ProMdl					mdl;
	ProName					name;
	CString					csName;
	//�õ�����Ԫ���Ĵ��λ��
	bom_set.m_strFilter.Format("[AsmNum]=%ld",num);
	bom_set.Open();
	prt_ins_set.Open();
	asm_set.Open();
	if (!bom_set.IsBOF())
	{
		bom_set.MoveFirst();
		do {
			//����㲿���Ĵ��λ�ø���ַ
			if (bom_set.m_Type.MakeLower()=="asm" && bom_set.m_PartNum>0 )
			{
				asm_set.m_strFilter.Format("[PdtNum]=%ld",bom_set.m_PartNum);
				asm_set.Requery();
				if (!asm_set.IsBOF())
				{
					asm_set.MoveFirst();
					csName=asm_set.m_SolidFileName.Left(asm_set.m_SolidFileName.GetLength()-4);
					ProStringToWstring(name,csName.GetBuffer());
					if (ProMdlInit(name,PRO_MDL_ASSEMBLY,&mdl)==PRO_TK_NO_ERROR)
					{
						//��Ӳ�������Ϣ
						AddCompAsmInfo((ProSolid)mdl,bom_set.m_PartNum);
					}
				}
			}
			else if(bom_set.m_Type.MakeLower()=="prt" && bom_set.m_PartNum>0)
			{
				prt_ins_set.m_strFilter.Format("[PrtNum]=%ld",bom_set.m_PartNum);
				prt_ins_set.Requery();
				if (!prt_ins_set.IsBOF())
				{
					prt_ins_set.MoveFirst();
					csName=prt_ins_set.m_SolidFileName.Left(prt_ins_set.m_SolidFileName.GetLength()-4);
					ProStringToWstring(name,csName.GetBuffer());
					if (ProMdlInit(name,PRO_MDL_PART,&mdl)==PRO_TK_NO_ERROR)
					{
						//��Ӳ�������Ϣ
						AddCompPartInfo((ProSolid)mdl,bom_set.m_PartNum);
					}
				}
			}
			bom_set.MoveNext();
		} while(!bom_set.IsEOF());
	}
	return TRUE;
}
BOOL CAsmTempOpen::AddCompPartInfo(ProSolid mdl,long PrtNum)             //����������Ϣ
{
	CNPartInsInfoTable		m_tempSet(m_pSet->m_pDatabase);	
	CNPartTypeSet           m_parttypeset(m_pSet->m_pDatabase);
	CPartMdlRecordSet       m_partmdlset(m_pSet->m_pDatabase);
	UsrPartPorp             temp_porp;
	ProDrawing              mdl_drw;
	/*ProName					name;
	CString					csName;*/
	m_tempSet.m_strFilter.Format("[PrtNum]=%ld",PrtNum);
	m_tempSet.Open();
	if(!m_tempSet.IsBOF())
	{
		m_tempSet.MoveFirst();
		temp_porp.owner  = mdl;
		temp_porp.Number = PrtNum;
		//�ж��������Ƿ�����ģ��
		switch(m_tempSet.m_MdlNum) {
			case -1:
				temp_porp.MdlType=2;           //��ģ�͵���������
				temp_porp.MdlNum=-1;
				break;
			default:
				temp_porp.MdlType=1;           //��ģ�͵�ʵ��
				temp_porp.MdlNum=m_tempSet.m_MdlNum;
				break;
		}			
		temp_porp.Name   =m_tempSet.m_Name;
		temp_porp.Code   =m_tempSet.m_InsCode;
		//���ģ�͵�����Լ��������
		m_parttypeset.m_strFilter.Format("[Number]=%ld",m_tempSet.m_TypeNum);
		m_parttypeset.Open();
		if (!m_parttypeset.IsBOF())
		{
			m_parttypeset.MoveFirst();
			temp_porp.TypeNum=m_tempSet.m_TypeNum;
			temp_porp.TypeName=m_parttypeset.m_Name;
		}
		else
		{
			temp_porp.TypeNum=-1;
			temp_porp.TypeName="";
		}
		temp_porp.Material=m_tempSet.m_Material;
		temp_porp.Person  ="";
		temp_porp.Note    =m_tempSet.m_Note;
		temp_porp.IsAudited=(m_tempSet.m_AuditState==_T("���ͨ��")?true:false);
		//�ļ�����Դ
		if (m_tempSet.m_PDMState==_T("�ύ"))
			temp_porp.ResourceType=2;
		else if(m_tempSet.m_LocalState==_T("����"))
			temp_porp.ResourceType=0;
		else if(m_tempSet.m_LocalState==_T("������"))
			temp_porp.ResourceType=1;
		//װ�����Ԫ��
		temp_porp.IsAsmComp=true;
		//���ģ�͵�����
		if (m_tempSet.m_MdlNum>0)
		{
			m_partmdlset.m_strFilter.Format("[Number]=%ld",m_tempSet.m_MdlNum);
			m_partmdlset.Open();
			m_partmdlset.Requery();
			if (!m_partmdlset.IsBOF())
			{
				m_partmdlset.MoveFirst();
				temp_porp.MdlName=m_partmdlset.m_Name;
			}
		}
		else temp_porp.MdlName="��ģ��";
		//���ù���ͼ
		if (!m_tempSet.m_DrawingName.IsEmpty() 
			&& LoadCompDrawing(temp_porp.ResourceType,m_tempSet.m_DrawingName,m_tempSet.m_DrawingPlace,&mdl_drw)==PRO_TK_NO_ERROR)
		{
			temp_porp.bHaveDraw=true;
			temp_porp.hDraw=mdl_drw;
		}
		else
		{
			temp_porp.bHaveDraw=false;
			temp_porp.hDraw=NULL;
		}

        SetMdlInfo(mdl,temp_porp);	

		if (temp_porp.MdlType==1)
		{
			//��ģ�͵���Ϣ����ӽ���
			UsrAddPartMdlOtherInfo(m_pSet->m_pDatabase,mdl,m_tempSet.m_MdlNum);
		}
	}
	return TRUE;
}
BOOL CAsmTempOpen::AddCompAsmInfo(ProSolid mdl,long num)              //����������Ϣ
{
	CNProductInsInfoTable asm_set(m_pSet->m_pDatabase);
	CNAsmType			  asm_type(m_pSet->m_pDatabase);
	CAsmManageTableSet    asmmdlset(m_pSet->m_pDatabase);
	ProDrawing            mdl_drw;
	//ProName				  name;
	//CString				  csName;
	asm_set.m_strFilter.Format("[PdtNum]=%ld",num);
	asm_set.Open();
	if (!asm_set.IsBOF())
	{
		asm_set.MoveFirst();
		UsrPartPorp temp_porp;
		temp_porp.owner=mdl;
		temp_porp.Number=num;
		switch(asm_set.m_MdlNum) {
		case -1:
			temp_porp.MdlType=2;
			temp_porp.MdlNum=-1;
			break;
		default:
			temp_porp.MdlType=1;
			temp_porp.MdlNum=asm_set.m_MdlNum;
			break;
		}
		temp_porp.Name=asm_set.m_Name;
		temp_porp.Code=asm_set.m_InsCode;
		//���ģ�͵����
		asm_type.m_strFilter.Format("[Number]=%ld",asm_set.m_TypeNum);
		asm_type.Open();
		if (!asm_type.IsBOF())
		{
			asm_type.MoveFirst();
			temp_porp.TypeNum=asm_set.m_TypeNum;
			temp_porp.TypeName=asm_type.m_Name;
		}
		else
		{
			temp_porp.TypeNum=-1;
			temp_porp.TypeName="";
		}
		temp_porp.Person="";
		temp_porp.Note=asm_set.m_Note;
		temp_porp.IsAudited=(asm_set.m_AuditState==_T("���ͨ��")?true:false);
		//�ļ�����Դ
		if (asm_set.m_PDMState==_T("�ύ"))
			temp_porp.ResourceType=2;
		else if(asm_set.m_LocalState==_T("����"))
			temp_porp.ResourceType=0;
		else if(asm_set.m_LocalState==_T("������"))
			temp_porp.ResourceType=1;
		//���ģ�͵�����
		if (asm_set.m_MdlNum>0)
		{
			asmmdlset.m_strFilter.Format("[Number]=%ld",asm_set.m_MdlNum);
			asmmdlset.Open();
			asmmdlset.Requery();
			if (!asmmdlset.IsBOF())
			{
				asmmdlset.MoveFirst();
				temp_porp.MdlName=asmmdlset.m_Name;
			}
		}
		else temp_porp.MdlName="��ģ��";

		//���ù���ͼ
		if (!asm_set.m_DrawingFileName.IsEmpty() 
			&& LoadCompDrawing(temp_porp.ResourceType,asm_set.m_DrawingFileName,asm_set.m_DrawingFilePlace,&mdl_drw)==PRO_TK_NO_ERROR)
		{
			temp_porp.bHaveDraw=true;
			temp_porp.hDraw=mdl_drw;
		}
		else
		{
			temp_porp.bHaveDraw=false;
			temp_porp.hDraw=NULL;
		}

		SetMdlInfo(mdl,temp_porp);

		if (temp_porp.MdlType==1)
		{			//��ģ�������Ϣ
			asm_inf_fn.AddAsmInsInfo(m_pSet->m_pDatabase,mdl,num);
		}
	}
	return TRUE;
}
ProError CAsmTempOpen::LoadCompDrawing(int ResourceType,CString filename,CString filepath,ProDrawing * comp_dra)
{
	ProPath  name;
	CString  csName;
	ProError status;
	if (filepath.Right(1)!="\\")
	{
		filepath+="\\";
	}	
	switch(ResourceType) {
	case 0:
		csName=filepath+filename;
		break;
	case 1:
		csName=localDir+"\\"+filename;
		break;
	case 2:
		csName=localDir+"\\"+filename;
		break;
	default:
		return PRO_TK_NOT_VALID;
		break;
	}
	ProStringToWstring(name,csName.GetBuffer());
	status=ProMdlLoad(name,PRO_MDL_DRAWING,PRO_B_FALSE,(ProMdl*)comp_dra);
	return status;
}