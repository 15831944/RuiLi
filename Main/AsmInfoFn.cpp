#include "StdAfx.h"
#include "asminfofn.h"
#include "CAsmPerfSet.h"
#include "CAsmPicSet.h"
#include "CAsmRelSet.h"
#include "CAsmVarSet.h"
#include "NAsmBomTable.h"
#include "NProductBOMTable.h"
#include "NProductInsInfoTable.h"
#include "NPartInsInfoTable.h"

CAsmInfoFn::CAsmInfoFn(void)
{
}

CAsmInfoFn::~CAsmInfoFn(void)
{
}
/************************************************************************/
/*给组件添加组件模型的性能参数信息                                      */
/************************************************************************/
BOOL CAsmInfoFn::AddAsmMdlPerfParaInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num)
{
	CAsmPerfSet             m_Set1(p_db);
	//添加性能参数信息
	m_Set1.m_strFilter.Format("[AsmNum]=%ld",asm_num);
	try{
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
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
/* 给组件添加组件模型的图片信息                                         */
/************************************************************************/
BOOL CAsmInfoFn::AddAsmMdlPicInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num)
{
	CAsmPicSet              m_Set2(p_db);
	m_Set2.m_strFilter.Format("[AsmNum]=%ld",asm_num);
	try
	{
		m_Set2.Open();
		if (!m_Set2.IsBOF())
		{
			m_Set2.MoveFirst();
			while (!m_Set2.IsEOF())
			{
				UsrPicture temp_pic;
				CString	   error_info;
				CString    temp_full_filename;
				int        tempnum=0;
				CString    mdlName=GetMdlInfo(asm_solid).Name;
				temp_pic.owner=asm_solid;
				temp_pic.isCheck=1;
				temp_pic.isMain=m_Set2.m_IsMain;
				temp_pic.fileName=mdlName+".jpg";
				temp_pic.filePath=GetMdlPath(asm_solid);			
				temp_full_filename=temp_pic.filePath+temp_pic.fileName;
				while (PathFileExistsA(temp_full_filename.GetBuffer()))
				{
					tempnum++;
					temp_pic.fileName.Format("%s_%d.jpg",mdlName,tempnum);
					temp_full_filename=temp_pic.filePath+temp_pic.fileName;
				}
				if (temp_pic.isMain==1)
				{
					SetMdlInfo(asm_solid)->pic_path=temp_pic.filePath+temp_pic.fileName;
				}
				temp_pic.Note=m_Set2.m_Note;		
				DBOpenPicSaveAsFile(&m_Set2.m_File,temp_pic.filePath+temp_pic.fileName);
				p_UsrPicture.Add(temp_pic);
				m_Set2.MoveNext();
			}
		}
	}
	catch (CDBException * e) {
		AfxMessageBox(e->m_strError);
		
		return FALSE;
	}
	return TRUE;
}
//把组件模型的事务特性表信息添加到组件模型中
BOOL CAsmInfoFn::AddAsmMdlSmlItemForMdl(CDatabase * p_db,ProMdl asm_solid,long asm_num)
{
	CAsmVarSet                  m_Set4(p_db);
	CNAsmBomTable				bom_set(p_db);
	CArray<ProSolid,ProSolid>	solidlist;
	ProSolid					tempsolid;
	ProName						name;
	ProMdlType                  mdltype;
	CArray<long,long>			comnumlist;
	//获得元件与编号的对应关系
	bom_set.m_strFilter.Format("[AsmNum]=%ld",asm_num);
	bom_set.Open();
	solidlist.Add((ProSolid)asm_solid);
	comnumlist.Add(0);
	if (!bom_set.IsBOF())
	{
		bom_set.MoveFirst();
		do {
			bom_set.m_Type=="prt"?(mdltype=PRO_MDL_PART):(mdltype=PRO_MDL_ASSEMBLY);
			ProStringToWstring(name,bom_set.m_FileName.Left(bom_set.m_FileName.GetLength()-4).GetBuffer());  // 去掉后缀得到文件名
			if (ProMdlInit(name,mdltype,(ProMdl*)&tempsolid)==PRO_TK_NO_ERROR)
			{
				solidlist.Add(tempsolid);
				comnumlist.Add(bom_set.m_ComNum);
			}			
			bom_set.MoveNext();
		} while(!bom_set.IsEOF());
	}
    m_Set4.m_strFilter.Format("[AsmNum]=%ld",asm_num);
	m_Set4.Open();
	if (!m_Set4.IsBOF())
	{
		m_Set4.MoveFirst();
		do {
				AsmSmlItem temp4;
				CString    csTemp=_T("");
				temp4.owner=(ProSolid)asm_solid;
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
					//预留
					temp4.marsk=TYPE_FEAT;
					p_AsmSmlItemList.Add(temp4);
				}				
				m_Set4.MoveNext();
		} while(!m_Set4.IsEOF());
	}
	return TRUE;
}
//把组件模型的事务特性表信息添加到组件实例中去
BOOL CAsmInfoFn::AddAsmMdlSmlItemForIns(CDatabase * p_db,ProMdl asm_solid,long asm_num/*实例号*/,long asm_mdl_num)
{
	CAsmVarSet                  m_Set4(p_db);
	CArray<ProSolid,ProSolid>	solidlist;
	CArray<long,long>			comnumlist;

	GetComAndComNumList(p_db,asm_solid,asm_num,solidlist,comnumlist);
	//添加事务特性表信息
    m_Set4.m_strFilter.Format("[AsmNum]=%ld",asm_mdl_num);
	m_Set4.Open();
	if (!m_Set4.IsBOF())
	{
		m_Set4.MoveFirst();
		do {
			AsmSmlItem	temp4;
			CString		csTemp=_T("");
			temp4.owner=(ProSolid)asm_solid;
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
				//预留
				temp4.marsk=TYPE_FEAT;
				p_AsmSmlItemList.Add(temp4);
			}				
			m_Set4.MoveNext();
		} while(!m_Set4.IsEOF());
	}
	return TRUE;
}
BOOL CAsmInfoFn::GetComAndComNumList(CDatabase * p_db,ProMdl asm_solid,long asm_num
						 ,CArray<ProSolid,ProSolid> & solidlist
						 ,CArray<long,long>	& comnumlist)
{
	CNProductInsInfoTable   asm_ins_set(p_db);
	CNProductBOMTable       asm_ins_bom(p_db);
	CNPartInsInfoTable	    prt_ins_set(p_db);	
	ProName					name;
	ProSolid				tempsolid;
	asm_ins_bom.m_strFilter.Format("[PdtNum]=%ld",asm_num);
	asm_ins_bom.Open();
	asm_ins_set.Open();
	prt_ins_set.Open();
	//组件本身
	solidlist.Add((ProSolid)asm_solid);
	comnumlist.Add(0);
	//组件下的元件
	if(!asm_ins_bom.IsBOF())
	{
		asm_ins_bom.MoveFirst();
		while (!asm_ins_bom.IsEOF())
		{
			if (asm_ins_bom.m_Type.MakeLower() == "asm")
			{
				asm_ins_set.m_strFilter.Format("[PdtNum]=%ld",asm_ins_bom.m_PrtNum);
				asm_ins_set.Requery();
				if(!asm_ins_set.IsBOF())
				{
					asm_ins_set.MoveFirst();
					ProStringToWstring(name,asm_ins_set.m_SolidFileName.Left(asm_ins_set.m_SolidFileName.GetLength()-4).GetBuffer());  
					if (ProMdlInit(name,PRO_MDL_ASSEMBLY,(ProMdl*)&tempsolid)==PRO_TK_NO_ERROR)
					{
						solidlist.Add(tempsolid);
						comnumlist.Add(asm_ins_bom.m_MdlComNum);
					}
				}				
			}
			if (asm_ins_bom.m_Type.MakeLower() == "prt")
			{
				prt_ins_set.m_strFilter.Format("[PrtNum]=%ld",asm_ins_bom.m_PrtNum);
				prt_ins_set.Requery();
				if (!prt_ins_set.IsBOF())
				{
					asm_ins_set.MoveFirst();
					ProStringToWstring(name,prt_ins_set.m_SolidFileName.Left(prt_ins_set.m_SolidFileName.GetLength()-4).GetBuffer());  
					if (ProMdlInit(name,PRO_MDL_PART,(ProMdl*)&tempsolid)==PRO_TK_NO_ERROR)
					{
						solidlist.Add(tempsolid);
						comnumlist.Add(asm_ins_bom.m_MdlComNum);
					}
				}
			}
			asm_ins_bom.MoveNext();
		}
	}
	return TRUE;
}
//添加组件的模型信息
BOOL CAsmInfoFn::AddAsmMdlInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num)
{
	try
	{	
		EraseMdlInfo(asm_solid,ERASE_ALL);
		if (!AddAsmMdlPerfParaInfo(p_db,asm_solid,asm_num))
		{
			return FALSE;
		}
		if (!AddAsmMdlPicInfo(p_db,asm_solid,asm_num))
		{
			return FALSE;
		}
		if (!AddAsmMdlSmlItemForMdl(p_db,asm_solid,asm_num))
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
//添加组件的模型信息
BOOL CAsmInfoFn::AddAsmInsInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num/*组件实例*/)
{
	long					asm_mdl_num=-1;
	CNProductInsInfoTable	asm_ins_set(p_db);
	try
	{
		asm_ins_set.m_strFilter.Format("[PdtNum]=%ld",asm_num);
		asm_ins_set.Open();
		if (asm_ins_set.IsBOF())
		{
			return FALSE;
		}
		if (asm_ins_set.m_MdlNum<=0)
		{
			return TRUE;
		}
		//获得组件实例的模型编号
		asm_mdl_num=asm_ins_set.m_MdlNum;
		EraseMdlInfo(asm_solid,ERASE_ALL);
		if (!AddAsmMdlPerfParaInfo(p_db,asm_solid,asm_mdl_num))
		{
			return FALSE;
		}
		if (!AddAsmMdlPicInfo(p_db,asm_solid,asm_mdl_num))
		{
			return FALSE;
		}
		if (!AddAsmMdlSmlItemForIns(p_db,asm_solid,asm_num/*与上面不同*/,asm_mdl_num))
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