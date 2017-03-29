#include "StdAfx.h"
#include "asmassembly.h"
#include "NProductInsInfoTable.h"
#include "NProductBOMTable.h"
#include "NPartInsInfoTable.h"
#include "NAsmType.h"
#include "FtpFile.h"
#include "AsmManageTableSet.h"
#include "CAsmPerfSet.h"
#include "CAsmPicSet.h"
#include "CAsmRelSet.h"
#include "CAsmVarSet.h"
#include "NAsmBomTable.h"
#include "AsmInfoFn.h"
#include "PartMdlRecordSet.h"
#include "NPartTypeSet.h"

CAsmAssembly::CAsmAssembly(void):asm_draw(NULL),m_db(NULL),m_asm_num(0),error_info(_T(""))
{
}

CAsmAssembly::~CAsmAssembly(void)
{
}
/************************************************************************/
/* �򿪲�Ʒʵ��															*/
/************************************************************************/
BOOL  CAsmAssembly::OpenProduct(CDatabase * p_db,long asm_num,ProSolid * asm_com)
{
	m_db=p_db;
	m_asm_num=asm_num;
	error_info.Empty();
	try
	{
		if (!GetCompInfo(asm_num))
		{
			return FALSE;
		}
		if (!CheckProduct(asm_num))
		{
			return FALSE;
		}
		if (!LoadSolid(asm_num))
		{
			return FALSE;
		}
		if (!GetCompMdl())
		{
			return FALSE;
		}
	}
	catch (CDBException * e) {
	     AfxMessageBox(e->m_strError);
		 return FALSE;
	}
	(*asm_com)=asm_solid;
	return TRUE;
}
/************************************************************************/
/* ������������ڵ���OpenProduct֮�����װ��ɹ��Ž���                */
/************************************************************************/
BOOL CAsmAssembly::AddInfo()
{
	if (m_db==NULL)
	{
		AfxMessageBox("���ݿ�������Ϣû��");
	}
	if (m_asm_num==0)
	{
		AfxMessageBox("û���������ʵ��");
	}
	try{
		if (!AddCompAsmInfo(asm_solid,asm_draw,m_asm_num))
		{
			return FALSE;
		}
		if (!AddComInfo(m_asm_num))
		{
			return FALSE;
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* װ�䲻�ɹ������ģ������ڴ�                                         */
/************************************************************************/
BOOL CAsmAssembly::CancelAssembly()
{
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if (!p_compitemlist[i].status && p_compitemlist[i].type=="asm")
		{
			ProMdlErase(p_compitemlist[i].mdl);
			if (p_compitemlist[i].drw!=NULL)
			{
				ProMdlErase(p_compitemlist[i].drw);
			}
		}
	}
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if (!p_compitemlist[i].status && p_compitemlist[i].type=="prt")
		{
			ProMdlErase(p_compitemlist[i].mdl);
			if (p_compitemlist[i].drw!=NULL)
			{
				ProMdlErase(p_compitemlist[i].drw);
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* �������������㲿���ļ������Լ�����ģ�͵���Ϣ��Ȼ�������ڵ�����ģ�ͽ��бȽ�
																		*/
/************************************************************************/
BOOL  CAsmAssembly::CheckProduct(long asm_num)
{
	ProMdl       tempmdl;
	ProName      name;
	CString      csName;
	UsrPartPorp  temp_prop;
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if (p_compitemlist[i].type == "asm")
		{
			csName=p_compitemlist[i].filename.Left(p_compitemlist[i].filename.GetLength()-4);
			ProStringToWstring(name,csName.GetBuffer());
			if (ProMdlInit(name,PRO_MDL_ASSEMBLY,&tempmdl)==PRO_TK_NO_ERROR)
			{
				//����Ѿ�����
				temp_prop=GetMdlInfo(tempmdl);
				if (temp_prop.Number!= p_compitemlist[i].num)
				{
					//���ͬ����ģ�;��в�ͬ�ı�ʶ�ţ�
					AfxMessageBox(p_compitemlist[i].filename+"�Ѿ����ڣ����ǲ�����ͬһ�����");
					return FALSE;
				}
				else
				{
					p_compitemlist[i].status=true;
				}
			}
			else
			{
				p_compitemlist[i].status=false;
			}
		}
		if (p_compitemlist[i].type == "prt")
		{
			csName=p_compitemlist[i].filename.Left(p_compitemlist[i].filename.GetLength()-4);
			ProStringToWstring(name,csName.GetBuffer());
			//����Ƿ��Ѿ�����
			if (ProMdlInit(name,PRO_MDL_PART,&tempmdl)==PRO_TK_NO_ERROR)
			{
				//����Ѿ�����
				temp_prop=GetMdlInfo(tempmdl);
				if (temp_prop.Number!= p_compitemlist[i].num)
				{
					//���ͬ����ģ�;��в�ͬ�ı�ʶ�ţ�
					AfxMessageBox(p_compitemlist[i].filename+"�Ѿ����ڣ����ǲ�����ͬһ�����");
					return FALSE;
				}
				else
				{
					p_compitemlist[i].status=true;  //�����ˣ�������ͬһ��
				}
			}
			else  //����ǲ�����
			{
				p_compitemlist[i].status=false;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* �ҵ�������㲿����������ʵ������Ʒʵ���еĶ�Ӧ��ϵ                 */
/************************************************************************/
BOOL  CAsmAssembly::GetCompInfo(long asm_num)
{
	CNProductInsInfoTable   asm_ins_set(m_db);
	CNProductBOMTable       asm_ins_bom(m_db);
	CNPartInsInfoTable	    prt_ins_set(m_db);	
	asm_ins_bom.m_strFilter.Format("[PdtNum]=%ld",asm_num);
	asm_ins_bom.Open();
	asm_ins_set.Open();
	prt_ins_set.Open();
	if(!asm_ins_bom.IsBOF())
	{
		asm_ins_bom.MoveFirst();
		while (!asm_ins_bom.IsEOF())
		{
			comp_item				temp_item;
			if (asm_ins_bom.m_Type.MakeLower() == "asm")
			{
				temp_item.type="asm";
				temp_item.num=asm_ins_bom.m_PrtNum;
				temp_item.mdlcomnum=asm_ins_bom.m_MdlComNum;
				temp_item.mdlnum=-1;
				//��øò���������ģ��
				asm_ins_set.m_strFilter.Format("[PdtNum]=%ld",asm_ins_bom.m_PrtNum);
				asm_ins_set.Requery();
				if (!asm_ins_set.IsBOF())
				{
					asm_ins_set.MoveFirst();
					temp_item.mdlnum=asm_ins_set.m_MdlNum;
					temp_item.pdmnum=asm_ins_set.m_PDMNum;
					temp_item.filename=asm_ins_set.m_SolidFileName;
					temp_item.filefullpath=asm_ins_set.m_SolidFilePlace;
					temp_item.drawfilename=asm_ins_set.m_DrawingFileName;
					temp_item.drawfilefullpath=asm_ins_set.m_DrawingFilePlace;

					if(asm_ins_set.m_PDMState=="�ύ")
					{
						temp_item.fileresource=2;
					}
					else if (asm_ins_set.m_LocalState=="����")
					{
						temp_item.fileresource=0;
					}
					else if (asm_ins_set.m_LocalState=="������")
					{
						temp_item.fileresource=1;
					}
					p_compitemlist.Add(temp_item);
				}
				else 
				{
					return FALSE;//�����װ�Ĳ����ڲ�Ʒʵ�����в������ˣ���ȡ��
				}
			}
			if (asm_ins_bom.m_Type.MakeLower() == "prt")
			{
				temp_item.type="prt";
				temp_item.num=asm_ins_bom.m_PrtNum;   //�����
				temp_item.mdlcomnum=asm_ins_bom.m_MdlComNum;
				temp_item.mdlnum=-1;                 //Ԥ������ģ��Ϊ-1
				//��øò���������ģ��
				prt_ins_set.m_strFilter.Format("[PrtNum]=%ld",asm_ins_bom.m_PrtNum);
				prt_ins_set.Requery();
				if (!prt_ins_set.IsBOF())
				{
					prt_ins_set.MoveFirst();
					temp_item.mdlnum=prt_ins_set.m_MdlNum;
					temp_item.pdmnum=prt_ins_set.m_PDMNum;
					temp_item.filename=prt_ins_set.m_SolidFileName;
					temp_item.filefullpath=prt_ins_set.m_SolidPlace;
					temp_item.drawfilename=prt_ins_set.m_DrawingName;
					temp_item.drawfilefullpath=prt_ins_set.m_DrawingPlace;
					if(prt_ins_set.m_PDMState=="�ύ")
					{
						temp_item.fileresource=2;
					}
					else if (prt_ins_set.m_LocalState=="����")
					{
						temp_item.fileresource=0;
					}
					else if (prt_ins_set.m_LocalState=="������")
					{
						temp_item.fileresource=1;
					}
					p_compitemlist.Add(temp_item);
				}
				else
				{
					return FALSE;//��������ʵ���в����ڣ���ȡ��
				}
			}
			asm_ins_bom.MoveNext();
		}
	}
	return TRUE;
}
/************************************************************************/
/* ���أ���ģ��														*/
/************************************************************************/
BOOL  CAsmAssembly::LoadSolid(long asm_num)
{
	CStringArray				files;   //�ڷ������ϵ��ļ���PDM�����ϵͳ���������ã�
	CStringArray                localfiles;//�ڱ����ϵ��ļ�
	CNProductInsInfoTable		asm_set(m_db);
	CString                     fileTemp1="",fileTemp2="";
	CString						csError;
	//1.����ļ��Ĵ��λ��
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if(p_compitemlist[i].status) continue;//����Ѿ��ڵ�ǰ�����ˣ�������
		switch(p_compitemlist[i].fileresource) {
		case 0:					//����
			localfiles.Add( p_compitemlist[i].filefullpath+"\\"+p_compitemlist[i].filename);
			if (!p_compitemlist[i].drawfilename.IsEmpty())
			{
				localfiles.Add( p_compitemlist[i].drawfilefullpath+"\\"+ p_compitemlist[i].drawfilename);
			}
			break;
		case 1:					//���ϵͳ
			files.Add( p_compitemlist[i].filefullpath+ "/"+p_compitemlist[i].filename+".1");
			if (!p_compitemlist[i].drawfilename.IsEmpty())
			{
				files.Add( p_compitemlist[i].drawfilefullpath+ "/"+p_compitemlist[i].drawfilename+".1");
			}
			break;
		default:				 //PDMϵͳ
			//ȥPDM��
		    UsrGetPartAndProductFromPdm(p_compitemlist[i].pdmnum,0,fileTemp1,fileTemp2);
			files.Add(fileTemp1);
			if (!p_compitemlist[i].drawfilename.IsEmpty()) files.Add(fileTemp2);
			break;
		}
	}
	

	//2.1����������Ĵ��λ��
	asm_set.m_strFilter.Format("[PdtNum]=%ld",asm_num);
	asm_set.Open();
	if (!asm_set.IsBOF())
	{
		asm_set.MoveFirst();
		if(asm_set.m_PDMState=="�ύ")
		{
			//ȥ����PDM��
			UsrGetPartAndProductFromPdm(asm_set.m_PDMNum,0,fileTemp1,fileTemp2);
			files.Add(fileTemp1);
			if (!asm_set.m_DrawingFileName.IsEmpty()) files.Add(fileTemp2);
		}
		else if (asm_set.m_LocalState=="����")
		{
			//�ӱ�����
			localfiles.Add(asm_set.m_SolidFilePlace+"\\"+asm_set.m_SolidFileName);
			if (!asm_set.m_DrawingFileName.IsEmpty())
			{
				localfiles.Add(asm_set.m_DrawingFilePlace+"\\"+asm_set.m_DrawingFileName);
			}			
		}
		else if (asm_set.m_LocalState=="������")
		{
			files.Add(asm_set.m_SolidFilePlace+"/"+asm_set.m_SolidFileName+".1");
			if (!asm_set.m_DrawingFileName.IsEmpty())
			{
				files.Add(asm_set.m_DrawingFilePlace+"/"+asm_set.m_DrawingFileName +".1");
			}	
		}
	}
	else return FALSE;//�����ʡ������

	//1.1����Ǳ����ļ�����Ҫ��鱾���Ƿ����
	if(localfiles.GetCount()>0)
	{
		if (!IsFilesExists(localfiles/*�ļ�����*/,csError))
		{
			AfxMessageBox(csError);
			return FALSE;
		}
	}
	//3.ѡ�񱣴�λ��,�����ļ���
	CString  newDir;	
	if (!SelectAnDirectory(localDir,""))
	{
		return FALSE;
	}	
	if (!CreateDirectoryGivenPathAndName(localDir,asm_set.m_Name,newDir))
	{
		return FALSE;
	}
	localDir=localDir+newDir;   //c:\\dd\\new
	//4.1���ص������ļ���
	CFtpFile                m_ftp;
	if(files.IsEmpty()) return FALSE;
	if (!m_ftp.DownloadFilestoDirectory(&files,localDir,myFtp,csError))
	{
		AfxMessageBox(csError);
		return FALSE;
	}
	//5.��ģ�͵����ڴ�
    //5.1�Ȱѱ��صĵ����ڴ�(���ص����)  ��������ܻ᲻̫���ʣ��Ժ���Ҫ�Ľ�
	ProError status;
	ProPath  name;
	CString  fullFileName;
	ProMdl   tempmdl;
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if(p_compitemlist[i].fileresource==0 && p_compitemlist[i].type=="asm")
		{
			fullFileName= p_compitemlist[i].filefullpath+"\\"+p_compitemlist[i].filename;
			ProStringToWstring(name,fullFileName.GetBuffer());
			status=ProMdlLoad(name,PRO_MDL_ASSEMBLY,PRO_B_FALSE,(ProMdl*)&tempmdl);
			if (!p_compitemlist[i].drawfilename.IsEmpty() )
			{
				fullFileName= p_compitemlist[i].drawfilefullpath+"\\"+p_compitemlist[i].drawfilename;
				ProStringToWstring(name,fullFileName.GetBuffer());
				status=ProMdlLoad(name,PRO_MDL_DRAWING,PRO_B_FALSE,(ProMdl*)&tempmdl);
			}
			if (status!=PRO_TK_NO_ERROR)
			{
				AfxMessageBox("��ģ�ͱ���ģ��"+fullFileName+"ʧ��");
				return FALSE;
			}
		}
	}
	//5.2�������
	fullFileName=localDir+"\\"+asm_set.m_SolidFileName;
	ProStringToWstring(name,fullFileName.GetBuffer());
	status=ProMdlLoad(name,PRO_MDL_ASSEMBLY,PRO_B_FALSE,(ProMdl*)&asm_solid); 
	if (!asm_set.m_DrawingFileName.IsEmpty())
	{
		fullFileName=localDir+"\\"+asm_set.m_DrawingFileName;
		ProStringToWstring(name,fullFileName.GetBuffer());
		status=ProMdlLoad(name,PRO_MDL_DRAWING,PRO_B_FALSE,(ProMdl*)&asm_draw); 
	}
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ģ��ʧ��");
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* ���Ԫ���ľ��                                                       */
/************************************************************************/
BOOL  CAsmAssembly::GetCompMdl()
{
	ProName        name;
	ProSolid       tempsolid;
	ProDrawing     tempdrw;
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		if (p_compitemlist[i].type=="asm")
		{
			ProStringToWstring(name,p_compitemlist[i].filename.Left(p_compitemlist[i].filename.GetLength()-4).GetBuffer());  
			if (ProMdlInit(name,PRO_MDL_ASSEMBLY,(ProMdl*)&tempsolid)==PRO_TK_NO_ERROR)
			{
				p_compitemlist[i].mdl=tempsolid;
				ProSolidToPostfixId(tempsolid,&(p_compitemlist[i].postid));
				if(!p_compitemlist[i].drawfilename.IsEmpty())
				{
					ProStringToWstring(name,p_compitemlist[i].drawfilename.Left(p_compitemlist[i].drawfilename.GetLength()-4).GetBuffer()); 
					if (ProMdlInit(name,PRO_MDL_DRAWING,(ProMdl*)&tempdrw))
					{
						p_compitemlist[i].drw=tempdrw;
					}
					else p_compitemlist[i].drw=NULL;
				}
			}
			{
				p_compitemlist[i].mdl=NULL;
			}
		}
		if (p_compitemlist[i].type=="prt")
		{
			ProStringToWstring(name,p_compitemlist[i].filename.Left(p_compitemlist[i].filename.GetLength()-4).GetBuffer());  
			if (ProMdlInit(name,PRO_MDL_PART,(ProMdl*)&tempsolid)==PRO_TK_NO_ERROR)
			{
				p_compitemlist[i].mdl=tempsolid;
				ProSolidToPostfixId(tempsolid,&(p_compitemlist[i].postid));
				if(!p_compitemlist[i].drawfilename.IsEmpty())
				{
					ProStringToWstring(name,p_compitemlist[i].drawfilename.Left(p_compitemlist[i].drawfilename.GetLength()-4).GetBuffer()); 
					if (ProMdlInit(name,PRO_MDL_DRAWING,(ProMdl*)&tempdrw))
					{
						p_compitemlist[i].drw=tempdrw;
					}
					else p_compitemlist[i].drw=NULL;
				}
			}
			else 
			{
				p_compitemlist[i].mdl=NULL;
			}
		}
	}
	return TRUE;
}
/************************************************************************/
/* �����ݿ��а�Ԫ���Ĺ�ϵʽ��ͼƬ���ɱ��������Ϣ��ӵ����Ե�ģ����     */
/************************************************************************/
BOOL  CAsmAssembly::AddComInfo(long asm_num)
{
	for (int i=0;i<p_compitemlist.GetCount();i++)
	{
		comp_item temp=p_compitemlist[i];
		if (p_compitemlist[i].type=="asm" /*&& p_compitemlist[i].mdlnum!=-1*/ && !p_compitemlist[i].status)
				AddCompAsmInfo(p_compitemlist[i].mdl,p_compitemlist[i].drw,p_compitemlist[i].num);
		if (p_compitemlist[i].type=="prt" /*&& p_compitemlist[i].mdlnum!=-1*/ && !p_compitemlist[i].status) 
				AddCompPartInfo(p_compitemlist[i].mdl,p_compitemlist[i].drw,p_compitemlist[i].num);
	}
	return TRUE;
}
/************************************************************************/
/* ����������Ϣ                                                       */
/************************************************************************/
BOOL CAsmAssembly::AddCompPartInfo(ProSolid part_solid,ProDrawing part_draw=NULL,long PrtNum=0)
{
	CNPartInsInfoTable		m_tempSet(m_db);	
	CNPartTypeSet           m_parttypeset(m_db);
	CPartMdlRecordSet       m_partmdlset(m_db);
	UsrPartPorp             temp_porp;

	m_tempSet.m_strFilter.Format("[PrtNum]=%ld",PrtNum);
	m_tempSet.Open();
	if(!m_tempSet.IsBOF())
	{
		m_tempSet.MoveFirst();
		temp_porp.owner  = part_solid;
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
				//��ģ�͵���Ϣ����ӽ���				
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
		temp_porp.IsAsmComp=false;
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
		//��ӹ���ͼ
		if (part_draw!=NULL)
		{
			temp_porp.bHaveDraw=true;
			temp_porp.hDraw=part_draw;
		}
		SetMdlInfo(part_solid,temp_porp);
		//���������Ϣ �������ģ�͵�ʵ������ģ�͵���Ϣ��ӵ�ʵ���У�
		//ע�⣬�������������������ģ�͵Ļ�����Ϣ֮�����
		//��Ϊ�����ڲ�Ҫ�޸�ģ�ͻ�����Ϣ
		if (temp_porp.MdlType==1)
		{
			UsrAddPartMdlOtherInfo(m_db,part_solid,temp_porp.MdlNum);
		}
	}
	return TRUE;
}
/************************************************************************/
/*����������Ϣ                                                        */
/************************************************************************/
BOOL CAsmAssembly::AddCompAsmInfo(ProSolid mdl,ProDrawing part_draw=NULL,long asm_num=0 /*���ʵ����*/)
{
	CNAsmType				asm_type(m_db);
	CNProductInsInfoTable		asm_set(m_db);
	CAsmManageTableSet          asm_mdl_set(m_db);
	UsrPartPorp					temp_porp;
	CAsmInfoFn                  asm_info_fn;
	//�����ԭ����
	EraseMdlInfo(asm_solid,ERASE_ALL);
	try
	{

		asm_set.m_strFilter.Format("[PdtNum]=%ld",asm_num);		
		asm_set.Open();
		if (!asm_set.IsBOF())
		{
			asm_set.MoveFirst();
			temp_porp.owner  = mdl;
			temp_porp.Number = asm_num;
			//�ж��������Ƿ�����ģ��
			switch(asm_set.m_MdlNum) {
		case -1:
			temp_porp.MdlType=2;           //��ģ�͵���������
			temp_porp.MdlNum=-1;
			break;
		default:
			temp_porp.MdlType=1;           //��ģ�͵�ʵ��
			temp_porp.MdlNum=asm_set.m_MdlNum;
			break;
			}			
			temp_porp.Name   =asm_set.m_Name;
			temp_porp.Code   =asm_set.m_InsCode;
			//���ģ�͵�����Լ��������
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
			temp_porp.Material="";
			temp_porp.Person  ="";
			temp_porp.Note    =asm_set.m_Note;
			temp_porp.IsAudited=(asm_set.m_AuditState==_T("���ͨ��")?true:false);
			//�ļ�����Դ
			if (asm_set.m_PDMState==_T("�ύ"))
				temp_porp.ResourceType=2;
			else if(asm_set.m_LocalState==_T("����"))
				temp_porp.ResourceType=0;
			else if(asm_set.m_LocalState==_T("������"))
				temp_porp.ResourceType=1;
			//װ�����Ԫ��
			temp_porp.IsAsmComp=true;
			//���ģ�͵�����
			if (asm_set.m_MdlNum>0)
			{
				asm_mdl_set.m_strFilter.Format("[Number]=%ld",asm_set.m_MdlNum);
				asm_mdl_set.Open();
				asm_mdl_set.Requery();
				if (!asm_mdl_set.IsBOF())
				{
					asm_mdl_set.MoveFirst();
					temp_porp.MdlName=asm_mdl_set.m_Name;
				}
			}
			else temp_porp.MdlName="��ģ��";
			//��ӹ���ͼ
			if (part_draw!=NULL)
			{
				temp_porp.bHaveDraw=true;
				temp_porp.hDraw=part_draw;
			}
			SetMdlInfo(mdl,temp_porp);
		}

		//��������ģ�͵�ʵ���������ģ�͵���Ϣ�����ʵ����
        if (temp_porp.MdlType==1)
        {
			asm_info_fn.AddAsmInsInfo(m_db,mdl,asm_num);
			return TRUE;
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
	}
	return TRUE;
}