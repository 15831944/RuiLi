#include "StdAfx.h"
#include "PartMdlRecordSet.h"
#include "partmdlopen.h"
//#include "partDrawingSet.h"
#include "partPerfparaTableSet.h"
#include "partPicSet.h"
#include "NPartInsInfoTable.h"
#include "partRelationSet.h"
#include "partVariantTableSet.h"
#include "UsrSWTXTable.h"
#include "NPartTypeSet.h"
#include "FtpFile.h"
CPartMdlOpen::CPartMdlOpen(void):part_draw(NULL),part_solid(NULL)
{
}

CPartMdlOpen::~CPartMdlOpen(void)
{
}
BOOL CPartMdlOpen::Open(CDatabase * p_db,long part_num)
{
	m_db = p_db;
	CPartMdlRecordSet  m_pSet(m_db);
	m_pSet.m_strFilter.Format("[Number]=%ld",part_num);
	try
	{
		m_pSet.Open();
		if (m_pSet.IsBOF())
		{
			AfxMessageBox("ѡ�е�ģ���Ѿ������ڣ�");
			return FALSE;
		}
		if (!CheckCurrentPartMdl(part_num))
		{
			return FALSE;
		}
		if (!LoadSolid(part_num))
		{
			return FALSE;
		}
		//���ģ��������Ϣ
		EraseMdlInfo(part_solid,ERASE_ALL);
		//��ģ�������Ϣ
		if (!AddPartMdlInfo(part_num))
		{
			return FALSE;
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		
	}
	return TRUE;
}
BOOL CPartMdlOpen::CheckCurrentPartMdl(long num)
{
	CPartMdlRecordSet   m_pSet(m_db);
	ProMdl				part_solid;
	ProType					type;
	ProName					w_name;
	int						w_id;
	ProError				status;
	CString                 error_info;
	m_pSet.m_strFilter.Format("[Number]=%ld",num);
	m_pSet.Open();
	if (m_pSet.IsBOF())
	{
		AfxMessageBox("ģ���Ѿ������ڣ�");
		return FALSE;
	}
	m_pSet.MoveFirst();
	switch(UsrCheckCurrentMdl(PRO_MDL_PART,m_pSet.m_SolidFileName,num,&part_solid)) {
	case CHECK_NAME_NUM_SAME:
		status = ProMdlTypeGet(part_solid, (ProMdlType*)&type);
		status = ProMdlNameGet(part_solid, w_name);
		status = ProObjectwindowCreate(w_name, type, &w_id);
		if(!status== PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
		status = ProMdlDisplay(part_solid);
		status = ProWindowActivate(w_id);		
		return FALSE;
		break;
	case CHECK_SNAME_NNUM:
		error_info.Format("���Ѿ��򿪾�����ͬ�ļ���(%s)�����,�������ѡ������ģ�Ͳ�ͬ��\n�޷�����Ϊ��򿪣��������ó���������",m_pSet.m_SolidFileName);
		AfxMessageBox(error_info);
		return FALSE;
		break;
	default:
		return TRUE;
	}
}
/************************************************************************/
/* ����ģ��                                                             */
/************************************************************************/
BOOL CPartMdlOpen::LoadSolid(long num)
{
	CPartMdlRecordSet		m_set(m_db);
	ProError				status;
	//ProPath				wcurrentDir;
	CStringArray			files;
	CFtpFile				m_ftp;
	CString					fullFileName;//����·�����ļ���
	ProPath					name;
	ProType					type;
	ProName					w_name;
	int						w_id;
	CString					newDir;	//�����������ļ���
	CString					csError;
	//1.ѡ����λ��
	if (!SelectAnDirectory(part_dir,""))
	{
		return FALSE;
	}
	//2.��ѡ��Ĵ��λ�������½�һ���ļ���
	m_set.m_strFilter.Format("[Number]=%ld",num);
	m_set.Open();
	if(m_set.IsBOF()) return FALSE;
	m_set.MoveFirst();
	if(m_set.m_SolidFileName.IsEmpty()) return FALSE;
	if (!CreateDirectoryGivenPathAndName(part_dir,m_set.m_Name,newDir))
	{
		return FALSE;
	}
	part_dir=part_dir+newDir;				//c:\\dd\\new

	//3.�����ļ����½����ļ�������
	files.Add(m_set.m_SolidFilePlace +"/"+m_set.m_SolidFileName+".1");
			//������ڹ���ͼ
	if(!m_set.m_DrawingFileName.IsEmpty()) files.Add(m_set.m_DrawingFilePlace +"/"+m_set.m_DrawingFileName+".1");
	if (!m_ftp.DownloadFilestoDirectory(&files,part_dir,myFtp,csError))
	{
		AfxMessageBox(csError);
		return FALSE;
	}

	//4.��ģ�͵����ڴ沢��
	fullFileName=part_dir+"\\"+m_set.m_SolidFileName+".1";
	ProStringToWstring(name,fullFileName.GetBuffer());
	status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&part_solid); 
	//����й���ͼ����򿪹���ͼ
	if(!m_set.m_DrawingFileName.IsEmpty())	
	{
		fullFileName=part_dir+"\\"+m_set.m_DrawingFileName+".1";
		ProStringToWstring(name,fullFileName.GetBuffer());
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&part_draw);
	}
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ģ��ʧ��");
		return FALSE;
	}	
	status = ProMdlTypeGet(part_solid, (ProMdlType*)&type);
	status=  ProMdlNameGet(part_solid, w_name);
	status=  ProObjectwindowCreate(w_name, type, &w_id);
	if(!status== PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	status = ProMdlDisplay(part_solid);
	status = ProWindowActivate(w_id);	
	return TRUE;
}
/************************************************************************/
/* ���������ʵ��                                                     */
/************************************************************************/
BOOL CPartMdlOpen::OpenPart(CDatabase * p_db,long part_num/*����ı�ʶ��*/)
{
	m_db=p_db;
	CNPartInsInfoTable  m_pSet(m_db);
	m_pSet.m_strFilter.Format("[PrtNum]=%ld",part_num);
	try
	{
		m_pSet.Open();
		if (m_pSet.IsBOF())
		{
			AfxMessageBox("ѡ�е���������ڣ�");
			return FALSE;
		}
		if (!CheckCurrentPart(part_num))
		{
			return FALSE;
		}
		if (!LoadPart(part_num))
		{
			return FALSE;
		}
		//���ģ��������Ϣ
		EraseMdlInfo(part_solid,ERASE_ALL);
		//�����������ģ�͵���Ϣ
		if (!AddPartInfo(part_num))
		{
			AfxMessageBox("SDFSD");
			return FALSE;
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		
	}
	return TRUE;
}
BOOL CPartMdlOpen::CheckCurrentPart(long num)
{
	CNPartInsInfoTable  m_pSet(m_db);
	ProMdl				part_solid;
	ProType					type;
	ProName					w_name;
	int						w_id;
	ProError				status;
	CString                 error_info;
	m_pSet.m_strFilter.Format("[PrtNum]=%ld",num);
	m_pSet.Open();
	if (m_pSet.IsBOF())
	{
		AfxMessageBox("����Ѿ������ڣ�");
		return FALSE;
	}
	m_pSet.MoveFirst();
	switch(UsrCheckCurrentMdl(PRO_MDL_PART,m_pSet.m_SolidFileName,num,&part_solid)) {
	case CHECK_NAME_NUM_SAME:
		status = ProMdlTypeGet(part_solid, (ProMdlType*)&type);
		status = ProMdlNameGet(part_solid, w_name);
		status = ProObjectwindowCreate(w_name, type, &w_id);
		if(!status== PRO_TK_NO_ERROR)
		{
			return FALSE;
		}
		status = ProMdlDisplay(part_solid);
		status = ProWindowActivate(w_id);		
		return FALSE;
		break;
	case CHECK_SNAME_NNUM:
		error_info.Format("���Ѿ��򿪾�����ͬ�ļ���(%s)�����,�������ѡ��������ͬ��\n�޷�����Ϊ��򿪣��������ó���������",m_pSet.m_SolidFileName);
		AfxMessageBox(error_info);
		return FALSE;
		break;
	default:
		return TRUE;
	}
}
/************************************************************************/
/* �����ʵ��                                                         */
/************************************************************************/
BOOL CPartMdlOpen::LoadPart(long num)
{
	CNPartInsInfoTable		m_set(m_db);
	ProError				status;
	ProPath					wcurrentDir;
	CStringArray			files;
	CFtpFile				m_ftp;
	CString					fullFileName;//����·�����ļ���
	ProPath					name;
	ProType					type;
	ProName					w_name;
	int						w_id;
	CString					newDir;	//�����������ļ���
	CString					csError;
	CString                 draw_dir;
	part_draw  = NULL;
	part_solid = NULL;

	//ѡ��Ŀ¼
	status   = ProDirectoryCurrentGet(wcurrentDir);
	part_dir = CString(wcurrentDir);       // "C:\\DFS\\"
	if (!SelectAnDirectory(part_dir,part_dir))
	{
		return FALSE;
	}

	m_set.m_strFilter.Format("[PrtNum]=%ld",num);
	m_set.Open();
	if(m_set.IsBOF()) return FALSE;
	m_set.MoveFirst();	

	//1�����ļ�
	if (m_set.m_PDMState==_T("�ύ"))
	{
		//����PDM����ȡģ�ͺ͹���ͼ�ĵ�ַ
		CString  fileTemp1="",fileTemp2="";
		if (!CreateDirectoryGivenPathAndName(part_dir,m_set.m_Name,newDir))
		{
			return FALSE;
		}
		part_dir=part_dir+newDir;
		draw_dir=part_dir;
		UsrGetPartAndProductFromPdm(m_set.m_PDMNum,0,fileTemp1,fileTemp2);
		if(!m_set.m_SolidFileName.IsEmpty()) 
		{					
			//files.Add(m_set.m_SolidPlace+"/"+m_set.m_SolidFileName+".1");
			files.Add(fileTemp1);
		}
		if(!m_set.m_DrawingName.IsEmpty())
		{
			//files.Add(m_set.m_DrawingPlace+"/"+m_set.m_DrawingName+".1");
			files.Add(fileTemp2);
		}	
		m_ftp.DownloadFilestoDirectory(&files,part_dir,myFtp,csError);
		if (!csError.IsEmpty())
		{
			AfxMessageBox(csError);
			return FALSE;
		}
	}
	else if (m_set.m_LocalState==_T("������"))
	{
		//���ݷ���������ȡģ�ͣ�����ͼ�ĵ�ַ,ͨ�����أ�
		//����Ҫ��һ�����ҹ��̣����Ƿ����ҵ��ƶ�ͼֽ
		if (!CreateDirectoryGivenPathAndName(part_dir,m_set.m_Name,newDir))
		{
			return FALSE;
		}
		part_dir=part_dir+newDir;
		draw_dir=part_dir;
		if(!m_set.m_SolidFileName.IsEmpty()) 
		{					
			files.Add(m_set.m_SolidPlace+"/"+m_set.m_SolidFileName+".1");
		}
		if(!m_set.m_DrawingName.IsEmpty())
		{
			files.Add(m_set.m_DrawingPlace+"/"+m_set.m_DrawingName+".1");
		}	
		m_ftp.DownloadFilestoDirectory(&files,part_dir,myFtp,csError);
		if (!csError.IsEmpty())
		{
			AfxMessageBox(csError);
			return FALSE;
		}
	}
	else if (m_set.m_LocalState==_T("����"))
	{
		//���ݱ��أ����ж��Ƿ�Ϊ�������������ж��ƶ���Ŀ¼�Ƿ�������ļ�������Ҫ����
		//�����ļ�����Ҫ�汾
		//���������ͬλ��������� 
		if(!m_set.m_SolidFileName.IsEmpty()) 
		{
			files.Add(m_set.m_SolidPlace+"\\"+m_set.m_SolidFileName);
			part_dir=m_set.m_SolidPlace;
		}					
		if(!m_set.m_DrawingName.IsEmpty())   
		{
			files.Add(m_set.m_DrawingPlace+"\\"+m_set.m_DrawingName);
			draw_dir=m_set.m_DrawingPlace;
		}	
		if (!IsFilesExists(files,csError))
		{
			AfxMessageBox(csError);
			return FALSE;
		}
	}
	//��ģ�͵����ڴ�
	fullFileName= part_dir+"\\"+m_set.m_SolidFileName;
	ProStringToWstring(name,fullFileName.GetBuffer());
	status		= ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&part_solid); 
	//����й���ͼ����򿪹���ͼ
	if(!m_set.m_DrawingName.IsEmpty())	
	{
		fullFileName=draw_dir+"\\"+m_set.m_DrawingName;
		ProStringToWstring(name,fullFileName.GetBuffer());
		status=ProMdlLoad(name,PRO_MDL_UNUSED,PRO_B_FALSE,(ProMdl*)&part_draw);
	}
	if (status!=PRO_TK_NO_ERROR)
	{
		AfxMessageBox("��ģ��ʧ��");
		return FALSE;
	}	
	status = ProMdlTypeGet(part_solid, (ProMdlType*)&type);
	status = ProMdlNameGet(part_solid, w_name);
	status = ProObjectwindowCreate(w_name, type, &w_id);
	if(!status== PRO_TK_NO_ERROR)
	{
		return FALSE;
	}
	status = ProMdlDisplay(part_solid);
	status = ProWindowActivate(w_id);	
	return TRUE;
}
/************************************************************************/
/* ������ģ�͵���Ϣ                                                   */
/************************************************************************/
BOOL CPartMdlOpen::AddPartMdlInfo(long PrtNum)
{	
	CNPartTypeSet           m_parttypeset(m_db);
	CPartMdlRecordSet       m_partmdlset(m_db);
	UsrPartPorp             temp_porp;
	m_partmdlset.m_strFilter.Format("[Number]=%ld",PrtNum);
	m_partmdlset.Open();
	if(!m_partmdlset.IsBOF())
	{
		m_partmdlset.MoveFirst();
		temp_porp.owner  = part_solid;
		temp_porp.Number = PrtNum;
		//����Ϊģ��
		temp_porp.MdlType=3;
		temp_porp.MdlNum=PrtNum;			
		temp_porp.Name   =m_partmdlset.m_Name;
		temp_porp.Code   =m_partmdlset.m_Code;
		//���ģ�͵�����Լ��������
		m_parttypeset.m_strFilter.Format("[Number]=%ld",m_partmdlset.m_TypeNum);
		m_parttypeset.Open();
		if (!m_parttypeset.IsBOF())
		{
			m_parttypeset.MoveFirst();
			temp_porp.TypeNum=m_partmdlset.m_TypeNum;
			temp_porp.TypeName=m_parttypeset.m_Name;
		}
		else
		{
			temp_porp.TypeNum=-1;
			temp_porp.TypeName="";
		}
		temp_porp.Material=m_partmdlset.m_Material;
		temp_porp.Person  ="";
		temp_porp.Note    =m_partmdlset.m_Note;
		temp_porp.IsAudited=false;
        temp_porp.ResourceType=1;
		temp_porp.IsAsmComp=false;
		temp_porp.MdlNum=-1;
		temp_porp.MdlName="��ģ��";
		//��ӹ���ͼ
		if (part_draw!=NULL)
		{
			temp_porp.bHaveDraw=true;
			temp_porp.hDraw=part_draw;
		}
		//�����ģ��������Ϣ��ӽ���
		UsrAddPartMdlOtherInfo(m_db,part_solid,PrtNum);
		SetMdlInfo(part_solid,temp_porp);	
	}
	return TRUE;
}
/************************************************************************/
/* ����������Ϣ                                                       */
/************************************************************************/
BOOL CPartMdlOpen::AddPartInfo(long PrtNum)
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

		if (temp_porp.MdlType==1)
		{
			//��ģ�͵���Ϣ����ӽ���
			UsrAddPartMdlOtherInfo(m_db,part_solid,m_tempSet.m_MdlNum);
		}
	}
	return TRUE;
}
BOOL CPartMdlOpen::AddPartMdlInfoForOtherUse(CDatabase * p_db,ProSolid solid,long part_num)
{
	m_db=p_db;
	part_solid=solid;
	EraseMdlInfo(solid,ERASE_ALL);
	return AddPartMdlInfo(part_num);
}